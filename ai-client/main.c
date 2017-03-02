#include "../shared/board.c"
#include "../shared/protocol.c"
#include "../shared/protocol_platform_wiringpi.c"
#include "ai.c"

#include <stdio.h>

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

int main(void) {
    hdp_initialize();

    /* Player id */
    char player_id = 2;
    /* Opponent id */
    char opponent_id = 1;
    /* Setup GPIO ports */
    init_GPIO();

    char next_row = 0;
    char next_col = 0;

    /* Loop forever */
    printf("Starting loop");
    while(1) {
        board_reset();
        /* Stalling: Wait for recieving data */
        printf("--Waiting for data\n");
        poll_read_data();
        printf("Recieved data, calculating next move\n");
        get_next_move(player_id, opponent_id, difficulty, &next_row, &next_col);
        printf("Next move: Row: %d, Col: %d\n", next_row, next_col);
        printf("Sending data\n");
        send_data(next_row, next_col);
    }
}
