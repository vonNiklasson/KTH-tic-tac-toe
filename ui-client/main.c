#include <pic32mx.h>

int main() {

    /* Pseudo-function flow */

    while (1) {
        bool ai = false;
        int ai_difficulty;

        /* Wait: Determine 1 or 2 players */

        // if 1 player:
            /* Determine AI level (easy, medium or hard) */

        int moves = 0;
        int winner = 0; // player 1 or player 2

        while (moves < 9 && winner == 0) {

            /* Wait: Read input from player 1 */

            // if ai:
                /* Send board and difficulty to ai-client */
                /* Ẃait: Recieve next move */
            // else:
                /* Wait: Read input from player 2 */

            /* Do check if a player has three in a row, if so quit the loop */
        }

        // if winner != 0:
            /* Flash the winning row a couple of times */
        // else:
            /* Do a slight delay and maybe blink some lights to indicate the game is over */
    }

	return 0;
}
