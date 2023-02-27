#define F_CPU 4000000UL
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
//* RTC Period */
#define RTC_PERIOD (511)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
uint16_t adcVal;
void ADC0_init(void);
uint16_t ADC0_read(void);
void USART3_init(void);
void USART3_sendChar(char c);
void USART3_sendString(char *str);
void USART3_init(void);
float flyttall=0;
uint16_t count=0;


void USART3_init(void)
{
	PORTB.DIR &= ~PIN1_bm;
	PORTB.DIR |= PIN0_bm;

	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	USART3.CTRLB |= USART_TXEN_bm;
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
	for(size_t i = 0; i < strlen(str); i++)
	{
		USART3_sendChar(str[i]);
	}
}

static int USART3_printChar(char c, FILE *stream)
{
	USART3_sendChar(c);
	return 0;
}
static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);



void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	ADC0.CTRLC = ADC_PRESC_DIV4_gc; /* CLK_PER divided by 4 */
	VREF.ADC0REF = VREF_REFSEL_VDD_gc;
	ADC0.CTRLA = ADC_ENABLE_bm /* ADC Enable: enabled */
	| ADC_RESSEL_10BIT_gc; /* 10-bit mode */
	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
	ADC0.CTRLA |= ADC_FREERUN_bm;

}
uint16_t ADC0_read(void)
{
	/* Start ADC conversion */
	ADC0.COMMAND = ADC_STCONV_bm;
	/* Wait until ADC conversion done */
	while ( !(ADC0.INTFLAGS & ADC_RESRDY_bm) )
	{
		;
	}
	/* Clear the interrupt flag by writing 1: */
	ADC0.INTFLAGS = ADC_RESRDY_bm;
	return ADC0.RES;
}

void LED_init(void)
{
	PORTE.DIRSET = PIN0_bm;
	PORTE.DIRSET = PIN1_bm;
	PORTE.DIRSET = PIN2_bm;
	PORTE.OUTSET = PIN0_bm & PIN1_bm & PIN2_bm;
}


int main(void)
{
	USART3_init();
	stdout = &USART_stream;
	PORTB_DIRCLR = PIN3_bm;
	PORTB.PIN2CTRL |= PORT_PULLUPEN_bm;
	ADC0_init();
	adcVal = ADC0_read();
	while (1)
	{
		if(ADC0.INTFLAGS & ADC_RESRDY_bm){
			if (adcVal < 600)
			{
				PORTE.OUTSET=0b00000001;
			}
			else if (adcVal < 300)
			{
				PORTE.OUTSET=0b00000010;
			}
			else
			{
				PORTE.OUTSET=0b00000100;
			}
			count++;
		}
		else{
			;
		}
		if (count>=100)
		{
			flyttall = adcVal / 2; //Her må det utføres noe mattematikk for å få korrekte tall, for nå deler vi bare på 2
			_delay_ms(50);
			printf("Temperaturen er %d\r\n", adcVal);
			_delay_ms(50);
			count=0;
		}
	}
}
