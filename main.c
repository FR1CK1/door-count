// unter Project --> Properties --> Toolchain --> Symbols --> Defined Symbols:
// ergänzt:  F_CPU=8000000 für delay-Zeiten

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
| D4-D7   | PD0-PD3             |
| A       | PC1                 |
| K       | GND                 |




|Ultras.  | SDK500              |
| ------- | ------------------- |
| Trigger | PC6					|
| Echo	  | PC7					|

*/


int main(void)
{
	char buffer[16]="";			//Buffer für Display
	char strline[16] = "";		//string vorbereitet für die Übertragung
	
	int iAnzahl = 0;			//Anzahl an Personen im Raum
	float dDistance = 0;		//Aktuelle Entfernung vom Ultraschall
	float dDistanceCal = -1;	//Kallibrierte Entfernung vom Ultraschall
	int errorCount = 0;			//Fehlerzähler
	bool somebody_here = false; //Erkennung, ob wer sich im Regelbereich vom US befindet
	
	//Initialisierung Board
	DDRB = 0xff;			//BoardLEDS
	DDRC |= (1 << 1);		//Stromversorgung
	
	//Display und US werden durch jeweilige Initfunktionen aktiviert
	
	//Zusatzgeräte 1 sek nach Programmstart erst versorgen
	PORTC = 0x00;
	_delay_ms(1000);
	PORTC |= (1 << 1);
	
	// Initiaisierung Display
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	
	//initialisierung Ultraschall
	init_distance();		
	
	
	//Kallibrierung Ultraschall
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
	
	//Kallibrierung
	while(dDistanceCal < 0){
		dDistanceCal = distance(14);		
		errorCount++;
		_delay_ms(50);
		if (errorCount > 20)
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
	
	//Leeren vom Display, Abschalten der Board LEDs
	lcd_clrscr();
	_delay_ms(100);
	PORTB = 0xff;
	
	
	//Board-LEDs als Entferungsangabe nutzen
	PORTB = ~(uint8_t)(dDistanceCal*100);
	_delay_ms(500);

	
	
	while (true)
	{
		//Vorlage für Texte
		
		/*
		memset(buffer, 0, 16);						//buffer löschen
		strncat(buffer, "Hello World", 16);			//16 Zeichen einwerfen
		lcd_gotoxy(0,0);							//position (Spalte,Zeile) setzen
		lcd_puts(buffer);							//Puffer schreiben
		*/
		
		
		//Messung durchführen
		dDistance=distance(8);
		
		//Fehlerprüfung
		if (dDistance>=0)
		{
			PORTB = ~(uint8_t)(dDistance*100); //Distanz auf Board-LEDs ausgeben
		}
		else
		{
			PORTB = 0x00;	// Fehler ausgeben
		}
		
		//Vergleich, ob US eine Verringerung feststellt
		if ((uint8_t)(dDistance*100) < (uint8_t)(dDistanceCal*100))
		{
			//Pürfen, ob schon jemand erkannt wurde
			if (somebody_here)
			{
				//Person befindet sich bereits im Messbereich
				_delay_ms(100);
			} else {
				//Person hat Messbereich betreten
				iAnzahl++;
				somebody_here = true;
				_delay_ms(100);
			}
		} else {
			//Person hat Messbereich verlassen
			somebody_here = false;
			_delay_ms(100);
		}	
		
		//1. Zeile
		memset(buffer, 0, 16);
		strncat(buffer,"Personen:",16);
		lcd_gotoxy(0,0);
		lcd_puts(buffer);
		
		//2. Zeile mit Zahl
		memset(buffer,0,16);
		sprintf(strline,"%d",iAnzahl);
		strncat(buffer, strline, 16);
		lcd_gotoxy(0,1);
		lcd_puts(buffer);
		
		/*
		Eventuelle Optimierung:
		Display nicht ansteuern, wenn keine Aktuallisierung notwendig ist
		Für Netzbetrieb uninteressant
		Für Batteriebetrieb notwendig
		*/
		
	}
}

