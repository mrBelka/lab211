#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 2000000

#define MYBSEL 12
volatile uint16_t receivedData;

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

void ADCA_init()
{
	ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
	ADCA.REFCTRL = ADC_REFSEL_VCC_gc;
	ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
	ADCA.CTRLA = ADC_ENABLE_bm;
}

unsigned int ADCA_Conversion(ADC_CH_t *Channel, char Pin)
{
	Channel->CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	Channel->MUXCTRL = (Pin << 3);
	Channel->CTRL |= ADC_CH_START_bm;
	
	while(!Channel->INTFLAGS);
	return Channel->RES;
}

unsigned int ADC_Value;

int main(void) {
	USART_Init(MYBSEL);
	PORTC.DIRSET = PIN3_bm;	//TX
	PORTC.DIRCLR = PIN2_bm;	//RX
	
	ADCA_init();
	
	while(1) {
		ADC_Value = ADCA_Conversion(&(ADCA.CH0),1);
		USART_Transmit((char)(ADC_Value/10000%10 + '0'));
		USART_Transmit((char)(ADC_Value/1000%10 + '0'));
		USART_Transmit((char)(ADC_Value/100%10 + '0'));
		USART_Transmit((char)(ADC_Value/10%10 + '0'));
		USART_Transmit((char)(ADC_Value%10 + '0'));
		USART_Transmit((char)' ');
		_delay_ms(1000);
		
		ADC_Value = ADCA_Conversion(&(ADCA.CH1),2);
		USART_Transmit((char)(ADC_Value/10000%10 + '0'));
		USART_Transmit((char)(ADC_Value/1000%10 + '0'));
		USART_Transmit((char)(ADC_Value/100%10 + '0'));
		USART_Transmit((char)(ADC_Value/10%10 + '0'));
		USART_Transmit((char)(ADC_Value%10 + '0'));
		USART_Transmit((char)' ');
		_delay_ms(1000);
		
		ADC_Value = ADCA_Conversion(&(ADCA.CH3),3);
		USART_Transmit((char)(ADC_Value/10000%10 + '0'));
		USART_Transmit((char)(ADC_Value/1000%10 + '0'));
		USART_Transmit((char)(ADC_Value/100%10 + '0'));
		USART_Transmit((char)(ADC_Value/10%10 + '0'));
		USART_Transmit((char)(ADC_Value%10 + '0'));
		USART_Transmit((char)'|');
		_delay_ms(5000);
	}
	return 0;
}
