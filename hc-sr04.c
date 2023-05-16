/*
 * hc_sr04.c
 *
 * Created: 08.04.2021 14:21:45
 *  Author: Prof. Dr. Ingolf Brunner
 *
 * Ultraschallmodul: HC-SR04
 * _________________________
 *
 * power supply :		5V DC
 * quiescent current :	<2mA
 * effectual angle:		<15°
 * ranging distance :	2cm – 500 cm
 * resolution :			0.3 cm
 * 
 */

#include "hc-sr04.h"

volatile uint8_t ovf_error = 0;		// zeigt Timer Overflow an

/* 
 * 16 Bit Timer/Counter 1
 * Schallgeschwindigkeit in trockener Luft bei 20 °C: 343.2 m/s
 * t = 2 * 5 m / 343,2 m = 29,14 ms
 * clkTn = 1 / (29,14 ms / 2^16) = 2^16 / 0.02914 s = 2.249 MHz
 * (clkI/O) / 8 --> Prescaler
 * Messung bis 5 m bei Takt bis zu 8 * 2.249 Mhz = 17.992 MHz möglich
 */

void init_timer1(void)		// Zeitmessung bis echo
{
	// Takt im Prescaler durch 8 teilen
	TCCR1B |=  (1<<CS11);
	TCCR1B &= ~( (1<<CS12) | (1<<CS10) );	// clkI/O / 8 (From prescaler)
	// Overflow Interrupt aktivieren
	// TIMSK |=  (1<<TOIE1);
}

ISR (TIMER1_OVF_vect)	// Überlauf Timer1
{
	ovf_error = 2;		// Overflow Error setzen --> nicht messbar
}

void init_distance(void)
{
	init_timer1();
	UM_PORT_DDRx |= (1<<UM_TRIG);	// Trigger Pin Output
	sei();			// Interrupts global zulassen
}

float distance(uint8_t amax)
{
	uint8_t a=0;
	uint16_t i=0, tout=0;
	float x=0, x_m=0, t=0;
	const float v_schall = 343.2;		// Schallgeschwindigkeit in trockener Luft
	
	for (a=0; a<amax; a++)
	{
		// Trigger
		UM_PORTx |= (1<<UM_TRIG);	// Trigger setzen
		_delay_us(10);				// 10 µs warten
		UM_PORTx &= ~(1<<UM_TRIG);	// Trigger löschen
		
		tout=0;
		while (!(UM_PORT_PINx & (1<<UM_ECHO)) && (tout<=65536))	// warten, bis Echo gesetzt ist
		{
			tout++;
		}
		
		// Messung beginnen
		TCNT1 = 0;				// Timer 1 auf 0 setzen
		ovf_error = 0;			// Overflow Error rücksetzen
		TIFR &= ~(1<<TOV1);		// ggf. gesetzte Overflow Interrups löschen
		TIMSK |=  (1<<TOIE1);	// Overflow Interrupt aktivieren
		if (tout==65636)
		{
			ovf_error = 1;		// Overflow Error setzen --> Messung startet nach Trigger nicht
		}
		while (UM_PORT_PINx & (1<<UM_ECHO))		// warten, bis Messung beendet ist
		{
			;
		}
		i = TCNT1;					// Zähler auslesen
		// i = 127;		// das wird korrekt ausgegeben
		t = (i * 8.0 / F_CPU) / 2;	// Zeit für eine Strecke
		if (t > 0.037)
		{
			ovf_error = 4;	// Kein Echo
		}
		x = v_schall * t;
		x_m = x_m + x;	// Mittelwert bilden: aufaddieren	
		_delay_ms(50);	// mind. 50 ms zwischen zwei Messungen warten
	} // end for
	x_m = x_m / amax;	// Mittelwert berechnen
	
	if (ovf_error == 0)
	{
		return x_m;		// kein Overflow --> Messwert zurückgeben
	}
	else
	{
		return -ovf_error;	// Overflow/Fehler: Fehler zurückgeben
	}	
}