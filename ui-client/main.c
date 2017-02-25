#include <pic32mx.h>
#include "../shared/board.c"

/* Using char instead of int since no 
 * number will be higher than 255 anyway */

#define FINAL_SCORE 3

void game_reset();

char ai;            // The difficulty of the ai from 1-3 (0 if 2 players)
char player1_turn;  // If it's player 1 that's up to do next move

char player_1_score;
char player_2_score;

/* Resets the functions for a new game */
void game_reset(void) {
    board_reset();
    ai = 0;

    player1_turn = 1;

    player_1_score = 0;
    player_2_score = 0;
}

int main() {

    /* Half pseudo-function flow below */

    /* Loop forever to keep the game going */
    while (1) {
        game_reset();

        /* Stalling: Determine 1 or 2 players. If 1 player, set ai to 1 */

        if (ai != 0) {
            /* Stalling: Determine AI level (easy, medium or hard). Sets ai to 1-3 */
        }

        while (player_1_score != FINAL_SCORE && player_2_score != FINAL_SCORE) {
            board_reset();

            while (board_moves_count < 9 && board_result_round == 0) {
                if (player1_turn == 1) {
                    /* Stalling: Read input from player 1 */
                } else {
                    if (ai > 0) {
                        /* Send board and difficulty to ai-client */
                        /* Ẃait: Recieve next move */
                    } else {
                        /* Stalling: Read input from player 2 */
                    }
                }
                board_moves_count++;
                player1_turn = !player1_turn; // Sets player1_turn to 1 or 0 depending on last value

                /* Do check if a player has three in a row , if so quit the loop */
            }

            if (board_result_round != 0) {
                if (board_result_round == 1) {
                    player_1_score++;
                } else {
                    player_2_score++;
                }
                /* Flash the winning row a couple of times */
            } else {
                player_1_score++;
                player_2_score++;
                /* Do a slight delay and maybe blink some lights to indicate the game is over */
            }
        }
    }

	return 0;
}
