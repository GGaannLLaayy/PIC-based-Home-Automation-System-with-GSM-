#include <xc.h>
#include "gsm.h"
#include "usart.h"
#define _XTAL_FREQ   4000000

void Setup_messaging(void)
{
    usart_println("AT");
    __delay_ms(2000);
    usart_println("AT+CMGF=1"); // text mode
    __delay_ms(2000);
    usart_println("AT+CNMI=2,2,0,0,0"); // +CMT:...
    __delay_ms(2000);
    usart_println("AT+CMGD=1,4");
    __delay_ms(2000);
}

void Start_message(char *Phone_number)
{
    usart_print("AT+CMGS=\"");      // message send cmd
    usart_print(Phone_number);
    usart_print("\"");
    usart_print(0x0D);              //CR 13
    __delay_ms(3000);
}

void Type_message_content(char *Content)
{
    usart_print(Content);
}

void Send_message(void)
{
    usart_write(0x1A);                // ctrl+z

}

void Call_number(char *Phone_number)
{
    usart_print("ATD=");
    usart_print(Phone_number);
    usart_println(";");
}

//char Wait_for_message(char data)
//{
//    if ((data != 0x0D)&&(data != 0x0A))
//    {
//        message_buffer[index] = data;
//        index++;
//        return 0;
//    }
//    else
//    {
//        message_buffer[index] = '\0';
//        index = 0;
//        return 1;
//    }

