/*
*
*List of the AT Commnads for the module: Quectel BG96
*
**/

/*
*The command delivers a product information text.
* Response
*   Quectel
*   BG96
*   Revision: <revision>
*   OK
*/

#define ATI "ATI"

/*
*The command returns a manufacturer identification text. See also AT+CGMI
*
* Response
*
*   Quectel
*   OK
*
*/

#define ATGMI "AT+GMI"

/*
*The command returns a product model identification text. It is identical with AT+CGMM.
*
* Response
*   BG96
*   OK
*
*/

#define ATGMM "AT+GMM"

/*
*The command delivers a product firmware version identification text. It is identical with AT+CGMR
*
* Response
*   <revision>
*   OK
*
*/

#define ATGMR "AT+GMR"

/*
*The command returns a manufacturer identification text. See also AT+GMI
*
* Response
*   Quectel
*   OK
*
*/

#define ATCGMI "AT+CGMI"

/*
*The command returns a product model identification text. It is identical with AT+GMM.
*
* Response
*   BG96
*   OK
*
*/

#define ATCGMM "AT+CGMM"

/*
*The command delivers a product firmware version identification text. It is identical with AT+GMR. 
*
* Response
*   <revision>
*   OK
*
*/

#define AT+CGMR "AT+CGMR"

/*
*The command returns the International Mobile Equipment Identity (IMEI). It is identical with AT+CGSN
*
* Response
*   <IMEI>
*   OK
*
*/

#define ATGSN "AT+GSN"

/*
*The command returns International Mobile Equipment Identity (IMEI). It is identical with AT+GSN
*
* Response
*   <IMEI>
*   OK
*
*/

#define ATCGSN "AT+CGSN"

/*
*The command resets AT command settings to their factory default values
*TA sets all current parameters to the manufacturer defined
*profile. See Table 8 on https://www.quectel.com/UploadImage/Downlad/Quectel_BG96_AT_Commands_Manual_V2.3.pdf.
*
* Response
*   OK
*
*/

//#define AT&F[<value>] "AT&F[<value>]"

/*
*The command displays the current settings of several AT command parameters, including the single-letter
AT command parameters which are not readable otherwise.
*
* Response
*   TA returns the current parameter settings. See Table 2 on https://www.quectel.com/UploadImage/Downlad/Quectel_BG96_AT_Commands_Manual_V2.3.pdf.
*   OK
*
*/

#define ATV "AT&V"


