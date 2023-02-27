#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#define MAX_COMMAND_LEN 9
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
char USART3_readChar(void);
void LED_on(void);
void LED_off(void);
void LED_init(void);
void executeCommand(char *command);

void USART3_init(void)
{
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600); //Set baud rate til uart komm

	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm; //Setter rx til inut og Tx til output
	
	PORTB.DIRSET = PIN0_bm; //Velger pins
	PORTB.DIRSET = PIN1_bm; //velger pins

} 

void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm)) //Sjekker at UART er paa og klar
	{
		;
	}
	USART3.TXDATAL = c; //Sender data
}

void USART3_sendString(char *str)
{
	for(size_t i = 0; i < strlen(str); i++) //Converterer string til char for aa kunne sende det med uart
	{
		USART3_sendChar(str[i]);
	}
}

char USART3_readChar(void)
{
	while (!(USART3.STATUS & USART_RXCIF_bm)) //Sjekker at UART er paa og klar
	{
		;
	}
	return USART3.RXDATAL;
}

void LED_init(void) //Slaar av led ved boot
{
	PORTE.DIRSET = PIN0_bm;
	PORTE.DIRSET = PIN1_bm;
	PORTE.OUTSET = PIN0_bm & PIN1_bm;
}

void executeCommand(char *command)
{
	enum Command {led_1_on, led_2_on, led_1_off, led_2_off};
	enum Command ledstate;
	switch(ledstate)
	{
			case led_1_on:
			PORTE.OUTSET=PIN0_bm;
			USART3_sendString("OK, LED 1 ON.\r\n");
			break;
			
			case led_2_on:
			PORTE.OUTSET=PIN1_bm;
			USART3_sendString("OK, LED 2 ON.\r\n");
			break;
			
			case led_1_off:
			PORTE.OUTCLR=PIN0_bm;
			USART3_sendString("OK, LED 1 OFF.\r\n");
			break;
			
			case led_2_off:
			PORTE.OUTCLR=PIN0_bm;
			USART3_sendString("OK, LED 2 OFF.\r\n");
			break;
			
			default:
			USART3_sendString("Unknown command.\r\n");
	}
	
}

int main(void)
{
	char command[MAX_COMMAND_LEN];
	uint8_t index = 0;
	char c;
	
	/* This delay invalidates the initial noise on the TX line, after device reset. */
	_delay_ms(10);
	
	USART3_init();
	LED_init();
	
	USART3_sendString("Type ON/OFF to control the LED.\r\n");
	
	while (1)
	{
		c = USART3_readChar();
		if(c != '\n' && c != '\r')
		{
			command[index++] = c;
			if(index > MAX_COMMAND_LEN)
			{
				index = 0;
			}
		}
		
		if(c == '\r')
		{
			command[index] = '\0';
			index = 0;
			executeCommand(command);
		}
	}
}