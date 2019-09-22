#include "basic_server.h"

// PLAY GAME ROUND
void play_game_round(struct Players *player, int red, int blue) {
    int move = player->move;
    switch (move)
    {
    case EVEN:
        process_EVEN(player, red, blue);
        break;
    case ODD:
        process_ODD(player, red, blue);
        break;
    case DOUB:
        process_DOUB(player, red, blue);
        break;
    case CON:
        process_CON(player, red, blue);
        break;
    default:
        break;
    }
    return;
}


// DICE LOGIC FUNCTIONS
int dice_roll()
{
    return rand() % 6 + 1;
}

// Return 1 if Even, 0 if Odd
int check_parity(int n)
{
    // MAKE ERROR CHECK: n between 1 and 6 (make function isDiceNum())
    return n % 2 == 0;
}

int is_even(int red, int blue)
{
    return check_parity(red + blue) && !is_doubles(red, blue);
}

int is_odd(int red, int blue)
{
    int sum = red + blue;
    return !check_parity(sum) && (sum) > 5;
}

int is_doubles(int red, int blue)
{
    return red == blue;
}

int contains(int n, int red, int blue)
{
    return red == n || blue == n;
}


// PROCESS FUNCTIONS
void process_EVEN(struct Players *p, int red, int blue)
{
    if (is_even(red, blue))
        p->correct = true;
    else
    {
        p->lives--;
        p->correct = false;
    }
}

void process_ODD(struct Players *p, int red, int blue)
{
    if (is_odd(red, blue))
        p->correct = true;
    else
    {
        p->lives--;
        p->correct = false;
    }
}

void process_DOUB(struct Players *p, int red, int blue)
{
    if (is_doubles(red, blue))
        p->correct = true;
    else
    {
        p->lives--;
        p->correct = false;
    }
}

void process_CON(struct Players *p, int red, int blue)
{
    if (contains(p->contains, red, blue))
        p->correct = true;
    else
    {
        p->lives--;
        p->correct = false;
    }
}
