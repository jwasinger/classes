#include <stdio.h>

#include "dominion.h"

int main(int argc, char **argv)
{
    int cards[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,sea_hag, tribute, steward};

    struct gameState start;
    struct gameState after;

    initializeGame(3, cards, cards, &start);
    start.hand[0][0] = smithy;
    start = after;
    playCard(0,0,0,0,&after);
}


