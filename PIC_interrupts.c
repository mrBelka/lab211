#define _XTAL_FREQ 4000000
#include<htc.h>

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & INTCLK);
void interrupt INT()
{
     if(RBIF)
     {
         PORTD=~PORTD;
         RBIF=0;
     }
}// interrupt
void Init()
{
     ANSEL  = 0;
     ANSELH = 0;
     TRISB=0b00010000;
     TRISD=0b00000000;
     IOCB4 = 1;
     RBIE=1;
     GIE=1;
}// Init
void main() {

   Init();
   while(1) { }
   }

/*#define _XTAL_FREQ 4000000
#include<htc.h>

__CONFIG(WDTDIS & UNPROTECT & MCLREN & LVPDIS & INTCLK);

void main()
{
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0x00;
    PORTD = 0xFF;
    RBIE = 1;
    GIE = 1;
    while(1);
}

void interrupt INTER(void)
{
    if(RBIF){
        PORTD = 0x00;
        RBIF = 0;
    }
}*/
