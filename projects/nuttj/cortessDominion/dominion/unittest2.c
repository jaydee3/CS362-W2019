/***************************************************************************************
 * Unit test for isGameOver() function
 **************************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

//Function prototypes
void testResult(int result, int expectedResult, char* message);

int main()
{
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, S; //G is the test game state, S = the default game state that is used to reset G after testing
	int i; //counter
	int result; //holds result of tested functions
	char buffer[100]; //holds failure messages

	printf("Performing unit test for isGameOver()\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(2, k, 100, &S);
	testResult(result, 0, "Function initializedGame failed");

	//Test that the newly initialized game is not over
	result = isGameOver(&S); //should return 0 to indicate game is not over
	testResult(result, 0, "Test to see if game was initialized to Game Over state. Test failed.");
	
	//Test if the empty Province supply causes the game to end
	memcpy(&G, &S, sizeof(S)); //copy the initialized game state to a test game state
	G.supplyCount[province] = 0; //set Province supply to 0
	result = isGameOver(&G); //should return 1 to indicate game is over
	testResult(result, 1, "Test to see if empty Province Supply causes Game Over. Test failed.");
	
	//Test if three empty supply piles causes the game to end
	memcpy(&G, &S, sizeof(S)); //copy the initialized game state to a test game state
	for (i = 1; i <= 2; i++) //test to see if 1 and 2 empty supply piles causes Game Over
	{
		G.supplyCount[i] = 0; //set supply to 0
		result = isGameOver(&G); 
		sprintf(buffer, "Test to see if %d Empty supply piles causes Game Over. Test failed.", i);
		testResult(result, 0, buffer); //isGameOver should return 0 to indicate game is not over
	}
	//test to see if 3 empty supply piles causes Game Over
		G.supplyCount[i] = 0; //set supply to 0. i will be currently equal to 3
		result = isGameOver(&G); 
		sprintf(buffer, "Test to see if %d Empty supply piles causes Game Over. Test failed.", i);
		testResult(result, 1, buffer); //isGameOver should return 1 to indicate game is over

	printf("All tests passed\n");

	return 0;
}

/**********************************************************
 *Test the test result to see if it is equal to expected. 
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
