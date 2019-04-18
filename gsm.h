/* 
 * File:   gsm.h
 * Author: Mahendra
 *
 * Created on August 17, 2018, 5:59 AM
 */

#ifndef GSM_H
#define	GSM_H

#ifdef	__cplusplus
extern "C" {
#endif

void Setup_messaging(void);
void Start_message(char *Phone_number);
void Type_message_content(char *Content);
void Send_message(void);
void Call_number(char *Phone_number);


#ifdef	__cplusplus
}
#endif

#endif	/* GSM_H */

