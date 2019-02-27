/***************************************************************************************
 * Card test for Smithy card
 **************************************************************************************/
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"
#include <time.h>

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
	int i, j, m; //counters
	int card = smithy; //the card that is being tested
	int drawn; //number of cards that will be drawn from deck and discard

	srand(time(0)); //seed random number generator

	printf("Performing card test for Smithy\n");
	for(m = 1; m <= 100; m++)
	{
		result = 0;
		handPos = -1;

		//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
		result = initializeGame(numPlayers, k, rand(), &I);
		testResult(result, 0, "Function initializedGame failed");
		I.whoseTurn = player;//set the current player

		//set a handcount between one and twenty and populate it with random cards
		I.handCount[player] = rand() % 20 + 1;
		for (i = 0; i < I.handCount[player]; i++)
		{
			I.hand[player][i] = rand() % 27;
			if(I.hand[player][i] == card) {
				handPos = i; //save position of smithy
			}
		}
		//if an smithy is not in the players hand, assign it to a random position
		if(handPos < 0){
			handPos = rand() % I.handCount[player];
			I.hand[player][handPos] = card;
		}

		//set a deckcount between zero and ten and populate it with random cards
		I.deckCount[player] = rand() % 11;
		for (i = 0; i < I.deckCount[player]; i++) {
			I.deck[player][i] = rand() % 27;
		}

		//set a discard count between zero and 10 and populate it with random cards
		I.discardCount[player] = rand() % 11;
		for (i = 0; i < I.discardCount[player]; i++) {
			I.discard[player][i] = rand() % 27;
		}
		
		//Get the number of cards that will be drawn
		drawn = I.deckCount[player] + I.discardCount[player];
		if(drawn > 3)
			drawn = 3;


		memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

		cardEffect(smithy, 0, 0, 0, &G, handPos, 0); //play the Smithy Card from the player's hand
		
		printf("Test %d    Starting deckCount: %d    Starting discardCount: %d\n", m, I.deckCount[player], I.discardCount[player]);

		//Test if the player is holding the proper number of cards
		if (G.handCount[player] != I.handCount[player] + drawn - 1) {
			printf("TEST FAILED: Smithy should increase number of cards in player's hands by %d: %d drawn cards minus discarded Smithy card\n", drawn, drawn);
			result += 1; //increase the count of failed tests by 1
		}

		//Test that the player drew three cards if the starting deck had at least three cards
		if (I.deckCount[player] >= 3 && (G.deckCount[player] != I.deckCount[player] - 3)) {
			printf("TEST FAILED: Smithy did not decrease the number of cards in player's deck by 3\n");
			result += 1; //increase the count of failed tests by 1
		}		
		//If the starting deck did not have three cards, then the discard had to be shuffled into the deck.
		//Test that the player drew the expected number of cards
		else if ((G.deckCount[player] + G.discardCount[player]) != (I.deckCount[player] + I.discardCount[player]) - drawn) {
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
		}
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
