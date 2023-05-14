/*
 * rtc.c
 *
 * Created: 11.08.2021 10:17:51
 *  Author: Ingolf Brunner
 */ 
#include "rtc.h"

void init_rtc(void)	// Initialisierung Timer/Counter 2 mit Uhrenquarz
{
	// A safe procedure for switching clock source is:
	// 1. Disable the Timer/Counter2 interrupts by clearing OCIE2 and TOIE2.
	TIMSK &= ~((1<<OCIE2) | (1<<TOIE2));
	// 2. Select clock source by setting AS2 as appropriate.
	ASSR  |=  (1<<AS2);
	// 3. Write new values to TCNT2, OCR2, and TCCR2.
	TCCR2 |= (1<<WGM21);				// Mode 2: CTC
	TCCR2 |= ((1<<CS22) | (1<<CS21));	// clkT2S/256 (From prescaler)
	TCNT2 = 0x00;
	OCR2 = 128-1;						// 2^15 / 2^8 / 2^7 = 1 Hz
	// 4. To switch to asynchronous operation: Wait for TCN2UB, OCR2UB, and TCR2UB.
	while(ASSR & ((1<<TCN2UB) | (1<<OCR2UB) | (1<<TCR2UB)));
	// 5. Clear the Timer/Counter2 Interrupt Flags.
	TIFR &= ~((1<<OCF2) | (1<<TOV2));
	// 6. Enable interrupts, if needed.
	TIMSK |= (1<<OCIE2);				// output compare interrupt
}

ISR(TIMER2_COMP_vect)	// Timer 2 output compare interrupt
{
	TCCR2 = TCCR2;      // Timer läuft nicht mit CPU-Takt, sondern
						// mit 32768 Hz! Schreibvorgang auf
						// ein Timer-Register --> am Schluss
						// im ASSR-Register nachsehen, ob dieser
						// fertig ist (siehe while am Schluss)
	sekunde++;
	if(sekunde == 60)
	{
		minute++;
		sekunde = 0;
	}
	if(minute == 60)
	{
		stunde++;
		minute = 0;
	}
	if(stunde == 24)
	{
		stunde = 0;
	}
	while(ASSR & ((1<<TCN2UB) | (1<<OCR2UB) | (1<<TCR2UB)));  // siehe erster Befehl in der Interrupt-Routine
}