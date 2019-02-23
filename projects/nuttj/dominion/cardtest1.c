/***************************************************************************************
 * Card test for Smithy card
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
	struct gameState G, I; //G is the test game state, I is the initial game state that is used to test against changes in G
	int numPlayers = 4; //holds the number of players in the game
	int result; //holds the number of tests that failed
	int handPos = 0; //the position the Smithy card will be played from
	int player = 0; //the player number
	int i, j; //counters

	printf("Performing card test for Smithy\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 100, &I);
	testResult(result, 0, "Function initializedGame failed");
	I.hand[player][handPos] = smithy; //add the smithy card to the player's first hand position


	memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

	cardEffect(smithy,0, 0, 0, &G, handPos, 0); //play the Smithy Card from the player's hand
	if (G.handCount[player] != I.handCount[player] + 2) { //3 cards minus 1 discarded card equals 2 card in players hand
		printf("TEST FAILED: Smithy did not increase the number of cards in player's hands by 2\n");
		result += 1; //increase the count of failed tests by 1
	}

	if (G.deckCount[player] != I.deckCount[player] - 3) { //3 cards should have been drawn from the deck
		printf("TEST FAILED: Smithy did not decrease the number of cards in player's deck by 3\n");
		result += 1; //increase the count of failed tests by 1
	}		

	if (G.playedCardCount != I.playedCardCount + 1) { //1 card should have been discarded
		printf("TEST FAILED: Smithy did not increase the number of cards in player's played pile by 1\n");
		result += 1; //increase the count of failed tests by 1
	}

	if (G.playedCards[G.playedCardCount -1] != smithy) { //the smithy card should be on top of the played card pile
		printf("TEST FAILED: Smithy is not on top of the played card pile\n");
		result += 1; //increase the count of failed tests by 1
	}
	/*Three cards should have been drawn from the deck, so the third card in the deck would be the last card in the
	player's hand when the player discarded the smithy card.  This card in the last position would have been moved
	into the emplo position in the player's hand that was created by the discarded Smithy card after it was played.
	Test that the Smithy hand position now holds the third cards that was drawn from the deck*/
	if(G.hand[player][handPos] != I.deck[player][I.deckCount[player]-3]) {
		printf("TEST FAILED: The last card in the player's hand was not moved to the discarded Smithy postion\n");
		result += 1; //increase the count of failed tests by 1
	}
	
	//Perfomr test to see if the other player's hands have been altered
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
