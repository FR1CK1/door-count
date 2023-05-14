/*
 * rtc.h
 *
 * Created: 11.08.2021 10:18:41
 *  Author: Ingolf Brunner
 */ 

// Quellen:
// siehe auch: https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR#Timer2_im_Asynchron_Mode
// Die Implementierung hier folgt dem Datenblatt des ATMEGA32

#ifndef RTC_H_
#define RTC_H_

#include <avr/interrupt.h>

void init_rtc(void);

//Variablen für die Zeit
volatile unsigned char sekunde;
volatile unsigned char minute;
volatile unsigned char stunde;



#endif /* RTC_H_ */