/*
 * ArbKrav1.c
 *
 * Created: 1/24/2023 2:17:42 PM
 * Author : Windows-NTNU
 */ 

#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	PORTB.DIRSET = PIN3_bm; //Aktivere led på kortet
	PORTE.DIRSET = 0b00000111;
    PORTB.DIRCLR = PIN2_bm; //Aktivere bryteren på kortet, hentet fra https://github.com/microchip-pic-avr-examples/avr128da48-led-toggle-example
    PORTB.PIN2CTRL |= PORT_PULLUPEN_bm; //https://onlinedocs.microchip.com/pr/GUID-40586B6A-B51F-492B-9B73-863AE7F489CD-en-US-3/index.html?GUID-5D1C4893-6086-49FE-9460-88DDA7B21445 Det er ikke pullup på kortet før det enables etter manual.
    
    
    while (1)
    {
		for (int i; i<3 ; i++)
		{
			PORTE.OUT= (1<<i); //Bitshift for å spare linjer
			_delay_ms(1000);
		}
    }

}