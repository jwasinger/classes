#include <stdio.h>

#include "../dominion.h"

int whoseTurn(struct gameState *state) {
  return state->whoseTurn;
}

int test_whose_turn(void)
{
  int my_whoseTurn = 1;
  struct gameState *state = malloc(sizeof(struct gameState));
  state->whoseTurn = my_whoseTurn;
  
  if(whoseTurn(state) == my_whoseTurn)
    return 1;

  return 0;
}

int main(int argc, char **argv)
{
    if(test_whose_turn())
      printf("test_new_turn() successful!\n");
    else
      printf("test_new_turn() failed!\n");

    return 0;
}
