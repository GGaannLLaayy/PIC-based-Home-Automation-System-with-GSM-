
#include <xc.h>
#include "usart.h"


void usart_init(void)
{

    TRISCbits.TRISC6 = 0;   
    TRISCbits.TRISC7 = 1;
    SPBRG = 25;                       // FOSC/(4* BAUD_RATE)-1

//    TXSTA = 1 << 5 |               // Transmit Enable
//            1 << 2;               // High Baud Rate 
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.SYNC = 0;            //Asynchronous mode select
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
   

   //PIE1bits.TXIE = 1;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


}

void usart_write(char data)
{
    while (!TXIF)
        ;
    //Now write the data to USART buffer
    TXREG = data;
}

void usart_print(const char *buffer)
{
    int i;
    for (i = 0; buffer[i] != '\0'; i++)
        usart_write(buffer[i]);
}

void usart_println(const char *buffer)
{
    usart_print(buffer);
    usart_write(0x0D); //Ascii values for '\r'  Line Feed
    usart_write(0x0A); // Ascii values for '\n'  Next Line
}

//char usart_rx(void)
//{
//    while(!RCIF)
//        ;
//    if(RCSTAbits.OERR)
//    {
//        RCSTAbits.CREN=0;
//        NOP();
//        RCSTAbits.CREN=0;
//        
//    }
//    return(RCREG);
//}
