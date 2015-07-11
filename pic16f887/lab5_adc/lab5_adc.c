#include <htc.h>
#define _XTAL_FREQ 4000000
__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & HS);

void main()
{
	do{
		ADCON1 = 0x00;       //adc initonce
		
		//устанавливаем режим альтернативной функции на входах 2 и 3 порта А
		ANSEL = 0x0C;        
	
		//конфигурируем порт D на выход
		TRISD = 0x00;        
		//порт А на вход
		TRISA = 0x0C;   

		ADCON0 = 0xCB;       //begin AD conversion on AN2
		while (ADCON0 == 1)//wait until ADCON0<1> flips to 0
			continue;

		//выводим результат
		PORTD = ADRESH;      
	} while (1);
}
