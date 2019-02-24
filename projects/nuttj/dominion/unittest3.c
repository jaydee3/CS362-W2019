/**************************************************************************************
 * Unit test for numHandCards function
 **************************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

//Function prototypes
void testResult(int result, int expectedResult, char* message);

int main(int argc, const char* argv[])
{
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G; //G is the test game state
	int numPlayers = 3; //holds the number of players in the game
	int result; //holds the number of tests that failed
	int i; //counter
	int cardCount[] = {4, 8, 7, 3};

	printf("Performing unit test for numHandCards() function\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 110, &G);
	testResult(result, 0, "Function initializedGame failed");

	//set the number of cards for eah player to the corresponding element in the arrray
	for (i = 0; i < numPlayers; i++)
	{
		G.handCount[i] = cardCount[i]; 
	}

	//Test that each players card count is equal to the number from the array
	for (G.whoseTurn = 0; G.whoseTurn < MAX_PLAYERS; G.whoseTurn++)
	{
		//Test for each actual player
		if (G.whoseTurn < numPlayers)
		{
			if (numHandCards(&G) != cardCount[G.whoseTurn])
			{
				printf("TEST FAILED: Player %d: expected # cards %d  actual # cards %d\n", G.whoseTurn, cardCount[G.whoseTurn], numHandCards(&G));
				result += 1;
			}
		}
		//Positions for players not in the game should be set to 0
		else if (G.whoseTurn < MAX_PLAYERS && numHandCards(&G) != 0)
		{
			printf("TEST FAILED: Player %d is not an actual player: expected # cards 0 actual # cards %d\n", G.whoseTurn, numHandCards(&G));
			result += 1;
		}
	}

	if(result == 0)
		printf("All tests passed\n\n");
	else
		printf("%d tests failed\n\n", result);	


	return 0;
}

/**********************************************************
 * Test the test result to see if it is equal to expected. 
 * Prints error message and exits program if value is not
 * equal to expected result
 ********************************************************/
void testResult(int result, int expectedResult, char* message)
{
	if(result != expectedResult)
	{
		printf("%s\n", message); //print failure message
		exit(1); //exit program with error code;
	}
}
