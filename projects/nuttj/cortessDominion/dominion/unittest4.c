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
	int numPlayers = 4; //number of players in the game
	struct gameState G; //G is the test game state
	int result; //holds the number of tests that failed
	int i, j; //counter;
	int bonus; //used to test the bonus parameter in updateCoins

	printf("Performing unit test for updateCoins() function\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 110, &G);
	testResult(result, 0, "Function initializedGame failed");

	//replace all cards in estate
	for (i = 0; i < numPlayers; i++) 
	{
    		for(j = 0; j < G.handCount[i]; j++)
		G.hand[i][j] = estate;
	}

	G.hand[0][0] = copper;
	G.hand[0][1] = silver;
	G.hand[0][2] = silver;
	G.hand[0][4] = gold;
	bonus = 4;

	//Test updateCoins with coins and no bonus
	updateCoins(0, &G, 0);
	if(G.coins != 8)
	{
		printf("TEST FAILED: Update coins with coins and no bonus did not return expected value\n");
		result += 1;
	}

	//Test updateCoins without coins but with bonus (player 1 has no coins)
	updateCoins(1, &G, bonus);
	if(G.coins != bonus)
	{
		printf("TEST FAILED: Update coins without coins and with bonus did not return expected value\n");
		result += 1;
	}

	//Test updateCoins with coins and bonus
	updateCoins(0, &G, bonus);
	if(G.coins != 8 + bonus)
	{
		printf("TEST FAILED: Update coins with coins and bonus did not return expected value\n");
		result += 1;
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
