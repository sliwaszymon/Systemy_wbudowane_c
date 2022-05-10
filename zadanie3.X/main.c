// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF

#include <xc.h>

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0
#define L_CR	0x0F		
#define L_NCR	0x0C	

#define L_CFG   0x38

void delay(unsigned int ms)
{
    unsigned int i;
    unsigned char j;
    
 for (i =0; i< ms; i++)
 {
 
  for (j =0 ; j < 200; j++)
   {
      Nop();
      Nop();
      Nop();
      Nop();
      Nop();
   }
 }
}

unsigned int adc(unsigned char kanal)
{
    switch(kanal)
    {
        case 0: ADCON0=0x01; break; //P1
        case 1: ADCON0=0x05; break; //P2
        case 2: ADCON0=0x09; break; 
    }
    
    ADCON0bits.GO=1;
    while(ADCON0bits.GO == 1);

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}

void lcd_wr(unsigned char val)
{
  LPORT=val;
}

void lcd_cmd(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=0;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}
 
void lcd_dat(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=1;
        //delay(3);
        LENA=0;
        //delay(3);
	LENA=1;
}

void lcd_init(void)
{
	LENA=0;
	LDAT=0;
	delay(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	delay(5);
	lcd_cmd(L_CFG);
        delay(1);
	lcd_cmd(L_CFG); //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON); //liga
	lcd_cmd(L_CLR); //limpa
	lcd_cmd(L_CFG); //configura
        lcd_cmd(L_L1);
}

void lcd_str(const char* str)
{
 unsigned char i=0;
  
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }  
}

void lcd_custom_char(unsigned char *Pattern, const char Location){
    unsigned int i;
    lcd_cmd(0x40+(Location*8));
    for (i=0; i<8; i++){
        lcd_dat(Pattern[i]);
    }
}

void main(void) {
    
    //Inicjalizacja konwertera analogowo cyfrowego
    ADCON0=0x01;
    ADCON1=0x0B;
    ADCON2=0x01;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    lcd_init(); //Inicjalizacja wy?wietlacza
    lcd_cmd(L_CLR); //Czyszczenie wy?wietlacza
    
    unsigned char full_bottle[]= {0x4,0x4,0xe,0xe,0xe,0xe,0xe,0xe}; 
    unsigned char half_bottle[]= {0x4,0x4,0xe,0xa,0xa,0xe,0xe,0xe};
    unsigned char empty_bottle[]= {0x4,0x4,0xe,0xa,0xa,0xa,0xa,0xe};
    unsigned char smile[]= {0x0,0xa,0xa,0xa,0x0,0x11,0xe,0x0};
    unsigned char sad[] = {0x0,0xa,0xa,0xa,0x0,0xe,0x11,0x0};
    unsigned char ciapki[] = {0x1b,0x1b,0x1b,0x0,0x0,0x0,0x0,0x0};
    lcd_custom_char(full_bottle, 0);
    lcd_custom_char(half_bottle, 1);
    lcd_custom_char(empty_bottle, 2);
    lcd_custom_char(smile, 3);
    lcd_custom_char(sad, 4);
    lcd_custom_char(ciapki, 5);
    
    
    while(1)
    {
       delay(500);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("  Lubisz piwko? "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(0);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(0);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("   My rowniez!  "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(3);
        
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("  Ale za szybko "); //napis
       lcd_cmd(L_L2); //Przej??ie do drugiej linii
       lcd_str("  sie konczy?   "); //napis
       lcd_cmd(0xCE); //Ustawienie karetki w drugiej
       lcd_dat(4);
       
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("  Gul!          "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(0);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(0);
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("      Gul!      "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(1);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(1);
       delay(1000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("          Gul!  "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(2);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(4);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(2);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("   Jest po 22?  "); //napis
       lcd_cmd(0xC5); //Ustawienie karetki w drugiej
       lcd_dat(4);
       lcd_cmd(0xC7); //Ustawienie karetki w drugiej
       lcd_dat(4);
       lcd_cmd(0xC9); //Ustawienie karetki w drugiej
       lcd_dat(4);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("Dla nas to nie  "); //napis
       lcd_cmd(L_L2);
       lcd_str("problem!        ");
       lcd_cmd(0xCA); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xCB); //Ustawienie karetki w drugiej
       lcd_dat(0);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("Omijamy lukiem  "); //napis
       lcd_cmd(L_L2);
       lcd_str("prohibicje!     ");
       lcd_cmd(0xCD); //Ustawienie karetki w drugiej
       lcd_dat(0);
       lcd_cmd(0xCE); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xCF); //Ustawienie karetki w drugiej
       lcd_dat(0);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("GDY JEST WEEKEND"); //napis
       lcd_cmd(L_L2);
       lcd_str("   LUB CI ZLE   ");
       lcd_cmd(0xC1); //Ustawienie karetki w drugiej
       lcd_dat(4);
       lcd_cmd(0xCE); //Ustawienie karetki w drugiej
       lcd_dat(4);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str("  WPADAJ DO NAS "); //napis
       lcd_cmd(L_L2);
       lcd_str("   POROB SIE!   ");
       lcd_cmd(0xC1); //Ustawienie karetki w drugiej
       lcd_dat(3);
       lcd_cmd(0xCE); //Ustawienie karetki w drugiej
       lcd_dat(3);
       
       delay(2000);
       lcd_cmd(L_CLR);
       lcd_cmd(L_L1); //Ustawienie karetki w pierwszej linii
       lcd_str(" Z KODEM  SYSWB "); //napis
       lcd_cmd(0x89); //Ustawienie karetki w drugiej
       lcd_dat(5);
       lcd_cmd(0x8F); //Ustawienie karetki w drugiej
       lcd_dat(5);
       lcd_cmd(L_L2);
       lcd_str("4 PROCENT ZNIZKI");
       delay(5000);       
    }
    
    return;
}

