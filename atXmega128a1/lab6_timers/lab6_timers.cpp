#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
	//устанавливаем 0-ой вывод порта Е на вывод
	PORTE.DIRSET = 0b00000001;
	//преррывание генерируется по переполнению счетчика
	//и обладает низким приоритетом
	TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	//разрешаем прерывания с низким приоритетом
	PMIC.CTRL = PMIC_LOLVLEN_bm;
	//разрешаем глобальные прерывания
	sei();

	//обычный режим
	TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
	//делитель частоты
	TCC0.CTRLA = TC_CLKSEL_DIV2_gc;

	while(1) {
	}
}

//функция обработки прерывания
ISR(TCC0_OVF_vect) {
	PORTE.OUTTGL = 0b00000001;
}
