#include <stm32f4xx.h>
#include <misc.h>
#include <stm32f4xx_usart.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

// максимальное количество принятых символов
#define MAX_STRLEN 12
// массив принятых символов
volatile char received_string[MAX_STRLEN+1];

int cnt = 0;

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

// функция инициализации USART1 и USART6
void init_USART1_6(uint32_t baudrate1, uint32_t baudrate6){
	// структура для инициализации пинов,
	// используемых в качестве TxD, RxD
	GPIO_InitTypeDef GPIO_InitStruct;
	// структура инициализации USART
	USART_InitTypeDef USART_InitStruct;
	// структура инициализации прерываний
	NVIC_InitTypeDef NVIC_InitStructure;

/*тактирование*/
	// включаем тактирование для модуля USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// включаем тактирование для модуля USART6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

/*настройка пинов*/
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

	// включаем тактирование порта C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// настройка пинов
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	// задаем для них режим альтернативной функции
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// привязываем пины к модулю USART6
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);


/*инициализация USARTов */
	// инициализация модуля USART1
	// задаем скорость обмена
	USART_InitStruct.USART_BaudRate = baudrate1;
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

	// инициализация модуля USART6
	// задаем скорость обмена
	USART_InitStruct.USART_BaudRate = baudrate6;
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
	USART_Init(USART6, &USART_InitStruct);

/*включаем прерывания*/
	// включаем прерывание по поступлению данных
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// включаем прерывание по поступлению данных
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

/*инициализация прерываний*/
	// прерывание конфигурируется для USART1
	NVIC_InitStructure.NVIC_IRQChannel =  USART1_IRQn;
	// устанавливаем приоритетную группу для прерываний USART1 и 6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// задаем подприоритет внутри группы
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// разрешаем глобальные прерывания
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// прерывание конфигурируется для USART6
	NVIC_InitStructure.NVIC_IRQChannel =  USART6_IRQn;
	// устанавливаем приоритетную группу для прерываний USART1 и 6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// задаем подприоритет внутри группы
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// разрешаем глобальные прерывания
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

/*включение модулей*/
	// включаем USART1
	USART_Cmd(USART1, ENABLE);
	// включаем USART6
	USART_Cmd(USART6, ENABLE);
}

int main(void) {
  // инициализация USART1
  init_USART1_6(9600,19200);

  while (1){
  }
}

// обработчик прерываний для всех прерываний связанных с UART1
void USART1_IRQHandler(void){
	// если установлен флаг прерывания по поступлению данных
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

		char t = USART1->DR;
		
		//отправляем запрос на получение веса НЕТТО
		USART_SendData(USART6, 0xFF);
		Delay(1000);
		USART_SendData(USART6, 0x01);
		Delay(1000);
		USART_SendData(USART6, 0xC2);
		Delay(1000);
		USART_SendData(USART6, 0x8A);
		Delay(1000);
		USART_SendData(USART6, 0xFF);
		Delay(1000);
		USART_SendData(USART6, 0xFF);
	}
}

// обработчик прерываний для всех прерываний связанных с UART6
void USART6_IRQHandler(void){
	// если установлен флаг прерывания по поступлению данных
	if( USART_GetITStatus(USART6, USART_IT_RXNE) ){

		char t = USART6->DR;

		if(t==0xFF){

				int i = 0;
				while(i<cnt){
					USART_SendData(USART1, received_string[i]);
					Delay(10000);
					i++;
				}
				cnt = 0;


		}else{
			received_string[cnt] = t;
			cnt++;
		}
	}
}
