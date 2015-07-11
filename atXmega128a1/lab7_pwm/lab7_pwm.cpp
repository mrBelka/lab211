#define  F_CPU    16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
	PORTE.DIRSET = 0b00000001;
	
	TCE0.CTRLB = TC_WGMODE_SS_gc | TC0_CCAEN_bm;
	TCE0.PER = 39999;
	TCE0.CTRLA = TC_CLKSEL_DIV8_gc;
	
	int i=90;
	while(1) {
		i++;
		if(i>620){		//границы 90-620
			i=90;
			TCE0.CCA = i;
			_delay_ms(50);
		}		
		TCE0.CCA = i;
		_delay_ms(2);
	}
}
