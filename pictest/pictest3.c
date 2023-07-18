#include <xc.h>

// Define baud rate
#define _XTAL_FREQ 20000000
#define BAUD 9600
#define BREG _XTAL_FREQ/(16UL*BAUD) - 1

// Assume GPIO pin is RA0
#define GPIO_PIN RA0

void UART_Init() {
    TXSTAbits.BRGH = 1;
    SPBRG = BREG;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
    TRISC7 = 1;
    TRISC6 = 0;
}


void UART_Write(char data) {
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

void UART_Write_Text(char* text) {
    int i;
    for(i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}

char UART_Read() {
    if(OERR) {
        CREN = 0;
        CREN = 1;
    }
    while(!RCIF);
    return RCREG;
}

void main(void) {
    UART_Init();

    TRISA0 = 0; 
    GPIO_PIN = 0; 

    int i;
    for (i=0;i<100;i++) {
	GPIO_PIN = 1;
        GPIO_PIN = 0;	
    }

    char buffer[6];
    int index = 0;
    int gpio_status = 0;
    
    UART_Write_Text("Hello from the test code");
    
    while(1) {
        char c = UART_Read();
        buffer[index] = c;
        index++;

        if (index == 5 && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[4] == '1') {
            GPIO_PIN = 1;
            gpio_status = 1;
            UART_Write_Text("SET 1");
            index = 0;
        } else if (index == 5 && buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T' && buffer[4] == '0') {
            GPIO_PIN = 0;
            gpio_status = 0;
            UART_Write_Text("SET 0");
            index = 0;
        } else if (index == 3 && buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T') {
            if (gpio_status == 1) {
                UART_Write_Text("1");
            } else {
                UART_Write_Text("0");
            }
            index = 0;
        } else if (index == 4 && buffer[0] == 'I' && buffer[1] == 'D' && buffer[2] == 'N' && buffer[3] == '?') {
            UART_Write_Text("pic16f877a");
            index = 0;
        } else if (index == 6) {
            UART_Write_Text("ERROR");  // send error message if command is not recognized
            index = 0;
        }
    }
}

