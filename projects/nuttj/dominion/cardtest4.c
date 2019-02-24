/**************************************************************************************
 * Card test for Council Room card
 * The Council Room gives +4 cards and +1 Buys allows the player to discard each of the
 * other players to draw a card
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
	int handPos = 0; //the position the Smithy card will be played from
	int player = 0; //the player number
	int i, j; //counters
	int card = council_room; //this is the card being tested

	/**********************************************************************************************************************
	 *********************************************************************************************************************/
	printf("Performing card test for Council Room\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 110, &I);
	testResult(result, 0, "Function initializedGame failed");
	I.hand[player][handPos] = card; //add the card to the player's hand position
	I.whoseTurn = player;//set the current player

	memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

	//Play card from player's hand, electing a discard and estate for four coins
	cardEffect(card, 0, 0, 0, &G, handPos, 0); //play the card from the player's hand
 
	//Number of buys should have increased by one
	if (G.numBuys != I.numBuys + 1) {
		printf("TEST FAILED: did not increase the number of buys by 1");
		printf("  Starting Buys: %d\n", I.numBuys);
		printf("  Ending Buys: %d\n", G.numBuys);
		result += 1; //increase the count of failed tests by 1
	}

	//Number of coins should not have increased
	if (G.coins != I.coins) {
		printf("TEST FAILED: Coins should have not have increased");
		printf("  Starting coins: %d\n", I.coins);
		printf("  Ending coins: %d\n", G.coins);
		result += 1; //increase the count of failed tests by 1
	}

	//Council room should have been played and 4 other cards drawn
	if (G.handCount[player] != I.handCount[player] + 3) { 
		printf("TEST FAILED: Net gain in player's hand count should be 3: Played Baron card and drew 4 new cardsd\n");
		printf("  Starting handCount: %d\n", I.handCount[player]);
		printf("  Ending handCount: %d\n", G.handCount[player]);
		result += 1; //increase the count of failed tests by 1
	}

	//4 cards should have been drawn
	if (G.deckCount[player] != I.deckCount[player] - 4) { 
		printf("TEST FAILED: The ending deck count should be 4 less than the starting deck count\n");
		printf("  Starting deckCount: %d\n", I.deckCount[player]);
		printf("  Ending deckCount: %d\n", G.deckCount[player]);
		result += 1; //increase the count of failed tests by 1
	}		

	//The played card should be in the played card pile
	if (G.playedCardCount != I.playedCardCount + 1) { 
		printf("TEST FAILED: Number of played cards should have increased by 1\n");
		printf("  Expected # of played cards: %d\n", I.playedCardCount + 1);
		printf("  playedCardCount: %d\n", G.playedCardCount);
		result += 1; //increase the count of failed tests by 1
	}

	//The played cards should be on the top of the played card pile
	if (G.playedCards[G.playedCardCount -1] != card) {
		printf("TEST FAILED: Council_Room is not on top of the played card pile\n");
		result += 1; //increase the count of failed tests by 1
	}

	//No cards should have been discarded
	if (G.discardCount[player] != I.discardCount[player]) { 
		printf("TEST FAILED: Number of ending discarded cards should be equal to the number of starting discarded cards\n");
		printf("  Expected # of discarded cards: %d\n", I.discardCount[player] + 1);
		printf("  discardCount: %d\n", G.discardCount[player]);
		result += 1; //increase the count of failed tests by 1
	}
	

	//Perform test to see if the other player's hands have been altered
	for(i = 1; i < numPlayers; i++)
	{
		//test if the count of cards in the player's hand has changed
		if(G.handCount[i] != I.handCount[i] + 1)
		{
			printf("TEST FAILED: Player %d's hand count should have increased by 1\n", i);\
			result += 1;
		}
		
		//test if the player is holding different cards in their hand postions
		for(j = 0; j < I.handCount[i] - 1; j++)
		{
			if(G.hand[i][j] != I.hand[i][j])
			{
				printf("TEST FAILED: Player %d has an incorrect card at hand position %d\n", i, j);
				result += 1;
				break;
			}
		}
	
		//Test the final cared in palyer's hand is the same as the card that was on the top of the deck	
		if(G.hand[i][G.handCount[i] - 1 ] != I.deck[i][I.deckCount[i] - 1])
		{
			printf("TEST FAILED: Player %d did not draw the expected card from the top of the deck%d\n", i, j);
			result += 1;
		}
		

		//test if the count of cards in the player's deck has changed
		if(G.deckCount[i] != I.deckCount[i] - 1)
		{
			printf("TEST FAILED: Player %d's deck count should have decreased by 1\n", i);\
			result += 1;
		}
		
		//test if the player's card has different cards in the deck postions
		for(j = 0; j < G.deckCount[i]; j++)
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

