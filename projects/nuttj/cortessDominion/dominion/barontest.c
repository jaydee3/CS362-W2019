/**************************************************************************************
 * Card test for Baron card
 * The Baron card increases the number of Buys by one and allows the player to discard
 * an Estate for 4 coins, and gain an Estate if they don't
 **************************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

//Function prototypes
void testResult(int result, int expectedResult, char* message);
int estateCount(int pile[], int size);

int main(int argc, const char* argv[])
{
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, I; //G is the test game state, I is the initial game state that is used to test against changes in G
	int numPlayers = 4; //holds the number of players in the game
	int result = 0; //holds the number of tests that failed
	int handPos = 0; //the position the card will be played from
	int player = 0; //the player number
	int i, j; //counters
	int card = baron; //this is the card being tested

	/**********************************************************************************************************************
	 *********************************************************************************************************************/
	printf("Performing card test for Baron with options: Player discards Estate  Not holding Estate  No Estate in supply\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 110, &I);
	testResult(result, 0, "Function initializedGame failed");
	I.hand[player][handPos] = card; //add the card to the player's hand position
	I.whoseTurn = player;//set the current player
	//If the player is not holding an estate, change it to gold
	for (i = 0; i < I.handCount[player]; i++){
		if(I.hand[player][i] == estate)
			I.hand[player][i] = gold;
	}
	//change the last card to an estate, then decrement hand count by one, so the card in the hand
	//array after the last players card is now an estate
	I.hand[player][I.handCount[player] - 1] = estate;
	I.handCount[player]--;

	I.supplyCount[estate] = 0;


	memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

	//Play card from player's hand, electing a discard and estate for four coins
	cardEffect(card, 1, 0, 0, &G, handPos, 0); //play the card from the player's hand
 
	//Number of buys should have increased by one
	if (G.numBuys != I.numBuys + 1) {
		printf("TEST FAILED: Baron did not increase the number of buys by 1\n");
		printf("  Starting Buys: %d\n", I.numBuys);
		printf("  Ending Buys: %d\n", G.numBuys);
		result += 1; //increase the count of failed tests by 1
	}

	//Baron and Estate should have been played
	if (G.handCount[player] != I.handCount[player] - 1) { 
		printf("TEST FAILED: Net loss in player's hand count should be 1: Played Baron card\n");
		printf("  Starting handCount: %d\n", I.handCount[player]);
		printf("  Ending handCount: %d\n", G.handCount[player]);
		result += 1; //increase the count of failed tests by 1
	}

	//No cards should have been drawn
	if (G.deckCount[player] != I.deckCount[player]) { 
		printf("TEST FAILED: The ended deck count should be equal to the starting deck count\n");
		printf("  Starting deckCount: %d\n", I.deckCount[player]);
		printf("  Ending deckCount: %d\n", G.deckCount[player]);
		result += 1; //increase the count of failed tests by 1
	}		

	//The played card should be in the played card pile
	if (G.playedCardCount != I.playedCardCount + 1) { 
		printf("TEST FAILED: Number of played cards should have increased by 1\n");
		printf("  Expected # of played cards: 1\n");
		printf("  playedCardCount: %d\n", G.playedCardCount);
		result += 1; //increase the count of failed tests by 1
	}

	//The played cards should be on the top of the played card pile
	if (G.playedCards[G.playedCardCount -1] != card) {
		printf("TEST FAILED: Baron is not on top of the played card pile\n");
		result += 1; //increase the count of failed tests by 1
	}

	//No card should ahve been discarded
	if (G.discardCount[player] != I.discardCount[player]) { 
		printf("TEST FAILED: No cards should have been discarded\n");
		printf("  Expected # of discarded cards: %d\n", I.discardCount[player]);
		printf("  discardCount: %d\n", G.discardCount[player]);
		result += 1; //increase the count of failed tests by 1
	}
	
	//Their should be the same number of estates in the supply
	if (G.supplyCount[estate] != I.supplyCount[estate]) {
		printf("TEST FAILED: Starting estate supply is not equal to endin Estate supply\n");
		printf("  Starting Estate supply: %d\n", I.supplyCount[estate]);
		printf("  Ending Estate supply: %d\n", G.supplyCount[estate]);
		result += 1; //increase the count of failed tests by 1
	}

	//Perform test to see if the other player's hands have been altered
	for(i = 1; i < numPlayers; i++)
	{
		//test if the count of cards in the player's hand has changed
		if(G.handCount[i] != I.handCount[i])
		{
			printf("TEST FAILED: Player %d's hand count has been altered\n", i);\
			result += 1;
		}
		
		//test if the player is holding different cards in their hand postions
		for(j = 0; j < I.handCount[i]; j++)
		{
			if(G.hand[i][j] != I.hand[i][j])
			{
				printf("TEST FAILED: Player %d has an incorrect card at hand position %d\n", i, j);
				result += 1;
				break;
			}
		}

		//test if the count of cards in the player's deck has changed
		if(G.deckCount[i] != I.deckCount[i])
		{
			printf("TEST FAILED: Player %d's deck count has been altered\n", i);
			result += 1;
		}
		
		//test if the player's card has different cards in the deck postions
		for(j = 0; j < I.deckCount[i]; j++)
		{
			if(G.deck[i][j] != I.deck[i][j])
			{
				printf("TEST FAILED: Player %d has an incorrect card at deck position %d\n", i, j);
				result += 1;
				break;
			}
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

/**********************************************************
 * Gets the number of estate cards in an array
 ********************************************************/
int estateCount(int pile[], int size)
{
	int i; //counter
	int count = 0; //count of treasure cards in pile
	for (i = 0; i < size; i++)
	{
		if(pile[i] == estate)
			count++;
	}
	
	return count;
}
