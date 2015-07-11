#include <htc.h>
#define _XTAL_FREQ 4000000

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & HS);

void main() {
	//устанавливаем 3-ый вывод порта C на вход
	TRISC = 0b00001000;

	//предделитель (см. формулу)
	PR2 = 0b01111100;
	CCPR1L = 0;

	//режим работы модуля CCP1 - режим ШИМ
	CCP1M3 = 1;
	CCP1M2 = 1;

	//коэффициент деления предделителя таймера TMR2
	T2CKPS0 = 1; 
	T2CKPS1 = 0;

	//включаем таймер
	TMR2ON = 1;

	//разрешаем прерывания
	GIE = 1;
	INTE = 1;

	//задаем скважность
	CCPR1L = 127;
}
