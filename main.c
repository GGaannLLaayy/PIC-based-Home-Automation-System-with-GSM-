/* 
 * File:   main.c
 * Author: Mahendra
 *
 * Created on August 17, 2018, 12:30 AM
 */
// PIC18F4520 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gsm.h"
#include "adc.h"
#include "usart.h"

#define _XTAL_FREQ   4000000
#define Ph_Num       "09795838275"

char serialRead(void);
void RecSMS(void);
void ClearBuffers(void);
char serialAvailable(void);



char rx_data, state, flag, rflag, dflag;
char IncData;
int x = 0;

char message_buffer[60];
int RcvdCheck = 0;
int RcvdConf = 0;
int index = 0;
int RcvdEnd = 0;
char MsgMob[15];
char MsgTxt[10];
int MsgLength = 0;

/*
 * 
 */
int main(void)
{

    ADC_init();
    usart_init();
    __delay_ms(3000);
    Setup_messaging();
    Start_message("09795838275");
    Type_message_content("Welcome");
    Send_message();
    
    TRISBbits.RB0 = 0;
    PORTBbits.RB0 = 1;
    while (1)
    {
        RecSMS();
        if (state == 1)
        {
            Start_message(Ph_Num);
            Type_message_content("Light ON");
            Send_message();
        }
    }

    return 0;
}

void __interrupt() isr(void)
{
    if (RCIF) // looking for receieve interrupt
    {
        rx_data = RCREG;
        flag = 1;
        //RCIF=0;
        if (RCSTAbits.OERR) /* check if any overrun occur due to continuous reception */
        {
            CREN = 0;
            NOP();
            CREN = 1;
        }
    }

}

char serialRead(void)
{
    flag = 0;
    return rx_data;
}

void RecSMS(void) // Receiving the SMS and extracting the Sender Mobile number &amp; Message Text
{

    if (serialRead())
    {
        IncData = serialRead();
        //UART1_Write_Text(IncData);
        if (IncData == '+')
        {
            RcvdCheck = 1;
        }
        if ((IncData == 'C') && (RcvdCheck == 1))
        {
            RcvdCheck = 2;
        }
        if ((IncData == 'M') && (RcvdCheck == 2))
        {
            RcvdCheck = 3;
        }
        if ((IncData == 'T') && (RcvdCheck == 3))
        {
            RcvdCheck = 4;
        }
        if (RcvdCheck == 4)
        {
            index = 0;
            RcvdConf = 1;
            RcvdCheck = 0;
        }

        if (RcvdConf == 1)
        {
            if (IncData == '\n')
            {
                RcvdEnd++;
            }
            if (RcvdEnd == 3)
            {
                RcvdEnd = 0;
            }
            message_buffer[index] = IncData;
            index++;

            if (RcvdEnd == 2)
            {
                RcvdConf = 0;
                MsgLength = index - 2;
                index = 0;
            }
            if (RcvdConf == 0)
            {

                //Mobile Number Save 
                for (x = 4; x < 17; x++)
                {
                    MsgMob[x - 4] = message_buffer[x];
                    //UART1_Write(MsgMob[x-4]);
                }

                // UART1_Write_Text("Message Text: ");
                for (x = 44; x < MsgLength; x++)
                {
                    MsgTxt[x - 44] = message_buffer[x];
                    //UART1_Write(MsgTxt[x-46]);
                }
                state = 0;
                if (MsgTxt[0] == 'A')
                {
                    state = 1;
                }//Light ON
                //                if (MsgTxt[0] == 'B')
                //                {
                //                    state = 2;
                //                }//Light OFF
                //                if (MsgTxt[0] == 'C')
                //                {
                //                    state = 2;
                //                }//Door Lock ON
                //                if (MsgTxt[0] == 'D')
                //                {
                //                    state = 3;
                //                }//Door Lock OFF
                //                if (MsgTxt[0] == 'E')
                //                {
                //                    state = 4;
                //                }//Water Level Read

                ClearBuffers();
            }
        }
    }
}

void ClearBuffers(void)
{
    strcpy(message_buffer, "");
    RcvdCheck = 0;
    RcvdConf = 0;
    index = 0;
    RcvdEnd = 0;
    strcpy(MsgMob, "");
    strcpy(MsgTxt, "");
    MsgLength = 0;
}

//char serialAvailable(void)
//{
////    char Temp;
////	if (OERR==1)
////	    Temp=RCREG;
////	if (FERR==1)
////	{
////		SPEN=0;
////		SPEN=1;
////    }
//	return RCIF;
//}