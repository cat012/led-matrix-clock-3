// file: main.h
// compiler: XC8 1.33
// 18-08-2019


#ifndef MAIN_H
#define MAIN_H


#define _XTAL_FREQ 8000000


// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled and controlled by software (SBOREN is enabled))
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
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)


#include <xc.h>


#define  TMR0_PRESCALER_256  0b00000111
#define  TMR0_PRESCALER_128  0b00000110
#define  TMR0_PRESCALER_64   0b00000101
#define  TMR0_PRESCALER_32   0b00000100
#define  TMR0_PRESCALER_16   0b00000011
#define  TMR0_PRESCALER_8    0b00000010
#define  TMR0_PRESCALER_4    0b00000001
#define  TMR0_PRESCALER_2    0b00000000

#define TMR0_PRESCALER  TMR0_PRESCALER_2

#define TMR0_OVF_FREQ     200U
#define TMR0_OVF_PRELOAD  (65536U-(((_XTAL_FREQ/4U)/2U)/TMR0_OVF_FREQ))

#define TMR0_PERIOD_MS  (1000U/TMR0_OVF_FREQ)


#define EVENT_PERIOD(x)  ((x)/TMR0_PERIOD_MS)


#define BRIGHTNESS_EE_ADDR  4095
#define SCREENMODE_EE_ADDR  4094


//debug LED

#define DLED_PIN_0   PORTCbits.RC0
#define DLED_TRIS_0  TRISCbits.RC0

#define DLED_PIN_1   PORTCbits.RC1
#define DLED_TRIS_1  TRISCbits.RC1

#define DLED_OFF  DLED_PIN_0=0; DLED_TRIS_0=0; DLED_PIN_1=0; DLED_TRIS_1=0

#define DLED_GREEN   DLED_PIN_0=1; DLED_TRIS_0=0; DLED_PIN_1=0; DLED_TRIS_1=0
#define DLED_ORANGE  DLED_PIN_0=0; DLED_TRIS_0=0; DLED_PIN_1=1; DLED_TRIS_1=0



#endif /* MAIN_H */


