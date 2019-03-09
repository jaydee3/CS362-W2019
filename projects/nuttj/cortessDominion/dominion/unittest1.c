/***************************************************************************************
 * Unit test for supplyCount() function
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
	struct gameState G; //G is the test game state
	int result; //holds result of tested functions
	int x[] = {-1, 0, 3, 5, 6, 7, 10, 11, 13, 14}; //holds supply count to be assigned to cards	

	printf("Performing unit test for supplyCount()\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(2, k, 100, &G);
	testResult(result, 0, "Function initializedGame failed");

	//Set supply count for each card
	G.supplyCount[adventurer] = x[0];
	G.supplyCount[council_room] = x[1];
	G.supplyCount[feast] = x[2];
	G.supplyCount[gardens] = x[3];
	G.supplyCount[mine] = x[4];
	G.supplyCount[remodel] = x[5];
	G.supplyCount[smithy] = x[6];
	G.supplyCount[village] = x[7];
	G.supplyCount[baron] = x[8];
	G.supplyCount[great_hall] = x[9];
	G.supplyCount[province] = x[0];
	G.supplyCount[gold] = x[1];

	//test all kingdom cards
	result = supplyCount(adventurer, &G);
	testResult(result, x[0], "Testing supply count for adventurer. Test Failed");
	result = supplyCount(council_room, &G);
	testResult(result, x[1], "Testing supply count for council_room. Test Failed");
	result = supplyCount(feast, &G);
	testResult(result, x[2], "Testing supply count for feast. Test Failed");
	result = supplyCount(gardens, &G);
	testResult(result, x[3], "Testing supply count for gardens. Test Failed");
	result = supplyCount(mine, &G);
	testResult(result, x[4], "Testing supply count for mine. Test Failed");
	result = supplyCount(remodel, &G);
	testResult(result, x[5], "Testing supply count for remodel. Test Failed");
	result = supplyCount(smithy, &G);
	testResult(result, x[6], "Testing supply count for smithy. Test Failed");
	result = supplyCount(village, &G);
	testResult(result, x[7], "Testing supply count for village. Test Failed");
	result = supplyCount(baron, &G);
	testResult(result, x[8], "Testing supply count for baron. Test Failed");
	result = supplyCount(great_hall, &G);
	testResult(result, x[9], "Testing supply count for great_hall. Test Failed");
	//Test some base cards
	result = supplyCount(province, &G);
	testResult(result, x[0], "Testing supply count for province. Test Failed");
	result = supplyCount(gold, &G);
	testResult(result, x[1], "Testing supply count for gold. Test Failed");
	//Test a kingdom card that have not been selected for the game
	result = supplyCount(steward, &G); //cards not in game are initialized to -1
	testResult(result, -1, "Testing supply count for steward. Test Failed");

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
