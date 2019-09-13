/**
* Based on code found at https://github.com/mafintosh/echo-servers.c (Copyright (c) 2014 Mathias Buus)
* Copyright 2019 Nicholas Pritchard, Ryan Bunney
**/

/**
 * @brief A simple example of a network server written in C implementing a basic echo
 * 
 * This is a good starting point for observing C-based network code but is by no means complete.
 * We encourage you to use this as a starting point for your project if you're not sure where to start.
 * Food for thought:
 *   - Can we wrap the action of sending ALL of out data and receiving ALL of the data?
 */

#include "basic_server.h"

// volatile bool breakflag = true;

int main (int argc, char *argv[]) {
    
    int server_fd, client_fd, err, opt_val, pid, min_player_count, child_num;
    int player_count = 0;
    struct sockaddr_in server, client;
    // time_t endwait, start;
    bool initialised = false;
    bool game_started = false;
    bool is_lobby_full = false;

    // PROCESS ARGUMENTS
    if (argc < 2) {
        fprintf(stderr,"Usage: %s [port]\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    // NO CAPACITY SPECIFIED
    else if (argc == 2) {
        min_player_count = 4;
    }
    // SPECIFIES A CAPACITY
    else if (argc == 3) {
        min_player_count = atoi(argv[2]);
        // if argument given is not an int (0) OR negative
        if (min_player_count == 0 || min_player_count < 0)
            min_player_count = 4;
    } // ELSE - other arguments are no processed

    int port = atoi(argv[1]);

    // INIT PLAYERS ARRAY
    int players[MAX_PLAYERS];

    // INIT PLAYER
    struct Players player; 
    {
        player.id = 0;
        player.init = 0;
        player.move = 0;
        player.contains = 0;
        player.correct = false;
        player.victory_state = CONTINUE;
        player.lives = NUM_LIVES;
        player.invalid = false;
    };

    // INIT GAME STATE
    struct Games game;
    {
        game.round = 1;
        game.red_blue[0] = 0;
        game.red_blue[1] = 0;
    };

    // SET THE SEED FOR RAND
    srand(time(NULL));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0){
        fprintf(stderr,"Could not create socket\n");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    opt_val = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val) < 0) {
        fprintf(stderr,"setsockopt failed\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0){
        fprintf(stderr,"Could not bind socket\n");
        // close(server_fd);
        exit(EXIT_FAILURE);
    } 

    err = listen(server_fd, 128);
    if (err < 0){
        fprintf(stderr,"Could not listen on socket\n");
        exit(EXIT_FAILURE);
    } 

    printf("\nServer is listening on port: %d\n", port);
    printf("------------------------------------------\n");
    printf("PLAYERS HAVE %d SECONDS TO JOIN LOBBY\n", LOBBY_FILL_TIME);
    printf("The minimum lobby size has been set to %d\n", min_player_count);
    printf("------------------------------------------\n");

    /*
     * FILL LOBBY WITH MULTIPLE CLIENTS
     * ---------------------------------
     *      Accepts clients within a time quantum (LOBBY_FILL_TIME), creating a child process 
     *      and pipes for each client. If the minimum number of clients has joined the lobby 
     *      after this time period, the parent process will break out of the following 
     *      while loop and the game will begin.
     */

    /*
    // THESE ARE TIMER FUNCTIONS THAT PREVENT THE JOINING OF LOBBY AFTER TIME OUT
    // unfortunately we couldn't get it working as you could enter the following
    // while loop during the lobby fill time, but will then block until a 
    // connection is made, by this time the lobby could have potentially expired
    endwait = start + LOBBY_FILL_TIME;
    signal(SIGALRM, ALARMhandler);
    alarm(LOBBY_FILL_TIME);
    */

    // With lobby fill time should be while (breakflag), breakflag being controlled
    // by the ALARMhandler function
    while (true) {
        socklen_t client_len = sizeof(client);
        // Will block until a connection is made
        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

        /* 
        // CHECK FOR LOBBY TIMEOUT
        if (!breakflag) {
            if (send(client_fd, "REJECT", 6, 0) < 0) {
                fprintf(stderr,"Client write failed\n");
                exit(EXIT_FAILURE);
            }
            shutdown(client_fd, 2);
            break;
        }
        */

        if (client_fd < 0) {
            fprintf(stderr,"Could not establish new connection\n");
            exit(EXIT_FAILURE);
        }

        // CREATE PIPES FOR IPC
        // These pipes will be unique for each client/child connection
        pipe(game.players[player_count].child_to_parent);
        pipe(game.players[player_count].parent_to_child);

        // CREATE CHILD PROCESS FOR EACH CLIENT
        pid = fork();
        switch(pid) {
            /* =============
             * ERROR ON FORK
             * ============= */
            case -1:
                perror("ERROR on fork");
                exit(EXIT_FAILURE);
                break;

            /* ===========================================
             * THE CHILD PROCESS - Created for each client
             * =========================================== */
            case 0:
                close(server_fd);
                player.id = getpid();
                child_num = player_count;
                char *buf;
                int read_from_client;
                while (true) {
                    // CHECK IF CLIENT IS INITIALISED
                    // If client is not initialised it will enter the following while loop until it becomes initialised
                    if (!initialised) {
                        while (true) {
                            // READ MESSAGE FROM CLIENT TO CHILD
                            buf = calloc(BUFFER_SIZE, sizeof(char)); // Clear our buffer so we don't accidentally send/print garbage
                            read_from_client = recv(client_fd, buf, BUFFER_SIZE, 0); // Try to read from the incoming client
                            char mess[20];

                            if (read_from_client < 0){
                                fprintf(stderr,"Client read failed\n");
                                exit(EXIT_FAILURE);
                            }

                            if (strstr(buf, "INIT") != NULL) {
                                // SEND TO PARENT THE ID OF THE INITIALISED CHILD PROCESS
                                write(game.players[child_num].child_to_parent[1], &player.id, SIZE_OF_INT);
                                // SEND WELCOME MESSAGE TO CLIENT
                                // TODO: dynamically allocate memory for mess
                                sprintf(mess, "WELCOME,%d", player.id);
                                if (send(client_fd, mess, strlen(mess), 0) < 0) {
                                    fprintf(stderr,"Client write failed\n");
                                    exit(EXIT_FAILURE);
                                }
                                initialised = true;
                                break;
                            }  
                            else {
                                // Send reject message
                                printf("ERROR: PLAYER MUST SEND 'INIT' BEFORE ANY OTHER MESSAGE\n");
                                continue;
                            }
                        }
                    }

                    /* =============================================
                     * AT THIS POINT THE CLIENT HAS BEEN INITIALISED
                     * ============================================= */

                    // START THE GAME IF IT HASN'T ALREADY BEEN STARTED
                    if (!game_started) {
                        // BLOCKING UNTIL MESSAGE IS RECIEVED FROM PARENT
                        // This message instructs child that the game has started and how many players are in the lobby
                        // If this function reads 0 (NOT_ENOUGH_PLAYERS), then a CANCEL message will be sent to the clients
                        read(game.players[child_num].parent_to_child[0], &player_count, SIZE_OF_INT);

                        // This If statement should never execute as the game will only run once the lobby is filled.
                        // This is because we couldn't figure out how to break from the while loop once Lobby Fill time
                        // had expired.
                        if (player_count == NOT_ENOUGH_PLAYERS) {
                            // Send START message to client
                            if (send(client_fd, "CANCEL", 6, 0) < 0) {
                                fprintf(stderr,"Client write failed\n");
                                exit(EXIT_FAILURE);
                            }
                            exit(EXIT_SUCCESS); // Exit the child process
                        }

                        // Send START message to client
                        buf = calloc(BUFFER_SIZE, sizeof(char));
                        sprintf(buf, "START,%d,%d", player_count, NUM_LIVES);
                        if (send(client_fd, buf, strlen(buf), 0) < 0) {
                            fprintf(stderr,"Client write failed\n");
                            exit(EXIT_FAILURE);
                        }
                        game_started = true;
                    }

                    // BLOCKING UNTIL MESSAGE IS RECIEVED FROM PARENT
                    // This message triggers the start of the round
                    read(game.players[child_num].parent_to_child[0], &game.round, SIZE_OF_INT);

                    // READING MESSAGE FROM CLIENT TO CHILD
                    buf = calloc(BUFFER_SIZE, sizeof(char)); // Clear our buffer so we don't accidentally send/print garbage
                    read_from_client = recv(client_fd, buf, BUFFER_SIZE, 0); // Try to read from the incoming client

                    if (read_from_client < 0){
                        fprintf(stderr,"Client read failed\n");
                        exit(EXIT_FAILURE);
                    }

                    // BLOCKING UNTIL MESSAGE IS RECIEVED FROM PARENT
                    // This message updates the child of the dice rolls
                    read(game.players[child_num].parent_to_child[0], &game.red_blue, SIZE_OF_INT * sizeof(game.red_blue));

                    // PROCESS THE RESULTS
                    // Will update the player struct and notify the client of the result
                    parse_message(&player, buf);
                    play_game_round(&player, game.red_blue[0], game.red_blue[1]);
                    send_message(&player, client_fd);

                    // UPDATE PARENT WITH HOW MANY LIVES REMAIN
                    write(game.players[child_num].child_to_parent[1], &player.lives, SIZE_OF_INT);

                    // BLOCKING UNTIL MESSAGE IS RECIEVED FROM PARENT
                    // This tells the child whether it is victorious (1), eliminated (0) or other (2).
                    // Other indicates the game should continue
                    read(game.players[child_num].parent_to_child[0], &player.victory_state, SIZE_OF_INT);

                    // CHECK VICTORY STATE OF PLAYER AND UPDATE THE CLIENT
                    buf = calloc(BUFFER_SIZE, sizeof(char));
                    switch (player.victory_state) {
                    case VICT:
                        sprintf(buf, "%d,VICT", player.id);
                        if (send(client_fd, buf, strlen(buf), 0) < 0) {
                            fprintf(stderr,"Client write failed\n");
                            exit(EXIT_FAILURE);
                        }
                        exit(EXIT_SUCCESS); // Exit the child process
                        break;
                    
                    case ELIM:
                        sprintf(buf, "%d,ELIM", player.id);
                        if (send(client_fd, buf, strlen(buf), 0) < 0) {
                            fprintf(stderr,"Client write failed\n");
                            exit(EXIT_FAILURE);
                        }
                        exit(EXIT_SUCCESS); // Exit the child process
                        break;

                    case CONTINUE: // Game to conitinue
                        break;

                    default:
                        break;
                    }
                }
                break;

            /* ==================
             * THE PARENT PROCESS
             * ================== */    
            default:
                // BLOCK UNTIL CHILD HAS INITIALISED
                read(game.players[player_count].child_to_parent[0], &players[player_count], SIZE_OF_INT * MAX_PLAYERS);
                printf("Player ID: %d has joined\n", players[player_count++]);
                close(client_fd);

                // FORCES A BREAK OUT OF THE WHILE LOOP ONCE THE MINIMUM NUMBER OF PLAYERS HAS FILLED THE LOBBY
                // We ideally wanted to only break once the timeout has reached, however, we were unable to make
                // that happen.
                if (player_count >= min_player_count) {
                    is_lobby_full = true;
                    break;
                }  
        }
        if (is_lobby_full) {
            break;
        }  
    }


    /*
     * PLAYING THE GAME
     * ---------------------------------
     *      This part of the code will be reached once the lobby
     *      fill time (LOBBY_FILL_TIME) has expired. 
     */
    #define PLAYERS_IN_GAME int i = 0; i < player_count; i++
    if (pid > 0) {
        printf("------------------------------------------\n\n");
        // CHECK IF ENOUGH PLAYERS ARE IN THE LOBBY
        // This code will never run as the above while loop will only break once the the min
        // player count has been filled, we have kept it here in the hope that we could figure
        // out how to break from the while loop once the Lobby Fill time had expired.
        if (player_count < min_player_count) {
            printf("TO FEW PLAYERS JOINED THE LOBBY, THE GAME WILL CANCEL\n\n");
            for (PLAYERS_IN_GAME) {
                int not_enough_players = NOT_ENOUGH_PLAYERS;
                write(game.players[i].parent_to_child[1], &not_enough_players, SIZE_OF_INT);
            }
            exit(EXIT_SUCCESS);
        }
        else {
            // TELL ALL CHILDREN THE GAME HAS STARTED
            for (PLAYERS_IN_GAME) {
                write(game.players[i].parent_to_child[1], &player_count, SIZE_OF_INT);
            }
            usleep(100000);
        }
        bool game_finished = false;

        printf("==========================================\n");
        printf("********** THE GAME HAS STARTED **********\n");
        printf("==========================================\n\n");

        // BEGIN THE GAME, ROUND BY ROUND
        while (true) {
            // TELL ALL CHILDREN THE ROUND HAS BEGUN
            for (PLAYERS_IN_GAME) {
                write(game.players[i].parent_to_child[1], &game.round, SIZE_OF_INT);
            }

            printf("-----------------------\n");
            printf("*-- ROUND %d STARTED --*\n", game.round);
            printf("-----------------------\n");

            // TIMEOUT FOR CLIENT TO MAKE MOVE (30 * 1s)
            for (int i = 0; i <= 30; i++) {
                printf("\rPlayers have %ds to make a move ", 30 - i);
                fflush(stdout);
                sleep(1);
            }

            printf("\n\nROLLING THE DICE\n");

            game.red_blue[0] = dice_roll();
            game.red_blue[1] = dice_roll();

            printf("The dice are %d and %d\n\n\n", game.red_blue[0], game.red_blue[1]);

            // TELL ALL CHILDREN THE RESULTS OF THE DICE ROLL
            for (PLAYERS_IN_GAME) {
                write(game.players[i].parent_to_child[1], &game.red_blue, SIZE_OF_INT * sizeof(game.red_blue));
            }

            // READ FROM CHILDREN THE LIVES REMAINING AND UPDATE PLAYER ARRAY
            for (PLAYERS_IN_GAME) {
                read(game.players[i].child_to_parent[0], &game.players[i].lives, SIZE_OF_INT);
            }

            // CHECK IF GAME IS COMPLETE
            int players_remaining = 0;
            for (PLAYERS_IN_GAME) {
                if (game.players[i].lives <= 0) { // Player has been eliminated
                    game.players[i].victory_state = ELIM;
                    continue;
                }
                game.players[i].victory_state = CONTINUE;
                players_remaining++;
            }
            if (players_remaining == 1) { // 1 Victorious player
                for (PLAYERS_IN_GAME) {
                    if (game.players[i].victory_state == CONTINUE) { // The only remaining player
                        game.players[i].victory_state = VICT;
                        game_finished = true;
                    } 
                }
            }

            // SEND VICTORY STATE TO CHILDREN
            for (PLAYERS_IN_GAME) {
                write(game.players[i].parent_to_child[1], &game.players[i].victory_state, SIZE_OF_INT);
            }

            // CHECK IF GAME HAS FINISHED AND BREAK WHILE LOOP
            if (game_finished) {
                break;
            }

            game.round++;
        }
        /*
         * CLOSING THE GAME
         * ---------------------------------
         *      This part of the code will be reached once the game has been completed, at this 
         *      point all the child processes should have been exited and the open sockets should 
         *      be shutdown. The program will exit with EXIT_SUCCESS
         */
        printf("=========================================\n");
        printf("*************** GAME OVER ***************\n");
        printf("=========================================\n\n");

        exit(EXIT_SUCCESS);
    }
}
