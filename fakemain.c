/*
 * main.c
 *
 * Created: 01.04.2021 10:21:37
 * Author : e5000166
 *
 * Beispielprogramm f?r hc-sr04.c
 *
 * Ultraschallmodul: HC-SR04
 * _________________________
 *
 * power supply :		5V DC
 * quiescent current :	<2mA
 * effectual angle:		<15?
 * ranging distance :	2cm ? 500 cm
 * resolution :			0.3 cm
 * 
 */ 
#define F_CPU 8000000		// interner RC-Oscillator

#include <avr/io.h>
#include <stdbool.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hc-sr04.h"

int main(void)
{
	float x=0;
	DDRB = 0xff;	// Port B Output
	init_distance();
	
	while (true)
	{
		x=distance(8);
		if (x>=0)
		{
			PORTB = ~(uint8_t)(x*100); // Entfernung in cm ausgeben
		}
		else
		{
			PORTB = 0xff;	// Fehler ausgeben
		}
		
	}
}