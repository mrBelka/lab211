#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

// максимальное количество принятых символов
#define MAX_STRLEN 12
// массив принятых символов
volatile char received_string[MAX_STRLEN+1];

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

// функция инициализации UART1
void init_USART1(uint32_t baudrate){
	// структура для инициализации пинов,
	// используемых в качестве TxD, RxD
	GPIO_InitTypeDef GPIO_InitStruct;
	// структура инициализации USART
	USART_InitTypeDef USART_InitStruct;
	// структура инициализации прерываний
	NVIC_InitTypeDef NVIC_InitStructure;

	// включаем тактирование для модуля USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// включаем тактирование порта B
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// настройка пинов
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	// задаем для них режим альтернативной функции
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// привязываем пины к модулю USART1
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	// инициализация модуля USART1
	// задаем скорость обмена
	USART_InitStruct.USART_BaudRate = baudrate;
	// кол-во битов данных
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	// кол-во стоп-битов
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	// четность
	USART_InitStruct.USART_Parity = USART_Parity_No;
	// аппаратное управление потоком данных
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// задаем, какую именно функцию выполняют пины
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);

	// включаем прерывание по поступлению данных
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// инициализация прерывания
	// прерывание конфигурируется для USART1
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	// устанавливаем приоритетную группу для прерываний USART1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// задаем подприоритет внутри группы
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// разрешаем глобальные прерывания
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// включаем USART1
	USART_Cmd(USART1, ENABLE);
}

// функция отправки данных
void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// ждем, пока предыдущие данные не отправятся
		while( !(USARTx->SR & 0x00000040) );
		// отправляем следующие
		USART_SendData(USARTx, *s);
		*s++;
	}
}

int main(void) {
  // инициализация USART1
  init_USART1(9600);

  while (1){
  }
}

// обработчик прерываний для всех прерываний связанных с UART1
void USART1_IRQHandler(void){
	// если установлен флаг прерывания по поступлению данных
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

		static uint8_t cnt = 0;
		char t = USART1->DR;

		if((t != 'n') && (cnt < MAX_STRLEN)){
			received_string[cnt] = t;
			cnt++;
		}
		else{
			cnt = 0;
			USART_puts(USART1, received_string);
		}
	}
}
