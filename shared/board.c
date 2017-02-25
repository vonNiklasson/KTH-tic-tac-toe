


void board_reset();         // Resets the state of the board

char board_moves_count;     // Keeps track of the number of moves
char board_moves[3];        // Markers per row
char board_result_round;    // 0 if draw, other wise 1 or 2 depnding on winner

/* Resets the variables for a new round */
void board_reset(void) {
    board_moves_count = 0;
    board_moves[0] = 0;
    board_moves[1] = 0;
    board_moves[2] = 0;

    board_result_round = 0;
}