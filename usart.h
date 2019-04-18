/* 
 * File:   usart.h
 * Author: Mahendra
 *
 * Created on August 17, 2018, 5:59 AM
 */

#ifndef USART_H
#define	USART_H

#ifdef	__cplusplus
extern "C" {
#endif

void usart_init(void);
void usart_write(char data);
void usart_print(const char *buffer);
void usart_println(const char *buffer);


#ifdef	__cplusplus
}
#endif

#endif	/* USART_H */

