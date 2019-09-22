#include "basic_server.h"

void print_player_state(struct Players *player) {
    char player_state[BUFFER_SIZE];
    sprintf(
        player_state, 
        "ID:\t  %d\nINIT:\t  %d\nMOVE:\t  %d\nCONTAINS: %d\nCORRECT:  %d\nLIVES:\t  %d\nINVALID:  %d", 
        player->id, 
        player->init,
        player->move, 
        player->contains, 
        player->correct, 
        player->lives,
        player->invalid
    );
            printf("-------------\n");
            printf("PLAYER STATE:\n");
            printf("-------------\n");
            printf("%s\n", player_state);
            printf("-------------\n\n");
}
