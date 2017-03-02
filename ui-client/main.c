#include "../shared/board.c"
#include "../shared/protocol.c"
#include "../shared/protocol_platform_pic32mx.c"
#include "../ai-client/ai.c"
//#include "platform.c" (Will be included during compile)

void play_game(void);
char play_round(char player1_turn, const char ai);
char poll_read_player_select(void);
char poll_read_ai_select(void);
void poll_read_valid_move(char *row, char *col);
void poll_read_ai_move(const char difficulty, char *row, char *col);

void print_game_state(void);
void print_game_score(const char p1_score, const char p2_score, const char only_player);
void clear_led_and_mini_sleep(void);

#define FINAL_SCORE 3

/* Using char instead of int since no
 * number will be higher than 255 anyway */

int main() {

    platform_init();
    platform_set_all_led(0);
    platform_set_player_led(1, 0);
    platform_set_player_led(2, 0);

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

    clear_led_and_mini_sleep();

    if (ai != 0) {
        /* Stalling: Determine AI level (easy, medium or hard). Sets ai to 1-3 */
        ai = poll_read_ai_select();
    }

    clear_led_and_mini_sleep();

    while (player_1_score != FINAL_SCORE && player_2_score != FINAL_SCORE) {
        platform_set_all_led(0);
        player1_start = !(rounds_played % 2);

        board_result_round = play_round(player1_start, ai);
        platform_set_player_led(1, 0);
        platform_set_player_led(2, 0);
        platform_sleep(1000);

        clear_led_and_mini_sleep();
        print_game_score(player_1_score, player_2_score, 0);

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

        /* Show the score and wait a moment before starting the new match */
        if (player_1_score != 0 || player_2_score != 0) {
            platform_sleep(1000);
        }
        print_game_score(player_1_score, player_2_score, 0);
        platform_sleep(1500);
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
            platform_set_player_led(1, 1);
            platform_set_player_led(2, 0);
            /* Stalling: Read input from player 1 */
            poll_read_valid_move(&next_row, &next_col);
            board_set_position(next_row, next_col, 1);
        } else {
            platform_set_player_led(1, 0);
            platform_set_player_led(2, 1);
            if (ai == 0) {
                /* Stalling: Read input from player 2 */
                poll_read_valid_move(&next_row, &next_col);
            } else {
                /* Send board and difficulty to ai-client */
                /* Stalling: Recieve next move */
                poll_read_ai_move(ai, &next_row, &next_col);
            }
            board_set_position(next_row, next_col, 2);
        }
        /* Print current state with LEDs */
        print_game_state();
        platform_sleep(20);

        board_moves_count = board_count_moves(0);
        player1_turn = !player1_turn; // Sets player1_turn to 1 or 0 depending on last value

        /* Do check if a player has three in a row , if so quit the loop */
        result = board_player_has_won();
        if (result != 0) break;
    }

    return result;
}

char poll_read_player_select(void) {
    platform_set_all_led(0);

    platform_set_led(0, 0, 3);
    platform_set_led(0, 2, 3);

    /* Wait for a button to be pressed */
    while (platform_get_button_state(0, 0) == 0 && 
           platform_get_button_state(0, 2) == 0) {};

    /* If 1 player was pressed */
    if (platform_get_button_state(0, 0) == 1) {
        platform_sleep(10);
        /* Wait for its release and send player state */
        while (platform_get_button_state(0, 0) == 1) {}
        return 1;
    }

    /* If 2 player was pressed */
    if (platform_get_button_state(0, 2) == 1) {
        platform_sleep(10);
        /* Wait for its release and send player state */
        while (platform_get_button_state(0, 2) == 1) {}
        return 2;
    }
}


char poll_read_ai_select(void) {
    platform_set_all_led(0);

    platform_set_led(0, 0, 3);
    platform_set_led(0, 1, 3);
    platform_set_led(0, 2, 3);

    /* Wait for a button to be pressed */
    while (platform_get_button_state(0, 0) == 0 && 
           platform_get_button_state(0, 1) == 0 && 
           platform_get_button_state(0, 2) == 0) {};

    /* If level 1 was pressed */
    if (platform_get_button_state(0, 0) == 1) {
        platform_sleep(10);
        /* Wait for its release and send level state */
        while (platform_get_button_state(0, 0) == 1) {}
        return 1;
    }

    /* If level 2 was pressed */
    if (platform_get_button_state(0, 1) == 1) {
        platform_sleep(10);
        /* Wait for its release and send level state */
        while (platform_get_button_state(0, 1) == 1) {}
        return 2;
    }

    /* If level 3 was pressed */
    if (platform_get_button_state(0, 2) == 1) {
        platform_sleep(10);
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

void poll_read_ai_move(const char difficulty, char *row, char *col) {
    /* Current fix when protocol won't work */
    platform_sleep(500);
    get_next_move(2, 1, difficulty, row, col);
    platform_sleep(500);
    return;

    hdp_initialize();

    hdp_bitrate = hdp_get_nearest_bitrate(8);

    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 0, difficulty);
    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 1, board_moves[0]);
    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 2, board_moves[1]);
    hdp_data_set_byte(hdp_send_data, DATA_BYTES_RESERVED, 3, board_moves[2]);

    hdp_transmit();

    /* Now wait for response */
    hdp_recieve();

    *row = hdp_recieve_data[0];
    *col = hdp_recieve_data[1];
    return;
}

/* Loop through and print the current game state */
void print_game_state(void) {
    int j;
    int i;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, board_get_position(i, j));
        }
    }
}

/* Prints the current score on row 2-0 on col 0 and 2 */
void print_game_score(const char p1_score, const char p2_score, const char only_player) {
    platform_set_all_led(0);

    int i;
    if (only_player == 0 || only_player == 1) {
        for (i = 0; i < p1_score; i++) {
            platform_set_led(2-i, 0, 1);
        }
    }

    if (only_player == 0 || only_player == 2) {
        for (i = 0; i < p2_score; i++) {
            platform_set_led(2-i, 2, 2);
        }
    }
}

void clear_led_and_mini_sleep(void) {
    platform_set_all_led(0);
    platform_sleep(100);
}

void led_test(void) {
    int i;
    int j;

    for(i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, 1);
            platform_sleep(250);
            //platform_set_led(i, j, 2);
            //platform_sleep(500);
        }
    }

    for(i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, 2);
            platform_sleep(250);
            //platform_set_led(i, j, 2);
            //platform_sleep(500);
        }
    }

    for(i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, 3);
            platform_sleep(250);
            //platform_set_led(i, j, 2);
            //platform_sleep(500);
        }
    }

    for(i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            platform_set_led(i, j, 0);
            platform_sleep(250);
            //platform_set_led(i, j, 2);
            //platform_sleep(500);
        }
    }
}

void button_test(void) {
    int i;
    int j;

    while (1) {

        for(i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                platform_set_led(i, j, platform_get_button_state(i, j));
            }
        }
    }
}