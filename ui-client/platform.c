#include <pic32mx.h>

char platform_get_button_state(const char row, const char col);
char platform_get_any_button_state(char *row, char *col);
void platform_set_led(const char row, const char col, const char state);
void platform_set_all_led(const char state);

void _PORTB(int state, int shift);
void _PORTD(int state, int shift);
void _PORTF(int state, int shift);
void _PORTG(int state, int shift);

/* Setup inputs & outputs etc */
void platform_init(void) {
    // TRISESET = ... etc
}

/* Gets the state of the button input */
char platform_get_button_state(const char row, const char col) {
    if (row == 0) {
        if (col == 0) {
            return 0;
        } else if (col == 1) {
            return 0;
        } else if (col == 2) {
            return 0;
        }
    } else if (row == 1) {
        if (col == 0) {
            return 0;
        } else if (col == 1) {
            return 0;
        } else if (col == 2) {
            return 0;
        }
    } else if (row == 2) {
        if (col == 0) {
            return 0;
        } else if (col == 1) {
            return 0;
        } else if (col == 2) {
            return 0;
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
    char b0 = state & 1;
    char b1 = state & 2;

    if (row == 0) {
        if (col == 0) {
            _PORTD(b0, 8); // Example
            _PORTD(b1, 9); // code
        } else if (col == 1) {
            
        } else if (col == 2) {
            
        }
    } else if (row == 1) {
        if (col == 0) {
            
        } else if (col == 1) {
            
        } else if (col == 2) {
            
        }
    } else if (row == 2) {
        if (col == 0) {
            
        } else if (col == 1) {
            
        } else if (col == 2) {
            
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
        PORTBSET = (1 << shift)
    }
}

void _PORTD(int state, int shift) {
    if (state == 1) {
        PORTDCLR = (1 << shift);
    } else {
        PORTDSET = (1 << shift)
    }
}

void _PORTF(int state, int shift) {
    if (state == 1) {
        PORTFCLR = (1 << shift);
    } else {
        PORTFSET = (1 << shift)
    }
}

void _PORTG(int state, int shift) {
    if (state == 1) {
        PORTGCLR = (1 << shift);
    } else {
        PORTGSET = (1 << shift)
    }
}
