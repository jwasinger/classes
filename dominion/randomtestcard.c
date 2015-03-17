#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_TESTS 400 

int main(int argc, char **argv)
{
  int cards[10] = {smithy, tribute, sea_hag, cutpurse, mine, minion, village, embargo, gardens, adventurer};
  int num_players = 0;
  struct gameState beginState;
  struct gameState endState;
  int i = 0;
  int seed = 0;
  int player = 0;
  int all_tests_passed = 1;
  int old_hand_count;
  int return_val = 0;

  //run NUM_TESTS tests
  for(i = 0; i < NUM_TESTS; i++)
  {
    num_players = rand() % 5;
    seed = rand();

    initializeGame(num_players, cards, seed, &beginState);
    beginState.deckCount[player] = rand() % MAX_DECK;
    beginState.discardCount[player] = 0;

    beginState.handCount[player] = rand() % MAX_HAND;
    
    endState = beginState;

    old_hand_count = beginState.handCount[player];

    //run smithy card; (the one without bugs)
    //smithy_card_effect_correct(smithy, 1, 1, &beginState, 0, NULL);
    return_val = cardEffect(smithy, 1, 1, 1, &beginState, 0, NULL);

    //test to see if the first card in the deck was discarded
    
    //first condition: the player's hand count should be 2 cards larger
    //because one card is discarded while three are drawn
    if(return_val == -1)
    {
      printf("smithy_card_effect returned an error value\n");
    }
    if(old_hand_count < beginState.handCount[player] - 2)
    {
      printf("test failed: drew too few cards\n");
      all_tests_passed = 0;
    }
    if(old_hand_count > beginState.handCount[player] - 2)
    {
      printf("test failed: drew too many cards\n");
      all_tests_passed = 0;
    }  
  }
  
  if(all_tests_passed)
  {
    printf("All tests have passed... congratulations!\n");
  }
  
  return 0;
}
