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
#include "hc-sr04.h"

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


int main(void)
{
	char buffer[16]="";		//Buffer für Display
	char strline[16] = "";	//string vorbereitet für die Übertragun
	
	int iAnzahl = 1;		//Anzahl an Personen im Raum
	float dDistance = 0;	//Entfernung vom Ultraschall (nicht geeicht)
	float dDistanceCal = -1;
	int errorCount = 0;
	bool somebody_here = false;
	
	
	DDRB = 0xff;			//BoardLEDS
	DDRC |= (1 << 1);		//Strom
	DDRD = 0xff;			//display
	
	//Zusatzgeräte 1 sek nach Programmstart erst versorgen
	PORTC = 0x00;
	_delay_ms(1000);
	PORTC |= (1 << 1);
	

	
	// Initiaisierung Display
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	
	//Kalibrierung Ultraschall
	
	init_distance();		//initialisierung Ultraschall
	
	//1. Zeile
	memset(buffer, 0, 16);
	strncat(buffer,"Kalibirere",16);
	lcd_gotoxy(0,0);
	lcd_puts(buffer);
	
	//2. Zeile
	memset(buffer, 0, 16);
	strncat(buffer,"Ultraschall...",16);
	lcd_gotoxy(0,1);
	lcd_puts(buffer);
	
	while(dDistanceCal < 0){
		dDistanceCal = distance(8);		
		errorCount++;
		_delay_ms(50);
		if (errorCount > 100)
		{
			//1. Zeile
			memset(buffer, 0, 16);
			strncat(buffer,"Kalibirerung",16);
			lcd_gotoxy(0,0);
			lcd_puts(buffer);
				
			//2. Zeile
			memset(buffer, 0, 16);
			strncat(buffer,"fehlgeschlagen",16);
			lcd_gotoxy(0,1);
			lcd_puts(buffer);
			
			while (true)
			{
				PORTB = 0x00;
				_delay_ms(250);
				PORTB = 0xff;
				_delay_ms(250);
			}
				
		}

	}
	
	lcd_clrscr();
	_delay_ms(100);
	PORTB = 0xff;
	
	PORTB = ~(uint8_t)(dDistanceCal*100); // Entfernung in cm ausgeben
	_delay_ms(5000);

	
	
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
		memset(buffer, 0, 16);
		strncat(buffer,"Baum:",16);
		lcd_gotoxy(0,0);
		lcd_puts(buffer);
		
		dDistance=distance(8);
		if (dDistance>=0)
		{
			PORTB = ~(uint8_t)(dDistance*100); // Entfernung in cm ausgeben
		}
		else
		{
			PORTB = 0xff;	// Fehler ausgeben
		}
		
		if ((uint8_t)(dDistance*100) < (uint8_t)(dDistanceCal*100))
		{
			
			if (somebody_here)
			{
				_delay_ms(100);
			} else {
				iAnzahl++;
				somebody_here = true;
				_delay_ms(100);
			}
		} else {
			somebody_here = false;
			_delay_ms(100);
		}	
		
		//2. Zeile mit Zahl
		memset(buffer,0,16);
		sprintf(strline,"%d",iAnzahl);
		strncat(buffer, strline, 16);
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		
	}
}

