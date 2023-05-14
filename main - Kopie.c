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
| VDD     | PC1                 |
| V0      | Ponti-->GND und PC1 |
| RS      | PD4                 |
| RW      | PD5                 |
| E       | PD6                 |
| D0-D3   | Not in Use          |
| D4-D7   | PD0-PD3             |
| A       | PC1                 |
| K       | GND                 |

*/

int main(void)
{
	char buffer[16]="";
	char strbuff[16] = "";
	
	int zahl = 1;
	
	DDRB = 0xff;
	DDRC = 0xff;
	DDRD = 0xff;
	
	PORTC = 0b0;
	_delay_ms(100);
	PORTC = 0b11111111;
	
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
		strncat(buffer,"Sek seit Start:",16);
		lcd_gotoxy(0,0); // position (Spalte,Zeile) setzen
		lcd_puts(buffer); //Puffer schreiben
		
		
		//2. Zeile mit Zahl
		memset(buffer,0,16);
		sprintf(strbuff,"%d",zahl);
		strncat(buffer, strbuff, 16);
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		
		//Zahl erhöt um 1 und 1 sekunde warten
		zahl++;
		_delay_ms(1000);
		
	}
}

