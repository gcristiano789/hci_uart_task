/*
 * Copyright (c) 2016 Nordic Semiconductor ASA
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/byteorder.h>
#include <logging/log.h>
#include <sys/util.h>

#include <device.h>
#include <init.h>
#include <drivers/uart.h>

#include <net/buf.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/buf.h>
#include <bluetooth/hci_raw.h>
#include <at_command_bg96.h>

#define LOG_MODULE_NAME hci_uart_task
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static struct device *hci_uart_task_dev;
static K_THREAD_STACK_DEFINE(tx_thread_stack, CONFIG_BT_HCI_TX_STACK_SIZE);
static struct k_thread tx_thread_data;
static K_FIFO_DEFINE(tx_queue);

#define H4_CMD 0x01
#define H4_ACL 0x02
#define H4_SCO 0x03
#define H4_EVT 0x04

/* Length of a discard/flush buffer.
 * This is sized to align with a BLE HCI packet:
 * 1 byte H:4 header + 32 bytes ACL/event data
 * Bigger values might overflow the stack since this is declared as a local
 * variable, smaller ones will force the caller to call into discard more
 * often.
 */
#define H4_DISCARD_LEN 33

static int h4_read(struct device *uart, u8_t *buf,
		   size_t len, size_t min)
{
	int total = 0;

	while (len) {
		int rx;

		rx = uart_fifo_read(uart, buf, len);
		if (rx == 0) {
			LOG_DBG("Got zero bytes from UART");
			if (total < min) {
				continue;
			}
			break;
		}

		LOG_DBG("read %d remaining %d", rx, len - rx);
		len -= rx;
		total += rx;
		buf += rx;
	}

	return total;
}

static size_t h4_discard(struct device *uart, size_t len)
{
	u8_t buf[H4_DISCARD_LEN];

	return uart_fifo_read(uart, buf, MIN(len, sizeof(buf)));
}

static void h4_cmd_recv(struct net_buf *buf, int *remaining)
{
	struct bt_hci_cmd_hdr hdr;

	/* We can ignore the return value since we pass len == min */
	h4_read(hci_uart_task_dev, (void *)&hdr, sizeof(hdr), sizeof(hdr));

	*remaining = hdr.param_len;

	net_buf_add_mem(buf, &hdr, sizeof(hdr));

	LOG_DBG("len %u", hdr.param_len);
}

static void h4_acl_recv(struct net_buf *buf, int *remaining)
{
	struct bt_hci_acl_hdr hdr;

	/* We can ignore the return value since we pass len == min */
	h4_read(hci_uart_task_dev, (void *)&hdr, sizeof(hdr), sizeof(hdr));

	net_buf_add_mem(buf, &hdr, sizeof(hdr));

	*remaining = sys_le16_to_cpu(hdr.len);

	LOG_DBG("len %u", *remaining);
}

static void bt_uart_isr(struct device *unused)
{
	static struct net_buf *buf;
	static int remaining;

	ARG_UNUSED(unused);

	while (uart_irq_update(hci_uart_task_dev) &&
	       uart_irq_is_pending(hci_uart_task_dev)) {
		int read;

		if (!uart_irq_rx_ready(hci_uart_task_dev)) {
			if (uart_irq_tx_ready(hci_uart_task_dev)) {
				LOG_DBG("transmit ready");
			} else {
				LOG_DBG("spurious interrupt");
			}
			/* Only the UART RX path is interrupt-enabled */
			break;
		}

		/* Beginning of a new packet */
		if (!remaining) {
			u8_t type;

			/* Get packet type */
			read = h4_read(hci_uart_task_dev, &type, sizeof(type), 0);
			if (read != sizeof(type)) {
				LOG_WRN("Unable to read H4 packet type");
				continue;
			}

			buf = bt_buf_get_tx(BT_BUF_H4, K_NO_WAIT, &type,
					    sizeof(type));
			if (!buf) {
				return;
			}

			switch (bt_buf_get_type(buf)) {
			case BT_BUF_CMD:
				h4_cmd_recv(buf, &remaining);
				break;
			case BT_BUF_ACL_OUT:
				h4_acl_recv(buf, &remaining);
				break;
			default:
				LOG_ERR("Unknown H4 type %u", type);
				return;
			}

			LOG_DBG("need to get %u bytes", remaining);

			if (remaining > net_buf_tailroom(buf)) {
				LOG_ERR("Not enough space in buffer");
				net_buf_unref(buf);
				buf = NULL;
			}
		}

		if (!buf) {
			read = h4_discard(hci_uart_task_dev, remaining);
			LOG_WRN("Discarded %d bytes", read);
			remaining -= read;
			continue;
		}

		read = h4_read(hci_uart_task_dev, net_buf_tail(buf), remaining, 0);

		buf->len += read;
		remaining -= read;

		LOG_DBG("received %d bytes", read);

		if (!remaining) {
			LOG_DBG("full packet received");

			/* Put buffer into TX queue, thread will dequeue */
			net_buf_put(&tx_queue, buf);
			buf = NULL;
		}
	}
}

static void tx_thread(void *p1, void *p2, void *p3)
{
	while (1) {
		struct net_buf *buf;
		int err;

		/* Wait until a buffer is available */
		buf = net_buf_get(&tx_queue, K_FOREVER);
		/* Pass buffer to the stack */
		err = bt_send(buf);
		if (err) {
			LOG_ERR("Unable to send (err %d)", err);
			net_buf_unref(buf);
		}

		/* Give other threads a chance to run if tx_queue keeps getting
		 * new data all the time.
		 */
		k_yield();
	}
}

static int h4_send(struct net_buf *buf)
{
	LOG_DBG("buf %p type %u len %u", buf, bt_buf_get_type(buf),
		    buf->len);

	while (buf->len) {
		uart_poll_out(hci_uart_task_dev, net_buf_pull_u8(buf));
	}

	net_buf_unref(buf);

	return 0;
}

#if defined(CONFIG_BT_CTLR_ASSERT_HANDLER)
void bt_ctlr_assert_handle(char *file, u32_t line)
{
	u32_t len = 0U, pos = 0U;

	/* Disable interrupts, this is unrecoverable */
	(void)irq_lock();

	uart_irq_rx_disable(hci_uart_task_dev);
	uart_irq_tx_disable(hci_uart_task_dev);

	if (file) {
		while (file[len] != '\0') {
			if (file[len] == '/') {
				pos = len + 1;
			}
			len++;
		}
		file += pos;
		len -= pos;
	}

	uart_poll_out(hci_uart_task_dev, H4_EVT);
	/* Vendor-Specific debug event */
	uart_poll_out(hci_uart_task_dev, 0xff);
	/* 0xAA + strlen + \0 + 32-bit line number */
	uart_poll_out(hci_uart_task_dev, 1 + len + 1 + 4);
	uart_poll_out(hci_uart_task_dev, 0xAA);

	if (len) {
		while (*file != '\0') {
			uart_poll_out(hci_uart_task_dev, *file);
			file++;
		}
		uart_poll_out(hci_uart_task_dev, 0x00);
	}

	uart_poll_out(hci_uart_task_dev, line >> 0 & 0xff);
	uart_poll_out(hci_uart_task_dev, line >> 8 & 0xff);
	uart_poll_out(hci_uart_task_dev, line >> 16 & 0xff);
	uart_poll_out(hci_uart_task_dev, line >> 24 & 0xff);

	while (1) {
	}
}
#endif /* CONFIG_BT_CTLR_ASSERT_HANDLER */

static int hci_uart_task_init(struct device *unused)
{
	LOG_DBG("");

	/* Derived from DT's bt-c2h-uart chosen node */
	hci_uart_task_dev = device_get_binding(CONFIG_BT_CTLR_TO_HOST_UART_DEV_NAME);
	if (!hci_uart_task_dev) {
		return -EINVAL;
	}

	uart_irq_rx_disable(hci_uart_task_dev);
	uart_irq_tx_disable(hci_uart_task_dev);

	uart_irq_callback_set(hci_uart_task_dev, bt_uart_isr);

	uart_irq_rx_enable(hci_uart_task_dev);

	return 0;
}

static int hci_uart_send_at_commands(char at_command[]){

      if (IS_ENABLED(CONFIG_BT_WAIT_NOP)) {
              /* Issue a Command Complete with NOP */
              int i;

              const struct {
                      const u8_t h4;
                      const struct bt_hci_evt_hdr hdr;
                      const struct bt_hci_evt_cmd_complete cc;
              } __packed cc_evt = {
                      .h4 = H4_EVT,
                      .hdr = {
                              .evt = BT_HCI_EVT_CMD_COMPLETE,
                              .len = sizeof(struct bt_hci_evt_cmd_complete),
                      },
                      .cc = {
                              .ncmd = 1,
                              .opcode = sys_cpu_to_le16(BT_OP_NOP),
                      },
              };

              for (i = 0; i < sizeof(cc_evt); i++) {
                      uart_poll_out(hci_uart_task_dev,
                                    *(((const u8_t *)&cc_evt)+i));
              }

      }
}

DEVICE_INIT(hci_uart_task, "hci_uart_task", &hci_uart_task_init, NULL, NULL,
	    APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

void main(void)
{
	/* incoming events and data from the controller */
	static K_FIFO_DEFINE(rx_queue);
	int err;

	LOG_DBG("Start");
	__ASSERT(hci_uart_task_dev, "UART device is NULL");

	/* Enable the raw interface, this will in turn open the HCI driver */
	bt_enable_raw(&rx_queue);

	if (IS_ENABLED(CONFIG_BT_WAIT_NOP)) {
		/* Issue a Command Complete with NOP */
		int i;

		const struct {
			const u8_t h4;
			const struct bt_hci_evt_hdr hdr;
			const struct bt_hci_evt_cmd_complete cc;
		} __packed cc_evt = {
			.h4 = H4_EVT,
			.hdr = {
				.evt = BT_HCI_EVT_CMD_COMPLETE,
				.len = sizeof(struct bt_hci_evt_cmd_complete),
			},
			.cc = {
				.ncmd = 1,
				.opcode = sys_cpu_to_le16(BT_OP_NOP),
			},
		};

		for (i = 0; i < sizeof(cc_evt); i++) {
			uart_poll_out(hci_uart_task_dev,
				      *(((const u8_t *)&cc_evt)+i));
		}
	}

	/* Spawn the TX thread and start feeding commands and data to the
	 * controller
	 */
	k_thread_create(&tx_thread_data, tx_thread_stack,
			K_THREAD_STACK_SIZEOF(tx_thread_stack), tx_thread,
			NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);

	while (1) {
		struct net_buf *buf;

		buf = net_buf_get(&rx_queue, K_FOREVER);
		err = h4_send(buf);
		if (err) {
			LOG_ERR("Failed to send");
		}
	}
}
