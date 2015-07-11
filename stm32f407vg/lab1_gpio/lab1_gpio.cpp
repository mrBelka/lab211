#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

// функция задежки
void Delay(__IO uint32_t nCount){
	while(nCount--){
	}
}

int main(void){
// инициализация необходимых выводов
	/* GPIO_InitTypeDef - структура, определенная в стандартной библиотеке
	 * и содержащая все настройки, которые имеют порты ввода-вывода.
	 * Подобная структура есть у всех периферийных модулей.
	 * К счастью, их инициализация очень похожа.
	 * Все настройки можно найти в заголовочном файле stm32f4xx_gpio.h и
	 * файле исходного кода stm32f4xx_gpio.c
	 */
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Здесь включается тактирование для используемого нами порта D.
	 * Тактирование должно быть включено для любого периферийного модуля
	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/*В этом участке кода задаются все настройки,
	 * затем структура передается в функцию Init(),
	 * которая устанавливает нужные биты в регистре,
	 * отвечающим за используемый модуль.
	 * Светодиоды подключены к пинам 12-15 порта D.
	 */
	//указываем, какие пины хотим инициализировать
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	//конфигурируем пины на выход
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// двухтактный выход
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	// не притягиваем выход ни к питанию, ни к земле
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	// включаем тактирование для порта A
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// инициализация порта A
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	// конфигурируем пины на вход
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	// вход с подтяжкой к земле
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	while (1){

		/* У каждого порта ввода-вывода есть
		 * два регистра данных, один (IDR) хранит информацию
		 * о состояниях входа каждого пина, второй (ODR) - о
		 * состояних выхода
		 */

		// если бит в регистре IDR порта А, соответствующий пину 0,
		// установлен, т.е. кнопка нажата
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){
			// проверяем, установлен ли бит в регистре ODR порта D для
			// пина 12, т.е. горит ли светодиод
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12))
				// если бит установлен, сбрасываем его
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			else
				// иначе устанавливаем
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
			// небольшая задержка для предотвращения дребезга контактов
			Delay(3000L);
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
			Delay(3000L);
		}
	}
}
