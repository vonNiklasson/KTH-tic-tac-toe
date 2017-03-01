#include "../shared/board.c"
//#include "platform.c" (Will be included during compile)

void play_game(void);
char play_round(char player1_turn, const char ai);
char poll_read_player_select(void);
char poll_read_ai_select(void);
void poll_read_valid_move(char *row, char *col);
void poll_read_ai_move(char *row, char *col);

#define FINAL_SCORE 3

/* Using char instead of int since no
 * number will be higher than 255 anyway */

int main() {

    /* Loop forever to keep the game going */
    while (1) {
        play_game();
    }

	return 0;
}

void play_game(void) {

    board_reset();

    char ai; // The difficulty of the ai from 1-3 (0 if 2 players)

    char player_1_score = 0;
    char player_2_score = 0;

    char rounds_played = 0;
    char player1_start = 1;

    /* Stalling: Determine 1 or 2 players. If 1 player, set ai to 1 */
    if (poll_read_player_select() == 1) {
        ai = 1;
    } else {
        ai = 0;
    }

    if (ai != 0) {
        /* Stalling: Determine AI level (easy, medium or hard). Sets ai to 1-3 */
        ai = poll_read_ai_select();
    }

    while (player_1_score != FINAL_SCORE && player_2_score != FINAL_SCORE) {
        platform_set_all_led(0);
        player1_start = rounds_played % 2;

        board_result_round = play_round(player1_start, ai);

        if (board_result_round != 0) {
            if (board_result_round == 1) {
                player_1_score++;
            } else {
                player_2_score++;
            }
            /* TODO: Flash the winning row a couple of times */
        } else {
            player_1_score++;
            player_2_score++;
        }
        rounds_played++;
        /* TODO: Wait a moment before starting the new match */
    }
    /* TODO: Do a slight delay and maybe blink some lights to indicate the game is over */
}

char play_round(char player1_turn, const char ai) {
    char next_row = 0;
    char next_col = 0;

    char result = 0;
    board_reset();
    while (board_moves_count < 9 && result == 0) {
        if (player1_turn == 1) {
            /* Stalling: Read input from player 1 */
            poll_read_valid_move(&next_row, &next_col);
            board_set_position(next_row, next_col, 1);
        } else {
            if (ai == 0) {
                /* Stalling: Read input from player 2 */
                poll_read_valid_move(&next_row, &next_col);
            } else {
                /* Send board and difficulty to ai-client */
                /* Stalling: Recieve next move */
                poll_read_ai_move(&next_row, &next_col);
            }
            board_set_position(next_row, next_col, 2);
        }
        /* TODO: Print current state with LEDs */
        print_game_state();

        board_moves_count = board_count_moves(0);
        player1_turn = !player1_turn; // Sets player1_turn to 1 or 0 depending on last value

        /* Do check if a player has three in a row , if so quit the loop */
        result = board_player_has_won();
        if (result != 0) break;
    }

    return result;
}

char poll_read_player_select(void) {
    /* Wait for a button to be pressed */
    while (platform_get_button_state(0, 0) == 0 && 
           platform_get_button_state(0, 2) == 0) {};

    /* If 1 player was pressed */
    if (platform_get_button_state(0, 0) == 1) {
        /* Wait for its release and send player state */
        while (platform_get_button_state(0, 0) == 1) {}
        return 1;
    }

    /* If 2 player was pressed */
    if (platform_get_button_state(0, 2) == 1) {
        /* Wait for its release and send player state */
        while (platform_get_button_state(0, 2) == 1) {}
        return 2;
    }
}


char poll_read_ai_select(void) {
    /* Wait for a button to be pressed */
    while (platform_get_button_state(0, 0) == 0 && 
           platform_get_button_state(0, 1) == 0 && 
           platform_get_button_state(0, 2) == 0) {};

    /* If level 1 was pressed */
    if (platform_get_button_state(0, 0) == 1) {
        /* Wait for its release and send level state */
        while (platform_get_button_state(0, 0) == 1) {}
        return 1;
    }

    /* If level 2 was pressed */
    if (platform_get_button_state(0, 1) == 1) {
        /* Wait for its release and send level state */
        while (platform_get_button_state(0, 1) == 1) {}
        return 2;
    }

    /* If level 3 was pressed */
    if (platform_get_button_state(0, 2) == 1) {
        /* Wait for its release and send level state */
        while (platform_get_button_state(0, 2) == 1) {}
        return 3;
    }
}

void poll_read_valid_move(char *row, char *col) {
    while (1) {
        if (platform_get_any_button_state(row, col)) {
            if (board_get_position(*row, *col) == 0) {
                while (platform_get_button_state(row, col) == 1) {}
                return;
            }
        }
    }
}

void poll_read_ai_move(char *row, char *col) {
    while (1) {}
    // TODO: Make a nice protocol here
}

void print_game_state(void) {
    /* TODO: Loop through and print the current game state */
}
