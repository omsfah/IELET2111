/*
 * Arbeidskrav 5.c
 *
 * Created: 2/27/2023 8:25:14 AM
 * Author : Windows-NTNU
 */ 

#include "omsfah_usart.h"
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	USART3_init();
	_delay_ms(10);
	stdout = &USART_stream;
    while (1) 
    {
	printf("test\r\n");
	_delay_ms(100);
		
    }
}

