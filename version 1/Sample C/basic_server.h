#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MAX_PLAYERS 5
#define NUM_LIVES   3

// GENERAL
#define SIZE_OF_INT sizeof(int)

// WAIT TIMES
#define LOBBY_FILL_TIME 30
#define ROUND_WAIT_TIME 30
#define TIME_OUT        0

// MOVE TYPE
#define EVEN 1
#define ODD  2
#define DOUB 3
#define CON  4

// PACKET ORDER
#define ID    0
#define MOV   1
#define GUESS 2
#define CON_N 3

// DELIMITERS
#define DELIM ","

// PLAYER VICTORY STATE
#define VICT     1
#define ELIM     0
#define CONTINUE 2

#define NOT_ENOUGH_PLAYERS 0

struct Players {
  int id;
  bool init;
  int move;
  int contains;
  bool correct;
  int lives;
  bool invalid;
  int victory_state;
  int child_to_parent[2];
  int parent_to_child[2];
};

struct Games {
  int round;
  int red_blue[2];
  struct Players players[MAX_PLAYERS];
};

// GAME LOGIC
extern int dice_roll(void);
extern int check_parity(int);
extern int is_even(int, int);
extern int is_odd(int, int);
extern int is_doubles(int, int);
extern int contains(int, int, int);
extern void play_game_round(struct Players *, int, int);
void process_EVEN(struct Players *, int, int);
void process_ODD(struct Players *, int, int);
void process_DOUB(struct Players *, int, int);
void process_CON(struct Players *, int, int);

// DEBUGGING
extern void print_player_state(struct Players *);

// PROCESSING
extern void parse_message(struct Players *, char *);
extern int what_move(char *);
extern void send_message(struct Players *, int);
void do_processing(int, struct Players *);
// void ALARMhandler(int);

