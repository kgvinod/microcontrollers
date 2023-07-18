#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 8000000   // Define the crystal frequency

void UART_Init(long baud_rate) {
    TRISCbits.TRISC6 = 0;   // TX pin set as output
    TRISCbits.TRISC7 = 1;   // RX pin set as input
    
    // Calculation for BRGH and SPBRG value
    unsigned int x;
    x = (_XTAL_FREQ - baud_rate * 64) / (baud_rate * 64);
    if (x > 255) {          // If the result is not within the range
        x = (_XTAL_FREQ - baud_rate * 16) / (baud_rate * 16);
        BRGH = 1;           // Set the high baud rate select bit
    }
    if (x < 256) {
        SPBRG = x;          // Set the baud rate value
        SYNC = 0;           // Set the USART in Asynchronous mode
        SPEN = 1;           // Enable the serial port pins
        CREN = 1;           // Enable the receiver
        TXEN = 1;           // Enable the transmitter
    }
}

void UART_Write(char data) {
    while (!TXIF) {}        // Wait until the transmitter is ready
    TXREG = data;           // Write the data to USART
}

void main() {
    TRISB = 0;              // Set PORTB as output
    PORTB = 0;              // Clear PORTB initially
    
    UART_Init(9600);        // Initialize UART with baud rate 9600
    
    while (1) {
        if (RCIF) {          // Check if data is received
            char data = RCREG;  // Read the received data
            UART_Write(data);   // Echo back the received data
        }
    }
}

