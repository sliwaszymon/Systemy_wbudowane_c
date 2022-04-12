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

unsigned char grey = 0;
unsigned char bcd = 0;
int sum = 0;
int added = 0;
int direction = 1;

void delay(unsigned int ms){
    unsigned int i;
    unsigned char j;
    for (i =0; i< ms; i++){
        for (j =0 ; j < 200; j++){
            Nop();
            Nop();
            Nop();
            Nop();
            Nop();
        }
    }
}

int setTask(int task, int change) {
    task = task + change;
    if (task == 10) {
        task = 1;
    } else if (task == 0) {
        task = 9;
    }
    return task;
}

unsigned char setDisplay(int task) {
    if (task == 2) {
        return 255;
    } else if (task == 9) {
        return 1;
    } else if (task == 1 || task == 3 || task == 7){
        return 0;
    }
}

unsigned char bin_up(unsigned char display) {
    return (display + 1);
}

unsigned char bin_down(unsigned char display) {
    return (display - 1);
}

unsigned char grey_up(unsigned char display) {
    display = (grey >>1) ^ grey;
    grey = grey + 1;
    return display;
}

unsigned char grey_down(unsigned char display) {
    display = (grey >>1) ^ grey;
    grey = grey - 1;
    return display;
}

unsigned char bcd_up() {
    unsigned int display;
    if (bcd > 99) {
        bcd = 1;
    } else {
        display = ((bcd / 10) << 4) | (bcd % 10);
        bcd = bcd + 1;
    }
    return display;
}

unsigned char bcd_down() {
    unsigned int display;
    if (bcd == 0) {
        bcd = 99;
    } else {
        display = ((bcd / 10) << 4) | (bcd % 10);
        bcd = bcd - 1;
    }
    return display;
}

unsigned char snake(unsigned char display){
    if (display < 7) {
		display = (display << 1) ^ 1;
	} else {
		if (direction == 1) {
            if (display == 224) {
				direction = 0;
				display = display >> 1;
            } else {
				display = display << 1; 	
			}
		} else {
			if (display == 7) {
				direction = 1;
				display = display << 1;
			} else {
				display = display >>1;
			}
		}
	}
    return display;
}

unsigned char queue() {
    if (sum == 128 || sum == 192 || sum == 224 || sum == 240 || sum == 248 || sum ==252 || sum == 254) {
        added = 1;
        sum = sum ^ added;
    } else if (sum == 255) {
        sum = 1;
        added = 1;
    } else {
        sum = sum ^ added;
        added = added << 1;
        sum = sum ^ added;
    }
    return sum;
}

unsigned char prng(unsigned char display) {
    int ans, xored;
	xored = (((display >> 0) & 1) ^ ((display >> 1) & 1)) ^ (((display >> 4) & 1) ^ ((display >> 5) & 1));
	ans = (xored << 5) | (display >> 1);
    return ans;
}

void main(void) {
    ADCON1=0x0F;
    
    TRISA=0xC3;
    TRISB=0x3F;   
    TRISC=0x01;
    TRISD=0x00;
    TRISE=0x00;
    
    PORTB = 0;
    
    unsigned char display = 0;
    int task = 1;
    
    while (1) {
        PORTD = display;
        delay(100); //Opoznienie
        
        //Symulator nie jest doskonaly - drobne spowolnienie odczytu przycisków
        unsigned int i = 6000;
        while (PORTBbits.RB4 && PORTBbits.RB3 && i > 0) {
            i--;
        }
        if (PORTBbits.RB3 == 0) {   //task do przodu
            task = setTask(task, 1);
            display = setDisplay(task);
            if (task == 3) {
                grey = 1;
            } else if (task == 4) {
                grey = 255;
            } else if (task == 5) {
                bcd = 0;
            } else if (task == 6) {
                bcd = 99;
            } else if (task == 7) {
                direction = 1;
            } else if (task == 8) {
                sum = 1;
                added = 1;
            }
        } else if (PORTBbits.RB4 == 0) {   //task do tylu
            task = setTask(task, -1);
            display = setDisplay(task);
            if (task == 3) {
                grey = 1;
            } else if (task == 4) {
                grey = 255;
            } else if (task == 5) {
                bcd = 0;
            } else if (task == 6) {
                bcd = 99;
            } else if (task == 7) {
                direction = 1;
            } else if (task == 8) {
                sum = 1;
                added = 1;
            }
        }
        
        if (task == 1) {
            display = bin_up(display);
        } else if (task == 2) {
            display = bin_down(display);
        } else if (task == 3) {
            display = grey_up(display);
        } else if (task == 4) {
            display = grey_down(display);
        } else if (task == 5) {
            display = bcd_up();
        } else if (task == 6) {
            display = bcd_down();
        } else if (task == 7) {
            display = snake(display);
        } else if (task == 8) {
            display = queue();
        } else {
            display = prng(display);
        }
    }
    return;
}
