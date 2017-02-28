#include "../shared/board.c"

void init_GPIO(void);
void ai_reset(void);
char strat_can_win(const char pid, char *row, char *col);
char _strat_can_win_3(const char pid, const char pos1, const char pos2, const char pos3);

int main(void) {

    /* Player id */
    char player_id = 2;
    /* Opponent id */
    char opponent_id = 1;
    /* Setup GPIO ports */
    init_GPIO();

    /* Loop forever */
    char difficulty;
    char next_row = 0;
    char next_col = 0;

    char can_win;
    char must_block;

    while (0) {
        next_row = 0;
        next_col = 0;
        ai_reset();
        /* Stalling: Wait for recieving data */
        while (1) {
            can_win = strat_can_win(player_id, &next_row, &next_col);
            if (can_win) break;

            must_block = strat_can_win(opponent_id, &next_row, &next_col);
            if (must_block) break;
        }

        /* Transmit the given move that will be saved in next_row & next_col */
    }

}

/* Setup eventual GPIO ports here */
void init_GPIO(void) {

}

/* Reset AI variables */
void ai_reset(void) {
    board_reset();
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