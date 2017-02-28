#include <pic32mx.h>

#define testport 0b1
#define TRIS TRISF
#define PORT PORTF

int main() {
    TRIS &= ~testport;
    while (1) {
        PORT |= testport;
        sleep(200);
        PORT &= ~testport;
        sleep(200);
    }
	return 0;
}

void sleep(milliseconds) {
    /* Dummy function here */
    int i, j, k;
    for(i = 0; i < milliseconds; i++)
    {
        for(j = 0; j < 8; j++)
        {
            for(k = 0; k < 635; k++)
            {
               // waste function, volatile makes sure it is not being optimized out by compiler
                int volatile t = 120 * j * i + k;
                t = t + 5;
            }
        }
    }   
}

/*
// GPIO 2
TRISD &= ~0b0100000000;
PORTD |= 0b0100000000;
PORTD &= ~0b0100000000;

// GPIO 3
TRISD &= ~0b1;

// GPIO 4
TRISF &= ~0b10;

// GPIO 5
TRISD &= ~0b10;

// GPIO 6
TRISD &= ~0b100;

// GPIO 7
TRISD &= ~0b1000000000;

// GPIO 8
TRISD &= ~0b10000000000;

// GPIO 9
TRISD &= ~0b1000;

// GPIO 10
TRISG &= ~0b1000000000;

// GPIO 11
TRISG &= ~0b100000000;

// GPIO 12
TRISG &= ~0b10000000;

// GPIO 13
TRISG &= ~0b1000000;

// GPIO A
TRISB &= ~0b1;

// GPIO A0
TRISB &= ~0b100;

// GPIO A1
TRISB &= ~0b10000;

// GPIO A2
TRISB &= ~0b100000000;

// GPIO A3 - No response for output

// GPIO A4 - No response for output

// GPIO A5 - No response for output

// GPIO A6
TRISB &= ~0b1000;

// GPIO A7
TRISB &= ~0b100000;

// GPIO A8
TRISB &= ~0b1000000000;

// GPIO A9 - No response for output

// GPIO A10 - No response for output

// GPIO A11
TRISB &= ~0b1000000000000000;

// GPIO 26
TRISE &= ~0b1;

// GPIO 27
TRISE &= ~0b10;

// GPIO 28
TRISE &= ~0b100;

// GPIO 29
TRISE &= ~0b1000;

// GPIO 30
TRISE &= ~0b10000;

// GPIO 31
TRISE &= ~0b100000;

// GPIO 32
TRISE &= ~0b1000000;

// GPIO 33
TRISE &= ~0b10000000;

// GPIO 34
TRISD &= ~0b100000;

// GPIO 35
TRISD &= ~0b100000000000;

// GPIO 36
TRISD &= ~0b1000000;

// GPIO 37
TRISd &= ~0b10000000;

// GPIO 38
TRISF &= ~0b1000000;

// GPIO 39
TRISF &= ~0b10000;

// GPIO 40
TRISF &= ~0b100000;

// GPIO 41
TRISB &= ~0b10;

*/