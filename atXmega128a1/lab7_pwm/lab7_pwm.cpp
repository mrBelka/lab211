#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
	PORTE.DIRSET = 0b00001111;
	//включаем 4 канала ШИМ
	TCE0.CTRLB = TC_WGMODE_DS_B_gc | TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm;
	//устанавливаем период
	TCE0.PER = 1000;
	//частота не делится
	TCE0.CTRLA = TC_CLKSEL_DIV1_gc;
	while(1) {	
		for(int i=0;i<300;i++){
			TCE0.CCA = i;
			TCE0.CCB = i+200;
			TCE0.CCC = i+400;
			TCE0.CCD = i+600;
			_delay_ms(1);
		}
	}
}
