#include <pic32mx.h>

void platform_delay(int milliseconds);
char platform_get_button_state(const char row, const char col);
char platform_get_any_button_state(char *row, char *col);
void platform_set_led(const char row, const char col, char state);
void platform_set_all_led(const char state);

void _PORTB(int state, int shift);
void _PORTD(int state, int shift);
void _PORTE(int state, int shift);
void _PORTF(int state, int shift);
void _PORTG(int state, int shift);

void platform_delay(int milliseconds) {
    /* TODO: Define a delay function here */
}

/* Setup inputs & outputs etc */
void platform_init(void) {
    //sets 9 pins to inputs for playfieldbuttons
    TRISDSET = 0b11100001111;
    TRISGSET = 0b1000000000;
    TRISFSET = 0b10;

    //sets 18 pins to outputs for RGB lights
    TRISGCLR = 0b110000000;
    TRISDCLR = 0b100011100000;
    TRISECLR = 0b11111111;
    TRISFCLR = 0b1110000;
    TRISBCLR = 0b10;
}

/* Gets the state of the button input */
char platform_get_button_state(const char row, const char col) {
    if (row == 0) {
        if (col == 0) {
            return ((PORTD &= 0b100000000) >> 8);
        } else if (col == 1) {
            return (PORTD &= 0b1);
        } else if (col == 2) {
            return ((PORTF &=0b10) >> 1);
        }
    } else if (row == 1) {
        if (col == 0) {
            return ((PORTD &=0b10) >> 1);
        } else if (col == 1) {
            return ((PORTD &= 0b100) >> 2);
        } else if (col == 2) {
            return ((PORTD &= 0b1000000000) >> 9);
        }
    } else if (row == 2) {
        if (col == 0) {
            return ((PORTD &= 0b10000000000) >> 10);
        } else if (col == 1) {
            return ((PORTD &= 0b1000) >> 3);
        } else if (col == 2) {
            return ((PORTG &= 0b1000000000) >> 9);
        }
    }
    return 0;
}

/* Returns if a button is pressed, if so returns 1 
 * and set row and col to pressed button */
char platform_get_any_button_state(char *row, char *col) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (platform_get_button_state(i, j) != 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
}

/* Lights up a LED in given state (0-2) */
void platform_set_led(const char row, const char col, const char state) {
    char light1 = state & 1;
    char light2 = (state & 2) >> 1;

    if (row == 0) {
        if (col == 0) {
            _PORTG(light1, 8); 
            _PORTE(light2, 7); 
            
        } else if (col == 1) {
            _PORTG(light1, 7); 
            _PORTD(light2, 5); 
            
        } else if (col == 2) {
            _PORTE(light1, 0); 
            _PORTD(light2, 11);
            
        }
    } else if (row == 1) {
        if (col == 0) {
            _PORTE(light1, 1); 
            _PORTD(light2, 6);
        } else if (col == 1) {
            _PORTE(light1, 2); 
            _PORTD(light2, 7);
        } else if (col == 2) {
            _PORTE(light1, 3); 
            _PORTF(light2, 6);
        }
    } else if (row == 2) {
        if (col == 0) {
            _PORTE(light1, 4); 
            _PORTF(light2, 4);
        } else if (col == 1) {
            _PORTE(light1, 5); 
            _PORTF(light2, 6);
        } else if (col == 2) {
            _PORTE(light1, 6); 
            _PORTB(light2, 1);
        }
    }
}

/* Sets all LED to given state (0-2) */
void platform_set_all_led(const char state) {
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, state);
        }
    }
}

/* Helper functions to set and clear each port more easily */
void _PORTB(int state, int shift) {
    if (state == 1) {
        PORTBCLR = (1 << shift);
    } else {
        PORTBSET = (1 << shift);
    }
}

void _PORTD(int state, int shift) {
    if (state == 1) {
        PORTDCLR = (1 << shift);
    } else {
        PORTDSET = (1 << shift);
    }
}

void _PORTE(int state, int shift) {
    if (state == 1) {
        PORTECLR = (1 << shift);
    } else {
        PORTESET = (1 << shift);
    }
}

void _PORTF(int state, int shift) {
    if (state == 1) {
        PORTFCLR = (1 << shift);
    } else {
        PORTFSET = (1 << shift);
    }
}

void _PORTG(int state, int shift) {
    if (state == 1) {
        PORTGCLR = (1 << shift);
    } else {
        PORTGSET = (1 << shift);
    }
}
