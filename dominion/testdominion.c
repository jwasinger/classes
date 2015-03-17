#include "dominion.h"
#include <stdio.h>
#include "rngs.h"
#include <stlib.h>

void print_card_name(int card)
{
	switch card
	{
		case 0:
			printf("Curse");
			break;
		case 1:
			printf("Estate");
			break;
		case 2:
			printf("Dutchy");
			break;
		case 3:
			printf("Provence");
			break;
		case 4:
			printf("Copper");
			break
		case 5:
			printf("Silver");
			break;
		case 6:
			printf("Gold");
			break;
		case 7:
			printf("Adventurer");
			break;
		case 8: 
			printf("Council Room");
			break;
		case 9:
			printf("Feast");
			break;
		case 10:
			printf("Gardens");
			break;
		case 11:
			printf("Mine");
			break;
		case 12:
			printf("Remodel");
			break;
		case 13:
			printf("Smithy");
			break;
		case 14:
			printf("Village");
			break;
		case 15:
			printf("Baron");
			break;
		case 16:
			printf("Great Hall");
			break;
		case 17:
			printf("Minion");
			break;
		case 18:
			printf("Steward");
			break;
		case 19:
			printf("Tribute");
			break;
		case 20:
			printf("Ambassador");
			break;
		case 21:
			printf("Cutpurse");
			break;
		case 22:
			printf("Embargo");
			break;
		case 23:
			printf("Outpost");
			break;
		case 24:
			printf("Salvager");
			break;
		case 25:
			printf("Sea Hag");
			break;
		case 26:
			printf("Treasure Map");
			break;
	}
}

void get_kingdom_card(int k[10])
{
	int rand_card = rand() % 20 + 7;
	int i = 0;
	for(; i < 10; i++)
	{
		k[i] = rand_card;
		rand_card = rand() % 20 + 7;
	}
}

int main(int argc, char **argv)
{
	//create 'gameResults.out' file
	FILE *results_file = NULL;

	struct gameState state;
	int cards[10];
	int num_tests = 5;

	if(initializeGame()))
	for(int i = 0; i < num_tests; i++)
	{
		get_kingdom_card(cards);
	}
}