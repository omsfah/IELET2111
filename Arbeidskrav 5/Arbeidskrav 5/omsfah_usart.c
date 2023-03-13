/*
 * omsfah_usart.c
 *
 * Created: 2/27/2023 9:30:41 AM
 *  Author: Windows-NTNU
 */ 

#include "omsfah_usart.h"
void USART3_init(void) 
{
	PORTB.DIR &= ~PIN1_bm;
	PORTB.DIR |= PIN0_bm;
	USART3_BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}
void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}
void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str);i++)
	{
		USART3_sendChar(str[i]);
	}
}
char USART3_readChar(void)
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
	{
		;
	}
	return USART3.RXDATAL;
}
uint8_t USART3_printChar(char c, FILE* stream)
{
	USART3_sendChar(c);
	return 0;
}
FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
