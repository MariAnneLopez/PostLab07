/* 
 * File:   PostLab07.c
 * Author: Marian López
 *
 * Created on 7 de abril de 2022, 11:11 AM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

int conv[3];

//INTERRUPCIONES
void __interrupt() isr (){
    if(INTCONbits.T0IF){ //Si es interrupción de TMR0
        if (PORTDbits.RD0 == 0){
            PORTC = conv[0];
            PORTDbits.RD1 = 0;
            PORTDbits.RD0 = 1;
            return;
        }
        if (PORTDbits.RD1 == 0){
            PORTC = conv[1];
            PORTDbits.RD2 = 0;
            PORTDbits.RD1 = 1;
            return;
        }
        if (PORTDbits.RD2 == 0){
            PORTC = conv[2];
            PORTDbits.RD0 = 0;
            PORTDbits.RD2 = 1;
            return;
        }
        INTCONbits.T0IF = 0; //Apaga bandera
        TMR0 = 217; //Resetea TMR0    
        }
    return;
}

//SETUP
void setup (void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0;
    TRISD = 0;
    PORTA = 0;
    PORTC = 0;
    PORTD = 6;
    
    OPTION_REGbits.nRBPU = 0; //Pullups activados
    INTCONbits.GIE = 1; //Interrupciones globales
    INTCONbits.T0IE = 1; //Interupción de TMR0
    INTCONbits.T0IF = 0; //Bandera de interrupción apagada
    
    OPTION_REG = 0b01010011; //Prescaler 1 : 16
    OSCCON = 0b00101000; //Oscilador a 250 kHz
    TMR0 = 217; //N de TMR0 para 10 ms
}

void main (void){
    setup();
    int val[3];
    while(1){
        if (!PORTBbits.RB0){
            while(!PORTBbits.RB0){ //Antirrebote
            }
            ++PORTA;
        }
        if (!PORTBbits.RB1){
            while(!PORTBbits.RB1){ //Antirrebote
            }
            --PORTA;
        }
        val[3] = decimal(PORTA); //Número en decimales
        for (int i = 0; i < 3; ++i){
            switch (val[i]){
                case 0: conv[i] = 0b11000000;
                case 1: conv[i] = 0b11111001;
                case 2: conv[i] = 0b10100100;
                case 3: conv[i] = 0b10110000;
                case 4: conv[i] = 0b10011001;
                case 5: conv[i] = 0b10010010;
                case 6: conv[i] = 0b10000010;
                case 7: conv[i] = 0b11111000;
                case 8: conv[i] = 0b10000000;
                case 9: conv[i] = 0b10010000;
                default: conv[i] = 0b11000000;
            }
        }
    }
    return;
}

int decimal (int dividendo){
    int centenas = 0;
    int decenas = 0;
    int unidades = 0;
    int residuo = 0;
    int cero = dividendo - dividendo;
    if (cero == 0){
        return (centenas, decenas, unidades);
    }
    centenas = dividendo / 100;
    residuo = dividendo % 100;
    if (residuo == 0){
        return (centenas, decenas, unidades);    
    }
    decenas = residuo / 10;
    residuo = residuo % 10;
    if (residuo == 0){
        return (centenas, decenas, unidades);   
    }
    unidades = residuo;
    return (centenas, decenas, unidades);
}