#include <avr/interrupt.h>
#define F_CPU 2000000
#define MYBSEL 12

volatile uint16_t receivedData;

void USART_Receive(void)
{
	//передаем 1 символ
	//ждем, пока данные передадутся или выйдет тайм-аут
	while (!(USARTC0.STATUS & USART_RXCIF_bm));
	receivedData = USARTC0.DATA;
}

void USART_Transmit(unsigned char data)
{
	//ждем, пока очистится буфер передатчика
	while (!(USARTC0.STATUS & USART_DREIF_bm));
	//кладем инфу в регистр для отправки
	USARTC0.DATA = data;
}

void USART_Init(unsigned int bsel) {
	int bscale = 0;
	//устанавливаем скорость передачи
	USARTC0.BAUDCTRLA = (unsigned char)(bsel & 0xFF);
	USARTC0.BAUDCTRLB = ((unsigned char)(bscale & 0x0F)<< 4) | ((unsigned char)(bsel >> 8) & 0x0F);
	//устанавливаем формат передачи: 8 бит данных и 1 стоп-бит
	USARTC0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc | USART_SBMODE_bm;
	//включаем приемник и передатчик
	USARTC0.CTRLB = (USART_RXEN_bm | USART_TXEN_bm);
}

int main(void) {
	USART_Init(MYBSEL);
	PORTC.DIRSET = PIN3_bm; //TX, инициализирован как выход
	PORTC.DIRCLR = PIN2_bm; //RX, инициализирован как вход
	while(1) {
		USART_Receive();
		USART_Transmit(receivedData);
	}
	return 0;
}
