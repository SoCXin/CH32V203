
================================================================================
                                Sample instructions
================================================================================
Version history
Data               Version    Owner       IAR       MDK         Description
2020-07-01        1.1       Willow      8.32      5.26        IAP_UART add para
================================================================================
Function description
================================================================================
This sample mainly shows the  IAP upgrade firmware , using UARTx and various 
serial port transmission protocols.

================================================================================
Testing environment
================================================================================
Test board:
---------------------
MM32 coreboard 

Auxiliary tools:
---------------------
Logic analyzer
Power generator
USB cable
MM32-Link

Auxiliary software:
---------------------
Gitlab
HEX merge tool & hextobin tool
Serial port tools (MM32 IAP TOOL)

================================================================================
How to use it
================================================================================
1£©Open the project, compile and run£»
2£©Using serial port tool and defined transport protocol to send upgrade 
   command to demo board£»
3£©Then to send the APP bin file like above , after a while, you can see that 
    the light on the PB5 pin blinks once a second.



================================================================================
Be careful
================================================================================
Make sure that the serial port and transport protocol are the same as those 
configured in the config.h file .
================================================================================


