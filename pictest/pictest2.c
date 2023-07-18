#include <xc.h>

// Configuration bits
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000 // 20MHz Clock
#define BAUDRATE 9600
#define SPBRG_VALUE ((_XTAL_FREQ/16)/BAUDRATE) - 1

void USART_Init() {
    TXSTA = 0b00100100; // Enable transmit, set high baud rate
    RCSTA = 0b10010000; // Enable continuous receive
    SPBRG = SPBRG_VALUE;
}

void USART_Send_Char(char ch) {
    while(!TXIF); // Wait for previous transmission to finish
    TXREG = ch; // Transmit char
}

void USART_Send_String(const char* str) {
    while(*str != '\0') {
        USART_Send_Char(*str);
        str++;
    }
}

void main() {
    TRISA0 = 0; // Set RA0 as output
    RA0 = 0; // Initial state is low

    USART_Init();

    char buffer[20];
    int i = 0;

    while(1) {
        if (RCIF) {
            char receivedChar = RCREG;
            buffer[i] = receivedChar;
            i++;

            if (i == 4 && strncmp(buffer, "IDN?", 4) == 0) {
                USART_Send_String("pic16f877a");
                i = 0;
            } else if (i == 5 && strncmp(buffer, "SET ", 4) == 0) {
                if (buffer[4] == '1') {
                    RA0 = 1;
                    USART_Send_String("SET 1");
                } else if (buffer[4] == '0') {
                    RA0 = 0;
                    USART_Send_String("SET 0");
                }
                i = 0;
            } else if (i == 3 && strncmp(buffer, "GET", 3) == 0) {
                if (RA0) {
                    USART_Send_String("1");
                } else {
                    USART_Send_String("0");
                }
                i = 0;
            } else if (i == 20) {
                i = 0; // if the command is not recognized, reset the buffer
            }
        }
    }
}
