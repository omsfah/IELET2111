/*
 * Arbeidskrav 5.c
 *
 * Created: 2/27/2023 8:25:14 AM
 * Author : Windows-NTNU
 */ 

#include <util/delay.h>
#include "omsfah_usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(PORTB_PORT_vect)
{
	
}

int main(void)
{
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	sei();
    while (1) 
    {
		;
    }
}

