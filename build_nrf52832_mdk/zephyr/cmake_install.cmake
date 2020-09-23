# Install script for directory: C:/Users/Admin/ncs/v1.3.1/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/arch/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/lib/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/soc/arm/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/boards/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/subsys/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/drivers/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/nrf/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/mcuboot/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/mcumgr/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/nrfxlib/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/cmsis/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/canopennode/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/civetweb/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/fatfs/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/nordic/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/st/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/libmetal/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/lvgl/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/mbedtls/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/open-amp/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/loramac-node/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/openthread/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/segger/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/tinycbor/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/tinycrypt/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/littlefs/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/mipi-sys-t/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/modules/nrf_hw_models/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/kernel/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/cmake/flash/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/cmake/usage/cmake_install.cmake")
  include("C:/Users/Admin/ncs/v1.3.1/zephyr/samples/bluetooth/hci_uart_task/build_nrf52832_mdk/zephyr/cmake/reports/cmake_install.cmake")

endif()

