/*
 * GccApplication1.c
 *
 * Created: 08.05.2023 10:50:19
 * Author : sfric
 */ 

#define F_CPU 3686400
#define __DELAY_BACKWARD_COMPATIBLE__ //can use Variables for _delay_**()
#include <util/delay.h>
#include <avr/io.h>


	/*
	D0 = Enable
	D1 = RS
	
	C = Display in 8 bit mode
	*/

//EasyAccess for RS and E

void enable(){
	PORTD |= (0b00000001);
	PORTB = (0x00);
	_delay_us(1);
}

void disable(uint8_t sleep){
	
	_delay_us(sleep);
	PORTD &= ~(0b00000001);
	_delay_us(1);
	PORTC = 0x00;
	PORTB = 0xff;
}

void rsON(){
	PORTD |= 0b00000010;
}


void rsOFF(){
	PORTD &= ~(0b00000010);
}







void clear(){

	enable();
	PORTC = 0b00000001;
	disable(1520);
	
	
}

void setFunctionSet(){
	PORTC = 0b0000110100;
	enable(37);
}


int main(void)
{
	
	DDRD = 0xff;
	DDRC = 0xff;
	DDRB = 0xff;
	
	rsOFF();
	clear();
	setFunctionSet();
	
	
	rsON();
	//Try a hello world
	enable();
	PORTC = 0b01001000;
	disable(37);
	
	PORTC = 0b01100101;
	disable(37);
	rsOFF();
	

    /* Replace with your application code */
    while (1) 
    {
    }
}

