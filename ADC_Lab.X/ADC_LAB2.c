/*
 * File:   ADC_LAB2.c
 * Author: Juanf
 *
 * Created on 26 de agosto de 2024, 11:29 AM
 */

#include <xc.h>

void ADC_Init(void);  // Declaración de la función que inicializa el ADC.
int ADC_Read(int);  // Declaración de la función que lee un valor digital del ADC desde un canal especificado.

#define VREF 5.00  // Define una constante VREF que representa el voltaje de referencia del ADC, en este caso, 5.00 V.

void main(void) {    
    char data[10];  // Declaración de un arreglo de caracteres para almacenar datos (no se utiliza en este ejemplo).
    int digital;  // Variable para almacenar el valor digital leído del ADC.
    unsigned int bits10a8;  // Variable para almacenar el valor de 10 bits reducido a 8 bits.
    TRISB = 0x00;  // Configura el puerto B como salida.
    OSCCON = 0x72;  // Configura el oscilador a 8 MHz.
    ADC_Init();  // Inicializa el ADC.
    while(1) {
        digital = ADC_Read(0);  // Lee el valor digital del canal 0 del ADC.
        bits10a8 = (unsigned int)(digital * 0.25);  // Reducimos el valor de 10 bits a 8 bits.
        PORTB = bits10a8;  // Envía el valor de 8 bits al puerto B.
    }  
    return;  // No es necesario en un bucle infinito como este.
}

void ADC_Init(void) {    
    TRISA = 0xff;  // Configura el puerto A como entrada.
    ADCON1 = 0x0e;  // Configura el ADC para utilizar Vss y Vdd como referencia y el pin AN0 como entrada analógica.
    ADCON2 = 0x92;  // Configura el ADC para utilizar un tiempo de espera mínimo de 4 TAD y un tiempo de adquisición de FOSC/32.
    ADRESH = 0;  // Limpia el registro de datos alto.
    ADRESL = 0;  // Limpia el registro de datos bajo.
}

int ADC_Read(int channel) {  // Función que lee un valor digital del ADC desde un canal especificado.
    int digital;  // Variable para almacenar el valor digital leído del ADC.
    ADCON0 = (ADCON0 & 0b11000011) | ((channel << 2) & 0b00111100);  // Selecciona el canal especificado.
    ADCON0 |= ((1 << ADON) | (1 << GO));  // Habilita el módulo ADC y inicia la conversión analógica a digital.
    while(ADCON0bits.GO_nDONE == 1);  // Espera a que la conversión analógica a digital termine.
    digital = (ADRESH * 256) | ADRESL;  // Combina los registros de datos alto y bajo para obtener el valor de 10 bits.
    return digital;  // Devuelve el valor digital leído del ADC.
}
