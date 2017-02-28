#include "../shared/board.c"

void init_GPIO(void);
char strat_can_win(const char pid, char *row, char *col);
char _strat_can_win_3(const char pid, const char pos1, const char pos2, const char pos3);
char strat_can_play_corner_or_center(char *row, char *col);
char strat_can_play_opposite_corner(const char opponent_id, char *row, char *col);
char strat_can_play_any_corner(char *row, char *col);
char strat_can_play_any_side(char *row, char *col);


/* Global var to spread the "don't care" moves */
char vary_move = 1;

int main(void) {

    /* Player id */
    char player_id = 2;
    /* Opponent id */
    char opponent_id = 1;
    /* Setup GPIO ports */
    init_GPIO();

    /* Loop forever */
    //char difficulty = 0;
    char next_row = 0;
    char next_col = 0;

    char found_strategy = 0;

    while (1) {
        board_reset();
        next_row = 0;
        next_col = 0;

        /* Stalling: Wait for recieving data */
        // Test data
        //board_moves[0] = 0b10000100;
        //board_moves[1] = 0b00010000;
        //board_moves[2] = 0b01001000;
        board_moves_count = board_count_moves(0);
        // End test data

        found_strategy = 0;

        while (1) {
            /* Reset the vary move */
            if (vary_move > 3) vary_move = 0;

            /* Check if can win */
            found_strategy = strat_can_win(player_id, &next_row, &next_col);
            if (found_strategy) break;

            /* Check if can block */
            found_strategy = strat_can_win(opponent_id, &next_row, &next_col);
            if (found_strategy) break;

            /* TODO: Create a fork */

            /* TODO: Block a fork */

            /* Play in a corner if it's the first move,
             * otherwise, play in the center if it works*/
            found_strategy = strat_can_play_corner_or_center(&next_row, &next_col);
            if (found_strategy) break;

            /* Play opposite corner */
            found_strategy = strat_can_play_opposite_corner(opponent_id, &next_row, &next_col);
            if (found_strategy) break;

            /* Play a corner */
            found_strategy = strat_can_play_any_corner(&next_row, &next_col);
            if (found_strategy) break;

            /* Play a side */
            found_strategy = strat_can_play_any_side(&next_row, &next_col);
            if (found_strategy) break;
        }

        /* Transmit the given move that will be saved in next_row & next_col */
        printf("Next row: %d\n", next_row);
        printf("Next col: %d\n", next_col);
        printf("------------\n");
        break;
    }
}

/* Setup eventual GPIO ports here */
void init_GPIO(void) {

}

char strat_can_win(const char pid, char *row, char *col) {
    /* Temp variables for row and col */
    char r;
    char c;

    int i;
    for (i = 0; i < 3; i++) {
        /* Check if any column on given row can give a win */
        c = _strat_can_win_3(   pid,
                                board_get_position(i, 0),
                                board_get_position(i, 1),
                                board_get_position(i, 2));
        /* If any column can give a win */
        if (c != -1) {
            *row = i;
            *col = c;
            return 1;
        }

        /* Check if any row on given column can give a win */
        r = _strat_can_win_3(   pid,
                                board_get_position(0, i),
                                board_get_position(1, i),
                                board_get_position(2, i));
        /* If any row can give a win */
        if (r != -1) {
            *row = r;
            *col = i;
            return 1;
        }
    }

    /* Check diagonally for a winning move */
    i = _strat_can_win_3(   pid,
                            board_get_position(0, 0),
                            board_get_position(1, 1),
                            board_get_position(2, 2));
    if (i != -1) {
        *row = i;
        *col = i;
        return 1;
    }

    /* Check diagonally (the other way) for a winning move */
    i = _strat_can_win_3(   pid,
                            board_get_position(2, 0),
                            board_get_position(1, 1),
                            board_get_position(0, 2));
    if (i != -1) {
        *row = 2 - i;
        *col = i;
        return 1;
    }

    return 0;
}

char _strat_can_win_3(const char pid, const char pos1, const char pos2, const char pos3) {
    if (pos1 == pid &&
        pos2 == pid &&
        pos3 == 0) {
        return 2;
    } else if (
        pos1 == pid &&
        pos2 == 0   &&
        pos3 == pid) {
        return 1;
    } else if (
        pos1 == 0   &&
        pos2 == pid &&
        pos3 == pid) {
        return 0;
    } else {
        return -1;
    }
}


char strat_can_play_corner_or_center(char *row, char *col) {
    if (board_count_moves(0) == 0) {
        return strat_can_play_any_corner(row, col);
    } else {
        /* Play the middle if possible */
        if (board_get_position(1, 1) == 0) {
            *row = 1;
            *col = 1;
            return 1;
        }
    }
    return 0;
}


char strat_can_play_opposite_corner(const char opponent_id, char *row, char *col) {
    int i; // Counter
    int r; // Temp row
    int c; // Temp col
    for (i = 0; i < 4; i++) {
        r = i & 1;
        c = (i >> 1) & 1;
        if (board_get_position(r * 2, c * 2) == opponent_id &&
            board_get_position((~r & 1) * 2, (~c & 1) * 2) == 0) {
            *row = (~r & 1) * 2;
            *col = (~c & 1) * 2;
            return 1;
        }
    }

    return 0;
}


char strat_can_play_any_corner(char *row, char *col) {
    int i; // Counter
    int r; // Temp row
    int c; // Temp col
    for (i = 0; i < 4; i++) {
        r = 2 * (vary_move & 1);
        c = 2 * ((vary_move >> 1) & 1);
        vary_move++;

        if (board_get_position(r, c) == 0) {
            *row = r;
            *col = c;
            return 1;
        }
    }

    return 0;
}


char strat_can_play_any_side(char *row, char *col) {
    int i; // Counter
    int r; // Temp row
    int c; // Temp col

    for (i = 0; i < 4; i++) {
        if (vary_move == 0) {
            r = 0;
            c = 1;
        } else if (vary_move == 1) {
            r = 1;
            c = 0;
        } else if (vary_move == 2) {
            r = 1;
            c = 2;
        } else if (vary_move == 3) {
            r = 2;
            c = 1;
        }

        if (board_get_position(r, c) == 0) {
            *row = r;
            *col = c;
            return 1;
        }

        vary_move++;
        vary_move = vary_move & 3; // Make sure to not let vary_move go over 3
    }

    return 0;
}
