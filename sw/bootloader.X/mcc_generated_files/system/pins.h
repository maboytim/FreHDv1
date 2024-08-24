/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.1.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0 aliases
#define TRS_ADDR3_TRIS                 TRISAbits.TRISA0
#define TRS_ADDR3_LAT                  LATAbits.LATA0
#define TRS_ADDR3_PORT                 PORTAbits.RA0
#define TRS_ADDR3_WPU                  WPUAbits.WPUA0
#define TRS_ADDR3_OD                   ODCONAbits.ODCA0
#define TRS_ADDR3_ANS                  ANSELAbits.ANSELA0
#define TRS_ADDR3_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define TRS_ADDR3_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define TRS_ADDR3_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define TRS_ADDR3_GetValue()           PORTAbits.RA0
#define TRS_ADDR3_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define TRS_ADDR3_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define TRS_ADDR3_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define TRS_ADDR3_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define TRS_ADDR3_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define TRS_ADDR3_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define TRS_ADDR3_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define TRS_ADDR3_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)
   
// get/set RA1 aliases
#define TRS_ADDR2_TRIS                 TRISAbits.TRISA1
#define TRS_ADDR2_LAT                  LATAbits.LATA1
#define TRS_ADDR2_PORT                 PORTAbits.RA1
#define TRS_ADDR2_WPU                  WPUAbits.WPUA1
#define TRS_ADDR2_OD                   ODCONAbits.ODCA1
#define TRS_ADDR2_ANS                  ANSELAbits.ANSELA1
#define TRS_ADDR2_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define TRS_ADDR2_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define TRS_ADDR2_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define TRS_ADDR2_GetValue()           PORTAbits.RA1
#define TRS_ADDR2_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define TRS_ADDR2_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define TRS_ADDR2_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define TRS_ADDR2_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define TRS_ADDR2_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define TRS_ADDR2_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define TRS_ADDR2_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define TRS_ADDR2_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)
   
// get/set RA2 aliases
#define TRS_ADDR1_TRIS                 TRISAbits.TRISA2
#define TRS_ADDR1_LAT                  LATAbits.LATA2
#define TRS_ADDR1_PORT                 PORTAbits.RA2
#define TRS_ADDR1_WPU                  WPUAbits.WPUA2
#define TRS_ADDR1_OD                   ODCONAbits.ODCA2
#define TRS_ADDR1_ANS                  ANSELAbits.ANSELA2
#define TRS_ADDR1_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define TRS_ADDR1_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define TRS_ADDR1_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define TRS_ADDR1_GetValue()           PORTAbits.RA2
#define TRS_ADDR1_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define TRS_ADDR1_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define TRS_ADDR1_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define TRS_ADDR1_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define TRS_ADDR1_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define TRS_ADDR1_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define TRS_ADDR1_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define TRS_ADDR1_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)
   
// get/set RA3 aliases
#define TRS_ADDR0_TRIS                 TRISAbits.TRISA3
#define TRS_ADDR0_LAT                  LATAbits.LATA3
#define TRS_ADDR0_PORT                 PORTAbits.RA3
#define TRS_ADDR0_WPU                  WPUAbits.WPUA3
#define TRS_ADDR0_OD                   ODCONAbits.ODCA3
#define TRS_ADDR0_ANS                  ANSELAbits.ANSELA3
#define TRS_ADDR0_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define TRS_ADDR0_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define TRS_ADDR0_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define TRS_ADDR0_GetValue()           PORTAbits.RA3
#define TRS_ADDR0_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define TRS_ADDR0_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define TRS_ADDR0_SetPullup()          do { WPUAbits.WPUA3 = 1; } while(0)
#define TRS_ADDR0_ResetPullup()        do { WPUAbits.WPUA3 = 0; } while(0)
#define TRS_ADDR0_SetPushPull()        do { ODCONAbits.ODCA3 = 0; } while(0)
#define TRS_ADDR0_SetOpenDrain()       do { ODCONAbits.ODCA3 = 1; } while(0)
#define TRS_ADDR0_SetAnalogMode()      do { ANSELAbits.ANSELA3 = 1; } while(0)
#define TRS_ADDR0_SetDigitalMode()     do { ANSELAbits.ANSELA3 = 0; } while(0)
   
// get/set RA4 aliases
#define TRS_RD_N_TRIS                 TRISAbits.TRISA4
#define TRS_RD_N_LAT                  LATAbits.LATA4
#define TRS_RD_N_PORT                 PORTAbits.RA4
#define TRS_RD_N_WPU                  WPUAbits.WPUA4
#define TRS_RD_N_OD                   ODCONAbits.ODCA4
#define TRS_RD_N_ANS                  ANSELAbits.ANSELA4
#define TRS_RD_N_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define TRS_RD_N_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define TRS_RD_N_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define TRS_RD_N_GetValue()           PORTAbits.RA4
#define TRS_RD_N_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define TRS_RD_N_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define TRS_RD_N_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define TRS_RD_N_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define TRS_RD_N_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define TRS_RD_N_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define TRS_RD_N_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define TRS_RD_N_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)
   
// get/set RA5 aliases
#define SD_CS_TRIS                 TRISAbits.TRISA5
#define SD_CS_LAT                  LATAbits.LATA5
#define SD_CS_PORT                 PORTAbits.RA5
#define SD_CS_WPU                  WPUAbits.WPUA5
#define SD_CS_OD                   ODCONAbits.ODCA5
#define SD_CS_ANS                  ANSELAbits.ANSELA5
#define SD_CS_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define SD_CS_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define SD_CS_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define SD_CS_GetValue()           PORTAbits.RA5
#define SD_CS_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define SD_CS_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define SD_CS_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define SD_CS_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define SD_CS_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define SD_CS_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define SD_CS_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define SD_CS_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)
   
// get/set RB0 aliases
#define SPARE1_TRIS                 TRISBbits.TRISB0
#define SPARE1_LAT                  LATBbits.LATB0
#define SPARE1_PORT                 PORTBbits.RB0
#define SPARE1_WPU                  WPUBbits.WPUB0
#define SPARE1_OD                   ODCONBbits.ODCB0
#define SPARE1_ANS                  ANSELBbits.ANSELB0
#define SPARE1_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define SPARE1_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define SPARE1_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define SPARE1_GetValue()           PORTBbits.RB0
#define SPARE1_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define SPARE1_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define SPARE1_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define SPARE1_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define SPARE1_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define SPARE1_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define SPARE1_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define SPARE1_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)
   
// get/set RB1 aliases
#define GAL_INT_TRIS                 TRISBbits.TRISB1
#define GAL_INT_LAT                  LATBbits.LATB1
#define GAL_INT_PORT                 PORTBbits.RB1
#define GAL_INT_WPU                  WPUBbits.WPUB1
#define GAL_INT_OD                   ODCONBbits.ODCB1
#define GAL_INT_ANS                  ANSELBbits.ANSELB1
#define GAL_INT_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define GAL_INT_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define GAL_INT_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define GAL_INT_GetValue()           PORTBbits.RB1
#define GAL_INT_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define GAL_INT_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define GAL_INT_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define GAL_INT_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define GAL_INT_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define GAL_INT_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define GAL_INT_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define GAL_INT_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)
   
// get/set RB2 aliases
#define RTC_INT_TRIS                 TRISBbits.TRISB2
#define RTC_INT_LAT                  LATBbits.LATB2
#define RTC_INT_PORT                 PORTBbits.RB2
#define RTC_INT_WPU                  WPUBbits.WPUB2
#define RTC_INT_OD                   ODCONBbits.ODCB2
#define RTC_INT_ANS                  ANSELBbits.ANSELB2
#define RTC_INT_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RTC_INT_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RTC_INT_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RTC_INT_GetValue()           PORTBbits.RB2
#define RTC_INT_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RTC_INT_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RTC_INT_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define RTC_INT_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define RTC_INT_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define RTC_INT_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define RTC_INT_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define RTC_INT_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)
   
// get/set RB3 aliases
#define TRS_WAIT_TRIS                 TRISBbits.TRISB3
#define TRS_WAIT_LAT                  LATBbits.LATB3
#define TRS_WAIT_PORT                 PORTBbits.RB3
#define TRS_WAIT_WPU                  WPUBbits.WPUB3
#define TRS_WAIT_OD                   ODCONBbits.ODCB3
#define TRS_WAIT_ANS                  ANSELBbits.ANSELB3
#define TRS_WAIT_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define TRS_WAIT_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define TRS_WAIT_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define TRS_WAIT_GetValue()           PORTBbits.RB3
#define TRS_WAIT_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define TRS_WAIT_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define TRS_WAIT_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define TRS_WAIT_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define TRS_WAIT_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define TRS_WAIT_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define TRS_WAIT_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define TRS_WAIT_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)
   
// get/set RB4 aliases
#define SD_CD_TRIS                 TRISBbits.TRISB4
#define SD_CD_LAT                  LATBbits.LATB4
#define SD_CD_PORT                 PORTBbits.RB4
#define SD_CD_WPU                  WPUBbits.WPUB4
#define SD_CD_OD                   ODCONBbits.ODCB4
#define SD_CD_ANS                  ANSELBbits.ANSELB4
#define SD_CD_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define SD_CD_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define SD_CD_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define SD_CD_GetValue()           PORTBbits.RB4
#define SD_CD_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define SD_CD_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define SD_CD_SetPullup()          do { WPUBbits.WPUB4 = 1; } while(0)
#define SD_CD_ResetPullup()        do { WPUBbits.WPUB4 = 0; } while(0)
#define SD_CD_SetPushPull()        do { ODCONBbits.ODCB4 = 0; } while(0)
#define SD_CD_SetOpenDrain()       do { ODCONBbits.ODCB4 = 1; } while(0)
#define SD_CD_SetAnalogMode()      do { ANSELBbits.ANSELB4 = 1; } while(0)
#define SD_CD_SetDigitalMode()     do { ANSELBbits.ANSELB4 = 0; } while(0)
   
// get/set RB5 aliases
#define SD_WP_TRIS                 TRISBbits.TRISB5
#define SD_WP_LAT                  LATBbits.LATB5
#define SD_WP_PORT                 PORTBbits.RB5
#define SD_WP_WPU                  WPUBbits.WPUB5
#define SD_WP_OD                   ODCONBbits.ODCB5
#define SD_WP_ANS                  ANSELBbits.ANSELB5
#define SD_WP_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define SD_WP_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define SD_WP_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define SD_WP_GetValue()           PORTBbits.RB5
#define SD_WP_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define SD_WP_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define SD_WP_SetPullup()          do { WPUBbits.WPUB5 = 1; } while(0)
#define SD_WP_ResetPullup()        do { WPUBbits.WPUB5 = 0; } while(0)
#define SD_WP_SetPushPull()        do { ODCONBbits.ODCB5 = 0; } while(0)
#define SD_WP_SetOpenDrain()       do { ODCONBbits.ODCB5 = 1; } while(0)
#define SD_WP_SetAnalogMode()      do { ANSELBbits.ANSELB5 = 1; } while(0)
#define SD_WP_SetDigitalMode()     do { ANSELBbits.ANSELB5 = 0; } while(0)
   
// get/set RB6 aliases
#define LED_1_TRIS                 TRISBbits.TRISB6
#define LED_1_LAT                  LATBbits.LATB6
#define LED_1_PORT                 PORTBbits.RB6
#define LED_1_WPU                  WPUBbits.WPUB6
#define LED_1_OD                   ODCONBbits.ODCB6
#define LED_1_ANS                  ANSELBbits.ANSELB6
#define LED_1_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define LED_1_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define LED_1_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define LED_1_GetValue()           PORTBbits.RB6
#define LED_1_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define LED_1_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define LED_1_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define LED_1_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define LED_1_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define LED_1_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define LED_1_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define LED_1_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)
   
// get/set RB7 aliases
#define LED_0_TRIS                 TRISBbits.TRISB7
#define LED_0_LAT                  LATBbits.LATB7
#define LED_0_PORT                 PORTBbits.RB7
#define LED_0_WPU                  WPUBbits.WPUB7
#define LED_0_OD                   ODCONBbits.ODCB7
#define LED_0_ANS                  ANSELBbits.ANSELB7
#define LED_0_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define LED_0_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define LED_0_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define LED_0_GetValue()           PORTBbits.RB7
#define LED_0_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define LED_0_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define LED_0_SetPullup()          do { WPUBbits.WPUB7 = 1; } while(0)
#define LED_0_ResetPullup()        do { WPUBbits.WPUB7 = 0; } while(0)
#define LED_0_SetPushPull()        do { ODCONBbits.ODCB7 = 0; } while(0)
#define LED_0_SetOpenDrain()       do { ODCONBbits.ODCB7 = 1; } while(0)
#define LED_0_SetAnalogMode()      do { ANSELBbits.ANSELB7 = 1; } while(0)
#define LED_0_SetDigitalMode()     do { ANSELBbits.ANSELB7 = 0; } while(0)
   
// get/set RC0 aliases
#define STAT_CS_TRIS                 TRISCbits.TRISC0
#define STAT_CS_LAT                  LATCbits.LATC0
#define STAT_CS_PORT                 PORTCbits.RC0
#define STAT_CS_WPU                  WPUCbits.WPUC0
#define STAT_CS_OD                   ODCONCbits.ODCC0
#define STAT_CS_ANS                  ANSELCbits.ANSELC0
#define STAT_CS_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define STAT_CS_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define STAT_CS_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define STAT_CS_GetValue()           PORTCbits.RC0
#define STAT_CS_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define STAT_CS_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define STAT_CS_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define STAT_CS_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define STAT_CS_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define STAT_CS_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define STAT_CS_SetAnalogMode()      do { ANSELCbits.ANSELC0 = 1; } while(0)
#define STAT_CS_SetDigitalMode()     do { ANSELCbits.ANSELC0 = 0; } while(0)
   
// get/set RC1 aliases
#define SDA_TRIS                 TRISCbits.TRISC1
#define SDA_LAT                  LATCbits.LATC1
#define SDA_PORT                 PORTCbits.RC1
#define SDA_WPU                  WPUCbits.WPUC1
#define SDA_OD                   ODCONCbits.ODCC1
#define SDA_ANS                  ANSELCbits.ANSELC1
#define SDA_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SDA_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SDA_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SDA_GetValue()           PORTCbits.RC1
#define SDA_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SDA_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SDA_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SDA_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SDA_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SDA_SetAnalogMode()      do { ANSELCbits.ANSELC1 = 1; } while(0)
#define SDA_SetDigitalMode()     do { ANSELCbits.ANSELC1 = 0; } while(0)
   
// get/set RC2 aliases
#define SCL_TRIS                 TRISCbits.TRISC2
#define SCL_LAT                  LATCbits.LATC2
#define SCL_PORT                 PORTCbits.RC2
#define SCL_WPU                  WPUCbits.WPUC2
#define SCL_OD                   ODCONCbits.ODCC2
#define SCL_ANS                  ANSELCbits.ANSELC2
#define SCL_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SCL_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SCL_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SCL_GetValue()           PORTCbits.RC2
#define SCL_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define SCL_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define SCL_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define SCL_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define SCL_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define SCL_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define SCL_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)
   
// get/set RC3 aliases
#define SPI_CLK_TRIS                 TRISCbits.TRISC3
#define SPI_CLK_LAT                  LATCbits.LATC3
#define SPI_CLK_PORT                 PORTCbits.RC3
#define SPI_CLK_WPU                  WPUCbits.WPUC3
#define SPI_CLK_OD                   ODCONCbits.ODCC3
#define SPI_CLK_ANS                  ANSELCbits.ANSELC3
#define SPI_CLK_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define SPI_CLK_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define SPI_CLK_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define SPI_CLK_GetValue()           PORTCbits.RC3
#define SPI_CLK_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define SPI_CLK_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define SPI_CLK_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define SPI_CLK_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define SPI_CLK_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define SPI_CLK_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define SPI_CLK_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define SPI_CLK_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)
   
// get/set RC4 aliases
#define SPI_SDI_TRIS                 TRISCbits.TRISC4
#define SPI_SDI_LAT                  LATCbits.LATC4
#define SPI_SDI_PORT                 PORTCbits.RC4
#define SPI_SDI_WPU                  WPUCbits.WPUC4
#define SPI_SDI_OD                   ODCONCbits.ODCC4
#define SPI_SDI_ANS                  ANSELCbits.ANSELC4
#define SPI_SDI_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define SPI_SDI_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define SPI_SDI_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define SPI_SDI_GetValue()           PORTCbits.RC4
#define SPI_SDI_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define SPI_SDI_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define SPI_SDI_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define SPI_SDI_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define SPI_SDI_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define SPI_SDI_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define SPI_SDI_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define SPI_SDI_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)
   
// get/set RC5 aliases
#define SPI_SDO_TRIS                 TRISCbits.TRISC5
#define SPI_SDO_LAT                  LATCbits.LATC5
#define SPI_SDO_PORT                 PORTCbits.RC5
#define SPI_SDO_WPU                  WPUCbits.WPUC5
#define SPI_SDO_OD                   ODCONCbits.ODCC5
#define SPI_SDO_ANS                  ANSELCbits.ANSELC5
#define SPI_SDO_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define SPI_SDO_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define SPI_SDO_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define SPI_SDO_GetValue()           PORTCbits.RC5
#define SPI_SDO_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define SPI_SDO_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define SPI_SDO_SetPullup()          do { WPUCbits.WPUC5 = 1; } while(0)
#define SPI_SDO_ResetPullup()        do { WPUCbits.WPUC5 = 0; } while(0)
#define SPI_SDO_SetPushPull()        do { ODCONCbits.ODCC5 = 0; } while(0)
#define SPI_SDO_SetOpenDrain()       do { ODCONCbits.ODCC5 = 1; } while(0)
#define SPI_SDO_SetAnalogMode()      do { ANSELCbits.ANSELC5 = 1; } while(0)
#define SPI_SDO_SetDigitalMode()     do { ANSELCbits.ANSELC5 = 0; } while(0)
   
// get/set RC6 aliases
#define TX_TRIS                 TRISCbits.TRISC6
#define TX_LAT                  LATCbits.LATC6
#define TX_PORT                 PORTCbits.RC6
#define TX_WPU                  WPUCbits.WPUC6
#define TX_OD                   ODCONCbits.ODCC6
#define TX_ANS                  ANSELCbits.ANSELC6
#define TX_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define TX_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define TX_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define TX_GetValue()           PORTCbits.RC6
#define TX_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define TX_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define TX_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define TX_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define TX_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define TX_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define TX_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define TX_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)
   
// get/set RC7 aliases
#define RX_TRIS                 TRISCbits.TRISC7
#define RX_LAT                  LATCbits.LATC7
#define RX_PORT                 PORTCbits.RC7
#define RX_WPU                  WPUCbits.WPUC7
#define RX_OD                   ODCONCbits.ODCC7
#define RX_ANS                  ANSELCbits.ANSELC7
#define RX_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RX_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RX_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RX_GetValue()           PORTCbits.RC7
#define RX_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RX_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RX_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define RX_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define RX_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define RX_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define RX_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RX_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)
   
// get/set RD0 aliases
#define TRS_DATA0_TRIS                 TRISDbits.TRISD0
#define TRS_DATA0_LAT                  LATDbits.LATD0
#define TRS_DATA0_PORT                 PORTDbits.RD0
#define TRS_DATA0_WPU                  WPUDbits.WPUD0
#define TRS_DATA0_OD                   ODCONDbits.ODCD0
#define TRS_DATA0_ANS                  ANSELDbits.ANSELD0
#define TRS_DATA0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define TRS_DATA0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define TRS_DATA0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define TRS_DATA0_GetValue()           PORTDbits.RD0
#define TRS_DATA0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define TRS_DATA0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define TRS_DATA0_SetPullup()          do { WPUDbits.WPUD0 = 1; } while(0)
#define TRS_DATA0_ResetPullup()        do { WPUDbits.WPUD0 = 0; } while(0)
#define TRS_DATA0_SetPushPull()        do { ODCONDbits.ODCD0 = 0; } while(0)
#define TRS_DATA0_SetOpenDrain()       do { ODCONDbits.ODCD0 = 1; } while(0)
#define TRS_DATA0_SetAnalogMode()      do { ANSELDbits.ANSELD0 = 1; } while(0)
#define TRS_DATA0_SetDigitalMode()     do { ANSELDbits.ANSELD0 = 0; } while(0)
   
// get/set RD1 aliases
#define TRS_DATA1_TRIS                 TRISDbits.TRISD1
#define TRS_DATA1_LAT                  LATDbits.LATD1
#define TRS_DATA1_PORT                 PORTDbits.RD1
#define TRS_DATA1_WPU                  WPUDbits.WPUD1
#define TRS_DATA1_OD                   ODCONDbits.ODCD1
#define TRS_DATA1_ANS                  ANSELDbits.ANSELD1
#define TRS_DATA1_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define TRS_DATA1_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define TRS_DATA1_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define TRS_DATA1_GetValue()           PORTDbits.RD1
#define TRS_DATA1_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define TRS_DATA1_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define TRS_DATA1_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define TRS_DATA1_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define TRS_DATA1_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define TRS_DATA1_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define TRS_DATA1_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define TRS_DATA1_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)
   
// get/set RD2 aliases
#define TRS_DATA2_TRIS                 TRISDbits.TRISD2
#define TRS_DATA2_LAT                  LATDbits.LATD2
#define TRS_DATA2_PORT                 PORTDbits.RD2
#define TRS_DATA2_WPU                  WPUDbits.WPUD2
#define TRS_DATA2_OD                   ODCONDbits.ODCD2
#define TRS_DATA2_ANS                  ANSELDbits.ANSELD2
#define TRS_DATA2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define TRS_DATA2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define TRS_DATA2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define TRS_DATA2_GetValue()           PORTDbits.RD2
#define TRS_DATA2_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define TRS_DATA2_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define TRS_DATA2_SetPullup()          do { WPUDbits.WPUD2 = 1; } while(0)
#define TRS_DATA2_ResetPullup()        do { WPUDbits.WPUD2 = 0; } while(0)
#define TRS_DATA2_SetPushPull()        do { ODCONDbits.ODCD2 = 0; } while(0)
#define TRS_DATA2_SetOpenDrain()       do { ODCONDbits.ODCD2 = 1; } while(0)
#define TRS_DATA2_SetAnalogMode()      do { ANSELDbits.ANSELD2 = 1; } while(0)
#define TRS_DATA2_SetDigitalMode()     do { ANSELDbits.ANSELD2 = 0; } while(0)
   
// get/set RD3 aliases
#define TRS_DATA3_TRIS                 TRISDbits.TRISD3
#define TRS_DATA3_LAT                  LATDbits.LATD3
#define TRS_DATA3_PORT                 PORTDbits.RD3
#define TRS_DATA3_WPU                  WPUDbits.WPUD3
#define TRS_DATA3_OD                   ODCONDbits.ODCD3
#define TRS_DATA3_ANS                  ANSELDbits.ANSELD3
#define TRS_DATA3_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define TRS_DATA3_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define TRS_DATA3_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define TRS_DATA3_GetValue()           PORTDbits.RD3
#define TRS_DATA3_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define TRS_DATA3_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define TRS_DATA3_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define TRS_DATA3_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define TRS_DATA3_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define TRS_DATA3_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define TRS_DATA3_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define TRS_DATA3_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)
   
// get/set RD4 aliases
#define TRS_DATA4_TRIS                 TRISDbits.TRISD4
#define TRS_DATA4_LAT                  LATDbits.LATD4
#define TRS_DATA4_PORT                 PORTDbits.RD4
#define TRS_DATA4_WPU                  WPUDbits.WPUD4
#define TRS_DATA4_OD                   ODCONDbits.ODCD4
#define TRS_DATA4_ANS                  ANSELDbits.ANSELD4
#define TRS_DATA4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define TRS_DATA4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define TRS_DATA4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define TRS_DATA4_GetValue()           PORTDbits.RD4
#define TRS_DATA4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define TRS_DATA4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define TRS_DATA4_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define TRS_DATA4_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define TRS_DATA4_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define TRS_DATA4_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define TRS_DATA4_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define TRS_DATA4_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)
   
// get/set RD5 aliases
#define TRS_DATA5_TRIS                 TRISDbits.TRISD5
#define TRS_DATA5_LAT                  LATDbits.LATD5
#define TRS_DATA5_PORT                 PORTDbits.RD5
#define TRS_DATA5_WPU                  WPUDbits.WPUD5
#define TRS_DATA5_OD                   ODCONDbits.ODCD5
#define TRS_DATA5_ANS                  ANSELDbits.ANSELD5
#define TRS_DATA5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define TRS_DATA5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define TRS_DATA5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define TRS_DATA5_GetValue()           PORTDbits.RD5
#define TRS_DATA5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define TRS_DATA5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define TRS_DATA5_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define TRS_DATA5_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define TRS_DATA5_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define TRS_DATA5_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define TRS_DATA5_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define TRS_DATA5_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)
   
// get/set RD6 aliases
#define TRS_DATA6_TRIS                 TRISDbits.TRISD6
#define TRS_DATA6_LAT                  LATDbits.LATD6
#define TRS_DATA6_PORT                 PORTDbits.RD6
#define TRS_DATA6_WPU                  WPUDbits.WPUD6
#define TRS_DATA6_OD                   ODCONDbits.ODCD6
#define TRS_DATA6_ANS                  ANSELDbits.ANSELD6
#define TRS_DATA6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define TRS_DATA6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define TRS_DATA6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define TRS_DATA6_GetValue()           PORTDbits.RD6
#define TRS_DATA6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define TRS_DATA6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define TRS_DATA6_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define TRS_DATA6_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define TRS_DATA6_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define TRS_DATA6_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define TRS_DATA6_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define TRS_DATA6_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)
   
// get/set RD7 aliases
#define TRS_DATA7_TRIS                 TRISDbits.TRISD7
#define TRS_DATA7_LAT                  LATDbits.LATD7
#define TRS_DATA7_PORT                 PORTDbits.RD7
#define TRS_DATA7_WPU                  WPUDbits.WPUD7
#define TRS_DATA7_OD                   ODCONDbits.ODCD7
#define TRS_DATA7_ANS                  ANSELDbits.ANSELD7
#define TRS_DATA7_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define TRS_DATA7_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define TRS_DATA7_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define TRS_DATA7_GetValue()           PORTDbits.RD7
#define TRS_DATA7_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define TRS_DATA7_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define TRS_DATA7_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define TRS_DATA7_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define TRS_DATA7_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define TRS_DATA7_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define TRS_DATA7_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define TRS_DATA7_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)
   
// get/set RE2 aliases
#define SPARE2_TRIS                 TRISEbits.TRISE2
#define SPARE2_LAT                  LATEbits.LATE2
#define SPARE2_PORT                 PORTEbits.RE2
#define SPARE2_WPU                  WPUEbits.WPUE2
#define SPARE2_OD                   ODCONEbits.ODCE2
#define SPARE2_ANS                  ANSELEbits.ANSELE2
#define SPARE2_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define SPARE2_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define SPARE2_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define SPARE2_GetValue()           PORTEbits.RE2
#define SPARE2_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define SPARE2_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define SPARE2_SetPullup()          do { WPUEbits.WPUE2 = 1; } while(0)
#define SPARE2_ResetPullup()        do { WPUEbits.WPUE2 = 0; } while(0)
#define SPARE2_SetPushPull()        do { ODCONEbits.ODCE2 = 0; } while(0)
#define SPARE2_SetOpenDrain()       do { ODCONEbits.ODCE2 = 1; } while(0)
#define SPARE2_SetAnalogMode()      do { ANSELEbits.ANSELE2 = 1; } while(0)
#define SPARE2_SetDigitalMode()     do { ANSELEbits.ANSELE2 = 0; } while(0)
   
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/