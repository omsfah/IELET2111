#define F_CPU 4000000UL

/* RTC Period */
#define RTC_PERIOD (511)
#include <avr/io.h>
#include <avr/interrupt.h>
uint16_t adcVal;
void ADC0_init(void);
uint16_t ADC0_read(void);


void ADC0_init(void)
{
	/* Disable digital input buffer */
	PORTD.PIN6CTRL &= ~PORT_ISC_gm;
	PORTD.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	/* Disable pull-up resistor */
	PORTD.PIN6CTRL &= ~PORT_PULLUPEN_bm;
	ADC0.CTRLC = ADC_PRESC_DIV4_gc /* CLK_PER divided by 4 */
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
	ADC0_init();
	adcVal = ADC0_read();
	while (1)
	{
		;
	}
}
