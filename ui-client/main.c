#include <pic32mx.h>

int moves_count;    // Keeps track of the number of moves
char moves_r1;      // Markers on row 1
char moves_r2;      // Markers on row 2
char moves_r3;      // Markers on row 3

int ai;             // The difficulty of the ai from 1-3 (0 if 2 players)
int player1_turn;   // If it's player 1 that's up to do next move

int result;         // 0 if draw, other wise 1 or 2 depnding on winner

/* Resets the functions for a new game */
int reset(void) {
    moves_count = 0;
    moves_r1 = 0;
    moves_r2 = 0;
    moves_r3 = 0;

    ai = 0;

    player1_turn = 1;

    result = 0;
}

int main() {

    /* Pseudo-function flow */

    while (1) {
        reset();

        /* Stalling: Determine 1 or 2 players. If 1 player, set ai to 1 */

        if (ai != 0) {
            /* Stalling: Determine AI level (easy, medium or hard). Sets ai to 1-3 */
        }

        while (moves_count < 9 && result == 0) {
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
            moves_count++;
            player1_turn = !player1_turn; // Sets player1_turn to 1 or 0 depending on last value

            /* Do check if a player has three in a row , if so quit the loop */
        }

        // if result != 0:
            /* Flash the winning row a couple of times */
        // else:
            /* Do a slight delay and maybe blink some lights to indicate the game is over */
    }

	return 0;
}
