#include "basic_server.h"

/*
FUNCTION TO HANDLE LOBBY FILL TIME INTERRUPT
Unfortunately not working, so commenting it out

void ALARMhandler(int sig)
{
    signal(SIGALRM, SIG_IGN);        
    printf("\nLOBBY FILL TIME HAS EXPIRED\n");
    printf("Any new clients attempting to join will recieve REJECT Message\n");
    breakflag = false;
    printf("%d\n", breakflag);
    signal(SIGALRM, ALARMhandler);
}
*/

int what_move(char *move) {
    if (strstr(move, "EVEN") != NULL)
        return EVEN;
    else if (strstr(move, "ODD") != NULL)
        return ODD;
    else if (strstr(move, "DOUB") != NULL)
        return DOUB;
    else if (strstr(move, "CON") != NULL)
        return CON;
    else
        return 0;
}

void parse_message(struct Players *player, char *client_mess) {
    char *token;
    bool is_move = false;
    int i = 0;

    token = strtok(client_mess, DELIM);
    while(token != NULL) {
        switch(i++) {
            case ID:
                if (strstr(token, "INIT") != NULL) {
                    player->init = true;
                    player->move = 0;
                }
                else if (player->id != atoi(token)) {
                    player->invalid = true;
                }
                break;
            case MOV:
                if (strstr(token, "MOV") != NULL)
                    is_move = true;
                else
                    is_move = false;
                break;    
            case GUESS:
                if (is_move) 
                    player->move = what_move(token);
                else
                    player->move = 0;
                break;    
            case CON_N:
                if (player->move == CON)
                    player->contains = atoi(token);
                break;    
        }
        if (player->init) break;
        token = strtok(NULL, DELIM);
    }
    return;
}

void send_message(struct Players *player, int client_fd) {
    char mess[BUFFER_SIZE];
    // CHECK VALIDITY OF PLAYER
    if (player->invalid) {
        if (send(client_fd, "REJECT", 6, 0) < 0) {
            fprintf(stderr,"Client write failed\n");
            exit(EXIT_FAILURE);
        }
    }
    // CHECK TO SEE IF INIT MESSAGE WAS RECIEVED AND ACCEPTED
    else if (player->init) {
        sprintf(mess, "WELCOME,%d", player->id);
        if (send(client_fd, mess, strlen(mess), 0) < 0) {
            fprintf(stderr,"Client write failed\n");
            exit(EXIT_FAILURE);
        }
        // RESET PLAYER INIT STATE
        player->init = false;
    }
    else if (player->correct) {
        sprintf(mess, "%d,PASS", player->id);
        if (send(client_fd, mess, strlen(mess), 0) < 0) {
            fprintf(stderr,"Client write failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (!player->correct) {
        sprintf(mess, "%d,FAIL", player->id);
        if (send(client_fd, mess, strlen(mess), 0) < 0) {
            fprintf(stderr,"Client write failed\n");
            exit(EXIT_FAILURE);
        }
    }
}

void do_processing(int client_fd, struct Players *player) {
    char *buf;
    
    while (player->lives > 0) {  
        buf = calloc(BUFFER_SIZE, sizeof(char)); // Clear our buffer so we don't accidentally send/print garbage
        int read = recv(client_fd, buf, BUFFER_SIZE, 0); // Try to read from the incoming client

        if (read < 0){
            fprintf(stderr,"Client read failed\n");
            exit(EXIT_FAILURE);
        }
        printf("server recieved: %s\n\n", buf);

        // AFTER THE 30s ie. after second read with dice roll
        parse_message(player, buf);
        if (player->move != 0)
            play_game_round(player, 1, 2);

        print_player_state(player);
        send_message(player, client_fd);
        if (player->invalid) {
            shutdown(client_fd, 2);
        }

        free(buf);
    }
    buf = calloc(BUFFER_SIZE, sizeof(char));
    sprintf(buf, "%d,ELIM", player->id);
    if (send(client_fd, buf, strlen(buf), 0) < 0) {
        fprintf(stderr,"Client write failed\n");
        exit(EXIT_FAILURE);
    }
    printf("NO MORE LIVES!!\nTerminating Connection\n\n");
    shutdown(client_fd, 1);
}
