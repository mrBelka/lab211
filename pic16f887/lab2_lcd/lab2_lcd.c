//тактовая частота
#define _XTAL_FREQ 4000000

#include    <htc.h>

//определяем выводы
#define    LCD_RS RA3
#define    LCD_RW RA4
#define    LCD_EN RA5

//линия передачи данных
#define LCD_DATA PORTB

//для обновления LCD, команды выполняются последовательно
#define LCD_STROBE() ((LCD_EN = 1),(LCD_EN = 0))

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & HS);

//функция задержки
void delay(int value){
	for (int i = 0; i<value*1000; i++){}
}

//функция записи в 4-х битном режиме
void lcd_write(unsigned char c)
{
	delay(4);
	LCD_DATA = ((c >> 4) & 0x0F);
	LCD_STROBE();
	LCD_DATA = (c & 0x0F);
	LCD_STROBE();
}

//очистка дисплея
void lcd_clear(void)
{
	LCD_RS = 0;
	lcd_write(0x1);
	delay(4);
}

//функция записи строки
void lcd_puts(const char * s)
{
	LCD_RS = 1;
	while (*s)
		lcd_write(*s++);
}

//функция записи одного символа
void lcd_putch(char c)
{
	LCD_RS = 1;
	lcd_write(c);
}

//перемещение к выбранной позиции
void lcd_goto(unsigned char pos)
{
	LCD_RS = 0;
	lcd_write(0x80 + pos);
}

//инициализация дисплея, 4-х битный режим
void lcd_init()
{
	//отключаем альтернативные функции
	ANSEL = 0x00;
	//настроиваем порты A и B на выход
	TRISA = 0;
	TRISB = 0;
	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;

	delay(15);
	LCD_DATA = 3;
	LCD_STROBE();
	delay(5);
	LCD_STROBE();
	delay(1);
	LCD_STROBE();
	delay(1);
	LCD_DATA = 2;
	LCD_STROBE();

	lcd_write(0x28); // Set interface length
	lcd_write(0xF);  // Display On, Cursor On, Cursor Blink
	lcd_clear();     // Clear screen
	lcd_write(0x6);  // Set entry Mode
}
void main() {
	lcd_init();
	lcd_goto(0x00);	//переход на вторую линию
	lcd_puts("Hello, Vova!");
	for (;;);
}
