#define _XTAL_FREQ 1000000
#include <htc.h>

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & INTCLK);

Init(void)
{
    TMR0 = 0;
    OPTION = 0b00000000;
    TRISD = 0x00;
    PORTD = 0xFF;
    GIE = 1;
}

main(void)
{
    Init();
    while(1)
    {
        //Poll the T0IF flag to see if TMR0 has overflowed
        if (TMR0>250)
        {
            PORTD = ~PORTD;
            TMR0 = 0;
            __delay_ms(100);
        }
    }
}
