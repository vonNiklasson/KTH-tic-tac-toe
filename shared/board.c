#include <stdio.h>


void board_reset();         // Resets the state of the board
char position_is_free(const char row, const char col);

char board_moves_count;     // Keeps track of the number of moves
unsigned char board_moves[3] = "";        // Markers per row
char board_result_round;    // 0 if draw, other wise 1 or 2 depnding on winner

/* Resets the variables for a new round */
void board_reset(void) {
    board_moves_count = 0;
    board_moves[0] = 0;
    board_moves[1] = 0;
    board_moves[2] = 0;

    board_result_round = 0;
}

/* Returns 1 if the position is free, 0 if the position is taken.
 * Grid values:
      COL
     0 1 2
 R 0 x x x
 O 1 x x x
 W 2 x x x */
char position_is_free(const char row, const char col) {
    return !((board_moves[row] >> (6 - col*2)) & 3);
}

/*
int main() {
    board_moves[0] = 0b11000000;
    board_moves[1] = 0b00100000;
    board_moves[2] = 0b00001100;

    char pos = position_is_free(2, 2);
    printf("Position 0, 0:\t %d\n", pos);
}
*/