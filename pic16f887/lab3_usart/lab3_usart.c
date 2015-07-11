#include <htc.h>

#define _XTAL_FREQ 4000000

//Set the time-out for serial data read in seconds
#define TIMEOUT         10

//Defined size for incoming buffer
#define BUF_SIZE        64

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & INTCLK);

unsigned char read_buffer[BUF_SIZE];
int rx_buffer_save_pointer = 0;   // This is a circular buffer save counter.
int rx_buffer_read_pointer = 0;   // This is a circular buffer read counter.
int rx_buffer_available = 0;		//The available bytes in the buffer. Countable backwards from rx_buffer_save_pointer

//инициализация USART
void Serial_begin(unsigned long baud)
{
	//    TXSTA: TRANSMIT STATUS AND CONTROL REGISTER
	//    *********************************************
	//    TX9: формат передачи
	//1 = 9 бит
	//0 = 8 бит
	TX9 = 0;
	//    TXEN: флаг разрешения передачи
	//1 = разрешена
	//0 = запрещена
	TXEN = 1;
	//    SYNC: режим работы
	//1 = синхронный
	//0 = асинхронный
	SYNC = 0;
	//BRGH: скорость передачи
	//для асинхронного режима:
	//1 = высокая
	//0 = низкая
	BRGH = 0;

	//RCSTA: RECEIVE STATUS AND CONTROL REGISTER
	//    *********************************************
	//    SPEN: включение порта
	//1 = порт включен (RX и TX сконфигурированы)
	//0 = порт отключен
	SPEN = 1;
	//    RX9: формат приема
	//1 = 9 бит
	//0 = 8 бит
	RX9 = 0;
	//    CREN: флаг включения приемника
	//асинхронный режим:
	//1 = включен
	//0 = отключен
	CREN = 1;

	//    BAUDCTL: BAUD RATE CONTROL REGISTER
	//    *********************************************
	//SCKP: Synchronous Clock Polarity Select bit
	//Asynchronous mode:
	//1 = Transmit inverted data to the RB7/TX/CK pin
	//0 = Transmit non-inverted data to the RB7/TX/CK pin
	SCKP = 0;
	//    BRG16: 16-bit Baud Rate Generator bit
	//1 = 16-bit Baud Rate Generator is used
	//0 = 8-bit Baud Rate Generator is used
	BRG16 = 1;
	//    ABDEN: автоопределение скорости приема-передачи
	//Asynchronous mode:
	//1 = включено
	//0 = отключено
	ABDEN = 0;

	//    The divider value is depends on SYNC, BRGH and BRG16. That can be 64,16 or 4
	unsigned int divider = 16;
	unsigned int BaudVal = (_XTAL_FREQ / baud / divider) - 1;
	SPBRGH = (BaudVal & 0xff00) >> 8;
	SPBRG = BaudVal & 0x00ff;

	//разрешаем прерывания и обнуляем флаги приема/передачи
	TXIF = 0;
	RCIF = 0;
	PEIE = 1;
	TXIE = 0;
	RCIE = 1;
	GIE = 1;

}

// Reads incoming serial data. Returns the first byte of incoming serial data available.
// Supports byte only.
unsigned char Serial_read(void)
{
	if (rx_buffer_available)
	{
		if (rx_buffer_read_pointer == BUF_SIZE)
			rx_buffer_read_pointer = 0;
		rx_buffer_available--;
		return read_buffer[rx_buffer_read_pointer++];
	}
	else
		return 0;   // No data available
}

//функция передачи одного байта
void Serial_write(unsigned char x)
{
	//ждем, пока буфер передатчика занят
	while (!TRMT);
	//передаем байт
	TXREG = x;
}

//функция передачи строки
void Serial_print(unsigned char *str)
{
	while (*str)
	{
		while (!TRMT);
		TXREG = *str++;
	}
}

unsigned char Serial_available(void)
{
	return rx_buffer_available;
}

//Waits for the transmission of outgoing serial data to complete.
void Serial_flush(void)
{
	rx_buffer_save_pointer = 0;   // This is a circular buffer save counter.
	rx_buffer_read_pointer = 0;   // This is a circular buffer read counter.
	rx_buffer_available = 0; //The available bytes in the buffer. Countable backwards from rx_buffer_save_pointer
}

unsigned char inByte = 0;      

void main(void)
{
	//начинаем передачу со скоростью 9600 бод
	Serial_begin(9600);

	while (1)
	{
		//если данные поступили
		if (Serial_available() > 0) {
			//читаем
			inByte = Serial_read();
			//отправляем
			Serial_write(inByte);
		}
	}
}
