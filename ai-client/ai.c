
void get_next_move(const char player_id, const char opponent_id, const char difficulty, char *row, char *col);
void init_GPIO(void);
char _strat_can_win(const char pid, char *row, char *col);
char strat_can_win(const char pid, char *row, char *col);
char _strat_can_win_3(const char pid, const char pos1, const char pos2, const char pos3);
char strat_can_fork(const char pid, const char oid, char *row, char *col);
char strat_can_play_corner_or_center(char *row, char *col);
char strat_can_play_opposite_corner(const char opponent_id, char *row, char *col);
char strat_can_play_any_corner(char *row, char *col);
char strat_can_play_any_side(char *row, char *col);

void poll_read_data(void);
void send_data(const char row, const char col);


/* Global var to spread the "don't care" moves */
char vary_move = 1;
/* Global variable so the protocol can access it */
char difficulty = 0;

void get_next_move(const char player_id, const char opponent_id, const char difficulty, char *row, char *col) {
    char found_strategy = 0;

    /* Reset the vary move */
    if (vary_move > 3) vary_move = 0;

    /* Check if can win */
    found_strategy = strat_can_win(player_id, row, col);
    if (found_strategy) return;

    /* Check if can block */
    found_strategy = strat_can_win(opponent_id, row, col);
    if (found_strategy) return;

    /* Create a fork (only on difficulty level 3) */
    if (difficulty > 2) {
        found_strategy = strat_can_fork(player_id, opponent_id, row, col);
        if (found_strategy) return;
    }

    /* Block a fork (only on difficulty level 2 or higher) */
    if (difficulty > 1) {
        found_strategy = strat_can_fork(opponent_id, player_id, row, col);
        if (found_strategy) return;
    }

    /* Play in a corner if it's the first move,
     * otherwise, play in the center if it works*/
    found_strategy = strat_can_play_corner_or_center(row, col);
    if (found_strategy) return;

    /* Play opposite corner */
    found_strategy = strat_can_play_opposite_corner(opponent_id, row, col);
    if (found_strategy) return;

    /* Play a corner */
    found_strategy = strat_can_play_any_corner(row, col);
    if (found_strategy) return;

    /* Play a side */
    found_strategy = strat_can_play_any_side(row, col);
    if (found_strategy) return;
}

/* Setup eventual GPIO ports here */
void init_GPIO(void) {

}

char _strat_can_win(const char pid, char *row, char *col) {
    int i;
    int j;

    char temp;

    char can_win = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (board_get_position(i, j) == 0) {
                board_set_position(i, j, pid);  // Sumilate the position to the player id
                if (strat_can_win(pid, &temp, &temp)) {
                    can_win = 1;
                }
                board_set_position(i, j, 0);    // Resets the simulated position

                if (can_win) {
                    *row = i;
                    *col = j;
                    return 1;
                }
            }
        }
    }

    return 0;
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

char strat_can_fork(const char pid, const char oid, char *row, char *col) {
    int i;
    int j;

    char temp_row;
    char temp_col;
    char temp;

    char fork_exist = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (board_get_position(i, j) == 0) {
                board_set_position(i, j, pid);  // Sumilate the position to the player id
                if (strat_can_win(pid, &temp_row, &temp_col)) {
                    board_set_position(temp_row, temp_col, oid);  // Simulate a block
                    fork_exist = strat_can_win(pid, &temp, &temp);
                    board_set_position(temp_row, temp_col, 0);    // Reset the simulate block
                }
                board_set_position(i, j, 0);    // Resets the simulated position

                if (fork_exist) {
                    *row = i;
                    *col = j;
                    return 1;
                }
            }
        }
    }

    return 0;
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

void poll_read_data(void) {
    hdp_bitrate = hdp_get_nearest_bitrate(8);

    hdp_recieve();

    difficulty = hdp_recieve_data[0];
    board_moves[0] = hdp_recieve_data[1];
    board_moves[1] = hdp_recieve_data[2];
    board_moves[2] = hdp_recieve_data[3];
}

void send_data(const char row, const char col) {
    hdp_bitrate = hdp_get_nearest_bitrate(8);

    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 0, row);
    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 1, col);

    hdp_transmit();
}