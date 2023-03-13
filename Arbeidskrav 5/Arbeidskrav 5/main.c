/*
 * Arbeidskrav 5.c
 *
 * Created: 2/27/2023 8:25:14 AM
 * Author : Windows-NTNU
 */ 

#include "omsfah_usart.h"
#include <util/delay.h>
#include <avr/interrupt.h>
volatile int trykk = 0;

ISR(PORTB_PORT_vect){
	trykk += 1;
	printf("Knappen er trykket, verdien er lik: %d\r\n",trykk);
	PORTB.INTFLAGS = PIN2_bm;
}

int main(void)
{
	PORTB.DIR = PIN3_bm;
	PORTB.OUT = ~PIN3_bm;
	PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	sei();
	USART3_init();
	_delay_ms(10);
	stdout = &USART_stream;
    while (1) 
    {
	_delay_ms(1000);
	PORTB.OUT ^=8;
    }
}

