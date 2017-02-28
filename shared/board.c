#include <stdio.h>

void board_reset();         // Resets the state of the board
char board_get_position(const char row, const char col);
char board_set_position(const char row, const char col, char value);
char board_player_has_won(void);

char board_moves_count;             // Keeps track of the number of moves
unsigned char board_moves[3] = "";  // Markers per row
char board_result_round;            // 0 if draw, other wise 1 or 2 depnding on winner

/* Resets the variables for a new round */
void board_reset(void) {
    board_moves_count = 0;
    board_moves[0] = 0;
    board_moves[1] = 0;
    board_moves[2] = 0;

    board_result_round = 0;
}

/* Returns player index on given positio.n
 * Grid values:
      COL
     0 1 2
 R 0 x x x
 O 1 x x x
 W 2 x x x
 */
char board_get_position(const char row, const char col) {
    return ((board_moves[row] >> (6 - col*2)) & 3);
}

/* Sets player index on given positio.n
 * Grid values:
      COL
     0 1 2
 R 0 x x x
 O 1 x x x
 W 2 x x x
 */
void board_set_poisition(const char row, const char col, char value) {
    board_moves[row] &= ~(3 << (6 - col*2));
    board_moves[row] |= value << (6 - col*2);
}

char board_player_has_won(void) {
    int i;
    for (i = 0; i < 2; i++) {
        /* Check each column and row for a tree in a row*/
        if (board_get_position(0, i) == board_get_position(1, i) &&
            board_get_position(0, i) == board_get_position(2, i) &&
            board_get_position(0, i) != 0) {
            return board_get_position(0, i);
        } else if (
            board_get_position(i, 0) == board_get_position(i, 1) &&
            board_get_position(i, 0) == board_get_position(i, 2) &&
            board_get_position(i, 0) != 0) {
            return board_get_position(i, 0);
        }
    }

    /* Check diagonally */
    if (board_get_position(0, 0) == board_get_position(1, 1) &&
        board_get_position(0, 0) == board_get_position(2, 2) &&
        board_get_position(0, 0) != 0) {
        return board_get_position(0, 0);
    } else if (
        board_get_position(0, 2) == board_get_position(1, 1) &&
        board_get_position(0, 2) == board_get_position(2, 0) &&
        board_get_position(0, 2) != 0) {
        return board_get_position(0, 2);
    }

    return 0;
}

/*
int main() {
    board_set_poisition(0, 0, 2);
    board_set_poisition(1, 0, 2);
    board_set_poisition(2, 0, 2);
    board_set_poisition(1, 1, 1);
    board_set_poisition(2, 2, 2);
    char winning = board_player_has_won();
    printf("Player has won:\t %d\n", winning);
}
*/