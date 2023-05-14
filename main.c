/*
* RTC_LCD-Display.c
*
* Original Created: 11.08.2021 08:52:32
* Author : Ingolf Brunner
*/

//#define F_CPU 3686400
// unter Project --> Properties --> Toolchain --> Symbols --> Defined Symbols:
// ergänzt:  F_CPU=3686400UL

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "lcd.h"

/*

Schaltplan

| Display | SDK500              |
| ------- | ------------------- |
| VSS     | GND                 |
| VDD     | 5V                  |
| V0      | Ponti-->GND und PC1 |
| RS      | PD4                 |
| RW      | PD5                 |
| E       | PD6                 |
| D0-D3   | Not in Use          |
| D4-D7   | PD0-PD3             |
| A       | PC1                 |
| K       | GND                 |
| Trigger | PC6					|
| Echo	  | PC7					|

*/

bool detect(){
		//6 Trigger - 7 Echo
		PORTC |= (1<<6);
		_delay_us(10);
		PORTC &= ~(1<<6);
			
		bool echo = false;
		for (int x = 0; x < 2000 ; x++)  //t in us/58=s in cm entfernung
		{
			if(PINC & (1<<7)){
				echo = true;
				_delay_us(1);
				} else {
				_delay_us(1);
			}
		}
		return echo;
}

int main(void)
{
	char buffer[16]="";
	char strbuff[16] = "";
	
	int zahl = 1;
	
	bool triggered = false;
	
	DDRB = 0xff;		//BoardLEDS
	DDRC = 0b01000010;	//Strom display und Echo
	PORTC |= (1 << 7);	//Setze 7 hochomig
	DDRD = 0xff;		// display
	
	//Display 1 sek nach Programmstart erst versorgen
	PORTC = 0x00;
	_delay_ms(1000);
	PORTC |= (1 << 1);
	
	// Initiaisierung
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	PORTB = 0xff;
	
	// https://www.mikrocontroller.net/articles/AVR-Tutorial:_Power_Management
	//set_sleep_mode(SLEEP_MODE_IDLE);
	//set_sleep_mode(SLEEP_MODE_PWR_DOWN);	// nur wenn INT0/INT1 level interrupt
	//set_sleep_mode(SLEEP_MODE_PWR_SAVE);	// nur wenn INT0/INT1 level interrupt
	// Standby und Extended Standby nur mit externen Takt
	//set_sleep_mode(SLEEP_MODE_STANDBY);	// nur wenn INT0/INT1 level interrupt
	//set_sleep_mode(SLEEP_MODE_EXT_STANDBY);	// nur wenn INT0/INT1 level interrupt
	
	
	while (true)
	{
		//Vorlage
		
		/*
		memset(buffer, 0, 16);	// buffer löschen
		strncat(buffer, "Hello World", 16); //16 Zeichen einwerfen
		lcd_gotoxy(0,0); // position (Spalte,Zeile) setzen
		lcd_puts(buffer); //Puffer schreiben
		*/
		
		
		//1. Zeile
		memset(buffer, 0, 16);	// buffer löschen
		strncat(buffer,"Baum:",16);
		lcd_gotoxy(0,0); // position (Spalte,Zeile) setzen
		lcd_puts(buffer); //Puffer schreiben
		
		
		//2. Zeile mit Zahl
		memset(buffer,0,16);
		sprintf(strbuff,"%d",zahl);
		strncat(buffer, strbuff, 16);
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		
		if (triggered){
			triggered = detect();
		} else {
			triggered = detect();
			if (triggered){
				zahl ++;
			}
		}
		_delay_ms(40);		
	}
}

