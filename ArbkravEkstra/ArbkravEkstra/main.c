/*
 * ArbkravEkstra.c
 *
 * Created: 22/04/2023 14:36:52
 * Author : NTNU
 */ 
#define F_CPU 4000000UL
#define PERIOD_VALUE (0x01a0)
#define DUTY_CYCLE (0x00D0)

#include "omsfah_usart.h"
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void ADC0_init(void);
uint16_t ADC0_read(void);

volatile bool state=1; //Volatile due to being modified by an interrupt
uint8_t dutyCycle = 0;
uint8_t dutyCycleStep = 1;
uint16_t adcVal;
uint16_t servoVal;

ISR(PORTB_PORT_vect){
	_delay_ms(10); //debounce 
	state = !state;
	if (state)
	{
		printf("Systemet er i LED modus\r\n");
	} 
	else
	{
		printf("Systemet er i servo modus\r\n");
	}
	PORTB.INTFLAGS = 0xff;
}

void TCA0_init(void)
{
	PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTB_gc;
	TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;
	//TCA.SPLIT.CTRLB = 
	// enable W00 and W03
	TCA0.SPLIT.CTRLB = TCA_SPLIT_HCMP0EN_bm | TCA_SPLIT_LCMP0EN_bm;
	TCA0.SPLIT.HPER = DUTY_CYCLE;
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV4_gc | TCA_SPLIT_ENABLE_bm;
}

void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	ADC0.CTRLC = ADC_PRESC_DIV4_gc; /* CLK_PER divided by 4 */
	VREF.ADC0REF = VREF_REFSEL_VDD_gc; /* Internal reference */
	ADC0.CTRLA = ADC_ENABLE_bm /* ADC Enable: enabled */
	| ADC_RESSEL_10BIT_gc; /* 10-bit mode */
	/* Select ADC channel */
	ADC0.MUXPOS = ADC_MUXPOS_AIN6_gc;
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

int main(void)
{
	//PORTB.DIR |= PIN0_bm;
	PORTB.DIR |= PIN3_bm;
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    sei();
    USART3_init();
	TCA0_init();
	ADC0_init();
    _delay_ms(10);
	stdout = &USART_stream;
	printf("Heiheihei\r\n");
    while (1) 
    {
		if (state)
		{
			TCA0.SPLIT.HCMP0=dutyCycle; 
			_delay_ms(5);
			dutyCycle = adcVal/4;
		} 
		else
		{
			
			TCA0.SPLIT.LCMP0=servoVal; //For some reason theres interference between ports
			_delay_ms(5);
			servoVal=adcVal+750;		
		}

	}
}