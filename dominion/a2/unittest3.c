#include <stdio.h>
#include "../dominion.h"

int getCost(int cardNumber)
{
  switch( cardNumber ) 
    {
    case curse:
      return 0;
    case estate:
      return 2;
    case duchy:
      return 5;
    case province:
      return 8;
    case copper:
      return 0;
    case silver:
      return 3;
    case gold:
      return 6;
    case adventurer:
      return 6;
    case council_room:
      return 5;
    case feast:
      return 4;
    case gardens:
      return 4;
    case mine:
      return 5;
    case remodel:
      return 4;
    case smithy:
      return 4;
    case village:
      return 3;
    case baron:
      return 4;
    case great_hall:
      return 3;
    case minion:
      return 5;
    case steward:
      return 3;
    case tribute:
      return 5;
    case ambassador:
      return 3;
    case cutpurse:
      return 4;
    case embargo: 
      return 2;
    case outpost:
      return 5;
    case salvager:
      return 4;
    case sea_hag:
      return 4;
    case treasure_map:
      return 4;
    }
	
  return -1;
}

int test_get_cost(void)
{
    if(getCost(curse) != 0)
        return 0;
    else if(getCost(estate) != 2)
        return 0;
    else if(getCost(duchy) != 5)
        return 0;
    else if(getCost(province) != 8)
        return 0;
    else if(getCost(copper) != 0)
        return 0;    
    else if(getCost(silver) != 3)
        return 0;    
    else if(getCost(gold) != 6)
        return 0;
    else if(getCost(adventurer) != 6)
        return 0;
    else if(getCost(council_room) != 5)
        return 0;
    else if(getCost(feast) != 4)
        return 0;
    else if(getCost(gardens) != 4)
        return 0;    
    else if(getCost(mine) != 5)
        return 0;
    else if(getCost(remodel) != 4)
        return 0;
    else if(getCost(smithy) != 4)
        return 0;
    else if(getCost(village) != 3)
        return 0;
    else if(getCost(baron) != 4)
        return 0;
    else if(getCost(great_hall) != 3)
        return 0;
    else if(getCost(minion) != 5)
        return 0;    
    else if(getCost(steward) != 3)
        return 0;    
    else if(getCost(tribute) != 5)
        return 0;
    else if(getCost(ambassador) != 3)
        return 0;
    else if(getCost(cutpurse) != 4)
        return 0;
    else if(getCost(embargo) != 2)
        return 0;
    else if(getCost(outpost) != 5)
        return 0;    
    else if(getCost(salvager) != 4)
        return 0;
    else if(getCost(sea_hag) != 4)
        return 0;
    else if(getCost(treasure_map) != 4)
        return 0;
    else
        return 1;
}

int main(int argc, char **argv)
{
    if(test_get_cost())
    {
        printf("success!");
        return 0;
    }

    printf("error...");
    return -1;
}
