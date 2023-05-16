/*
 * hc_sr04.h
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


#define F_CPU 8000000		// interner RC-Oscillator
#define UM_PORTx		PORTC	// Port Ultraschallmodul
#define UM_PORT_DDRx	DDRC	// Datenrichtungsregister Port Ultraschallmodul
#define UM_PORT_PINx	PINC	// Pin Port Ultraschallmodul
#define UM_TRIG			PINC6	// Trigger Pin Ultraschallmodul
#define UM_ECHO			PINC7	// Echo Pin Ultraschallmodul


#include <avr/io.h>
#include <stdbool.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Prototypen
void init_timer1(void);
void init_distance(void);
float distance(uint8_t amax);