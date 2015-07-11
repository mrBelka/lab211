#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void) {
	//конфигурация портов
	PORTE.DIRSET = 0b01100000; //на вход
	PORTE.PIN5CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //подтяжка на 5 вывод и работа по ниспадающему фронту
	PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc; //то же самое для 6-ого вывода
	//сопоставляем прерывания и выводы
	PORTE.INT0MASK = 0b00100000;
	PORTE.INT1MASK = 0b01000000;
	//задаем приоритет прерываний
	PORTE.INTCTRL = PORT_INT0LVL_HI_gc | PORT_INT1LVL_LO_gc;
	//разрешаем прерывания высокого и низкого приоритетов
	PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_LOLVLEN_bm;
	//глобальное разрешение прерывание
	sei();

	while(1) {
		PORTE.OUTTGL = 0b00000001;
		_delay_ms(500);
	}
}

//функция обработки первого прерывания
ISR(PORTE_INT0_vect) {
	for(uint8_t i=0; i<20; i++) {
		PORTE.OUTTGL = 0b00010000;
		_delay_ms(100);
	}
}

//функция обработки второго прерывания
ISR(PORTE_INT1_vect) {
	for(uint8_t i=0; i<20; i++) {
		PORTE.OUTTGL = 0b0001000;
		_delay_ms(100);
	}
}
