#ifndef USART_H_
#define USART_H_

#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE))+0.5)
#include <avr/io.h>
#include <stdio.h>
#include <string.h>

void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char* str);
char USART3_readChar(void);
uint8_t USART3_printChar(char c, FILE* stream);

extern FILE USART_stream;
#endif