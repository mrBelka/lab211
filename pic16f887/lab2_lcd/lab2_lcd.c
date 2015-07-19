//тактовая частота
#define _XTAL_FREQ 4000000

#include    <htc.h>

#define    LCD_RS RA3
#define    LCD_RW RA4
#define    LCD_EN RA5
#define  LCD_DATA PORTB

//стробирование (макрос, операции выполняются последовательно)
#define LCD_STROBE() ((LCD_EN = 1),(LCD_EN = 0))

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & HS);

//запись в 4-хбитном режиме
void lcd_write(unsigned char c)
{
	__delay_ms(50);
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
        __delay_ms(50);
}

//запись строки
void lcd_puts(const char * s)
{
	LCD_RS = 1;
	while(*s)
            lcd_write(*s++);
}

//запись символа
void lcd_putch(char c)
{
	LCD_RS = 1;
	lcd_write(c);
}

//переход к указанному адресу
void lcd_goto(unsigned char pos)
{
	LCD_RS = 0;
	lcd_write(0x80 + pos);
}

//инициализация контроллера дисплея
void lcd_init()
{
	//отключаем альтернативные функции выводов порта А
	ANSEL = 0x00;
	//выводы портов А и В настраиваем на выход
	TRISA = 0;
	TRISB = 0;

        //инициализируем управляющие выводы
	LCD_RS = 0;
	LCD_EN = 0;
	LCD_RW = 0;

        //задержка для того, чтобы питание стало стабильным
	__delay_ms(100);

        //0b00101000 - 4-хбитная шина данных, 2 строки, 5х7 пикселей на 1 символ
        lcd_write(0x28);
        //0b00001111 - включаем дисплей, включаем курсор, курсор мигает
	lcd_write(0xF);
        //очищаем экран
	lcd_clear();
        //инкремент адреса, дисплей не сдвигается
	lcd_write(0x6);
}
void main() {
	lcd_init();
	lcd_goto(0x00);
	lcd_puts("Hello, Vova!");
	for (;;);
}
