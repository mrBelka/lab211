#include <avr/interrupt.h>
#define F_CPU 2000000
#include <util/delay.h>
#define MYBSEL 12

volatile uint16_t receivedData;
uint16_t mas[8];

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

int pos[256];

int main(void) {
	USART_Init(MYBSEL);
	PORTC.DIRSET = PIN3_bm; //TX, инициализирован как выход
	PORTC.DIRCLR = PIN2_bm; //RX, инициализирован как вход
	
	PORTH.DIR = 0b00000000;
	
	for(int i=0;i<256;i++)pos[i]=0;
	
	
	//подогнать значения
	pos[114]=0;
	pos[82]=1;
	pos[80]=2;
	pos[16]=3;
	pos[48]=4;
	pos[50]=5;
	pos[54]=6;
	pos[55]=7;
	pos[39]=8;
	
	PORTE.DIRSET = 0b00000001;

	TCE0.CTRLB = TC_WGMODE_SS_gc | TC0_CCAEN_bm;
	TCE0.PER = 39999;
	TCE0.CTRLA = TC_CLKSEL_DIV8_gc;
	
	int i,j=90;
	while(1) {
		//USART_Receive();
		receivedData = PORTH.IN;
		receivedData = pos[receivedData];
		
		TCE0.CCA = 90+receivedData*50;
		_delay_ms(100);
		
		i = -1;
		while(receivedData>0){
			i++;
			mas[i] = receivedData%10;
			receivedData/=10;
		}
		
		/*j++;
		if (j>620){		//границы 90-620
			j = 90;
			TCE0.CCA = j;
			_delay_ms(50);
		}
		TCE0.CCA = j;
		_delay_ms(2);*/

		

		for(;i>=0;i--){
			USART_Transmit(mas[i]+48);
			_delay_ms(10);
		}
		_delay_ms(100);
	}
	return 0;
}
