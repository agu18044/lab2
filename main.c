/*
 * File:   main.c
 * Author: Diego A.
 *
 */

//******************************************************************************
//  Liberias
//******************************************************************************
#include <xc.h>
#include <stdint.h>
#include "7seg.h"
#include "adc.h"

//******************************************************************************
//  Palabra de cofiguración
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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

//******************************************************************************
//  Variables
//******************************************************************************
#define _XTAL_FREQ 8000000 
uint8_t valorADC = 0;
uint8_t mtx = 0;
uint8_t contador;
uint8_t n1;
uint8_t n2;


//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);

//******************************************************************************
//  Interupciones
//******************************************************************************
void __interrupt() isr(void){
    
    if (INTCONbits.RBIF == 1){      // comprobar interupcion del puerto B
        if (PORTBbits.RB0 ==0){     // interupcion en RB0 incrementa PORTD 
            PORTD++;                
        }
        if (PORTBbits.RB1 == 0){    // interupcion en RB1 decrementa PORTD
            PORTD--;
        }
        INTCONbits.RBIF = 0;        //  limpia la bandera 
    }
    if (TMR0IF == 1){               // interupcion para la multiplexación
        TMR0 = 240;                 // caraga el valor al TMR0
        if (mtx == 1){
            PORTC = display(n1);    //se mueve el valor analogo al display1
            PORTEbits.RE1 = 1;      //se enciende el display    
        }
        else if (mtx == 2){;
            PORTC = display(n2);    //se mueve el valor analogo al display2
            PORTEbits.RE2 = 1;      //se enciende el display   
            mtx = 0;                
        }
       mtx++;
       TMR0IF = 0;                  // limpia la bandera  
    }
    if (ADIF){                      // interupcion para leer el valor del ADC
        valorADC = ADRESH;          // sube el valor de ADRESH a una variable
        ADIF = 0;                   // limpia la bandera
    }
}

//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {       
    setup();
    while(1){
        ADCmed(0); // se seleciona el RA0 como entrada analogica
        contador = PORTD;
       
        // separa la lectura del ADC en dos nibles
        n1 = valorADC & 15;
        n2 = (valorADC >> 4) & 15;

        // funcion para compar el valor del contador con el del canal analogico
        if (valorADC>contador){
            PORTEbits.RE0 = 1;
        }
        else{
            PORTEbits.RE0 = 0;
        }
    }
}

//******************************************************************************
//  Cofiguración
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    TRISBbits.TRISB0 = 1;   // entrada boton 1
    TRISBbits.TRISB1 = 1;   // entrada boton 2 

    //configuracion de interupciones en el puerto B
    IOCBbits.IOCB0 = 1;     // habilita ISR de Interrupt of change en RB0
    IOCBbits.IOCB1 = 1;     // habilita ISR de Interrupt of change en RB1
    INTCONbits.RBIE = 1;    // habilita ISR del puerto B
    INTCONbits.RBIF = 0;    // limpia bandera de ISR Interrupt on change
    INTCONbits.GIE = 1;     // habilita ISRs
    // configuracion de interupciones del TIMER0
    INTCONbits.TMR0IE = 1;     //timer0
    INTCONbits.TMR0IF=0;
    OPTION_REGbits.T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
    OPTION_REGbits.T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
    OPTION_REGbits.PSA = 0;   // bit 3  Prescaler Assignment bit...0 = Prescaler is assigned to the Timer0
    OPTION_REGbits.PS2 = 1;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 240;               //valor del timer 0 
    // configuración ADC
    ADCON0bits.ADCS = 1;    // Conversion a FOSC/8
    ADCON1 = 0;             // todos los bits de ADCON en 0
    }