//константа F_CPU задает частоту тактирования
#define F_CPU 2000000
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	//устанавливаем направление работы выводов порта E
	PORTE.DIR = 0b11011111;
	//высокий логический уровень на первые 5 выводов
	PORTE.OUT = 0b00011111;
	int dir = 0;
	while(1) {
		//на выводе 5 установлен высокий логический уровень
		if(PORTE.IN & 0b00100000){
			//защита от дребезга контактов
			while(PORTE.IN & 0b00100000);
			_delay_ms(100);
			dir = 1 - dir;
			//переключение светодиода
			if(dir)PORTE.OUT = 0b00011111;
			else PORTE.OUT = 0b00000000;
		}
	}
}
