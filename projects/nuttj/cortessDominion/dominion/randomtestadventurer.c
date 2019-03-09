/**************************************************************************************
 * Card test for Adventurer card
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
int treasureCount(struct gameState state, int player);
int treasureHand(struct gameState state, int player);

int main(int argc, const char* argv[])
{
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, I; //G is the test game state, I is the initial game state that is used to test against changes in G
	int numPlayers = 4; //holds the number of players in the game
	int result; //holds the number of tests that failed
	int handPos = -1; //the position the card will be played from
	int player = 0; //the player number
	int i, j, m; //counters
	int deckTreasure = 0; //hold the number of treasures in the deck
	int discardTreasure = 0; //hold the number of treasures in the discard pile
	int treasure; //holds the total count of treasures that should be added to the players hand
	int card = adventurer; //this is the card being tested
	int drawn; //holds number of cards drawn

	srand(time(0)); 

	printf("Performing card test for Adventurer\n");

	//This is the loop that performs the test
	for(m = 1; m <= 20; m++)
	{
		result = 0;
		handPos = -1;
		deckTreasure = 0;
		discardTreasure = 0;
		drawn = 0;		

		//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
		result = initializeGame(numPlayers, k, rand(), &I);
		testResult(result, 0, "Function initializedGame failed");
		I.whoseTurn = player;//set the current player

		//set a handcount between one and twenty and populate it with random cards
		I.handCount[player] = rand() % 20 + 1; 
		for (i = 0; i < I.handCount[player]; i++)
		{
			I.hand[player][i] = rand() % 27;
			if(I.hand[player][i] == adventurer) {
				handPos = i; //save position of adventurer
			}
		}
		//if an adventurer is not in the players hand, assign it to a random position
		if(handPos < 0){
			handPos = rand() % I.handCount[player];
			I.hand[player][handPos] = adventurer;	
		}

		//set a deckcount between one and twenty and populate it with random cards
		I.deckCount[player] = rand() % 20 + 1; 
		for (i = 0; i < I.deckCount[player]; i++) {
			I.deck[player][i] = rand() % 27;
		}
		
		//set a discard count between one and twenty and populate it with random cards
		I.discardCount[player] = rand() % 20 + 1; 
		for (i = 0; i < I.discardCount[player]; i++) {
			I.discard[player][i] = rand() % 27;
		}
		
		memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

		//See if there are at least two treasures in the deck
		for (i = G.deckCount[player] - 1; i >= 0 && deckTreasure < 2; i--)
		{
			if(G.deck[player][i] == copper || G.deck[player][i] == silver || G.deck[player][i] == gold)
				deckTreasure++; //increase the count of treasure in the deck
		}

		//If two treasures were not found in the deck, see if two treasure can be reached in the discard pile
		for (i = G.discardCount[player] - 1; i >= 0 && deckTreasure + discardTreasure < 2; i--)
		{
			if(G.discard[player][i] == copper || G.discard[player][i] == silver || G.discard[player][i] == gold)
				discardTreasure++; //increase the count of treasure in the discard pile
		}

		//Get the total count of treasure
		treasure = deckTreasure + discardTreasure;

		cardEffect(card, 0, 0, 0, &G, handPos, 0); //play the card from the player's hand

		//get the number of drawn cards
		if (deckTreasure == 2)
			drawn = I.deckCount[player] - G.deckCount[player];
		else
			drawn = I.deckCount[player] + I.discardCount[player] - G.deckCount[player];

		printf("Test %d: treasure in deck: %d   treasure in discard: %d  total card drawn = %d\n" , m, deckTreasure, discardTreasure, drawn);

		//Test the number of cards in the players hand 
		if (G.handCount[player] != I.handCount[player] + treasure - 1) { 
			printf("TEST FAILED: Net gain in player's hand count should be %d: %d drawn treasure cards and 1 discarded Adventurer Card", treasure - 1, treasure);
			printf("  Starting handCount: %d", I.handCount[player]);
			printf("  Ending handCount: %d\n", G.handCount[player]);
			result += 1; //increase the count of failed tests by 1:
		}

		//if there ate two treasures in the deck
		if (deckTreasure == 2){
			if (G.discardCount[player] != I.discardCount[player] + drawn - treasure) { //all the drawn cards except the drawntreasure cards should have been discarded
				printf("TEST FAILED: Number of discarded cards should be the original discard count plus number of cards drawn - 2 treasure cards");
				printf("  Expected #: %d", I.discardCount[player] + drawn - 2);
				printf("  discardCount: %d\n", G.discardCount[player]);
				result += 1; //increase the count of failed tests by 1
			}
			if (G.deckCount[player] != I.deckCount[player] - drawn) { //All the cards in the deck up to the second treasure card should have been drawn
				printf("TEST FAILED: The expected number of cards were not drawn from the deck");
				printf("  Expected deckCount: %d", I.deckCount[player] - drawn);
				printf("  Ending deckCount: %d\n", G.deckCount[player]);
				result += 1; //increase the count of failed tests by 1
			}		
		}
		else //the discard has to be shuffled in the deck
		{
			if (G.discardCount[player] != drawn - treasure) { //all the drawn cards except the drawntreasure cards should have been discarded
				printf("TEST FAILED: Number of discarded cards should be the number of cards drawn - 2 treasure cards");
				printf("  Expected # of discarded cards: %d", drawn - 2);
				printf("  discardCount: %d\n", G.discardCount[player]);
				result += 1; //increase the count of failed tests by 1
			}
			if (G.deckCount[player] != I.deckCount[player] + I.discardCount[player] - drawn) { //discard should have been shuffled into the deck
				printf("TEST FAILED: The expected number of cards were not drawn from the deck");
				printf("  Expected deckcount: %d", I.deckCount[player]+I.discardCount[player] - drawn);
				printf("  Ending deckCount: %d\n", G.deckCount[player]);
				result += 1; //increase the count of failed tests by 1
		}		
		}

		if (G.playedCardCount != I.playedCardCount + 1) { //The tested card should be in the play pile
			printf("TEST FAILED: Number of played cards should hve increased by 1");
			printf("  Expected # of played cards: %d", I.playedCardCount + 1);
			printf("  playedCardCount: %d\n", G.playedCardCount);
			result += 1; //increase the count of Tfailed tests by 1
		}

		if (G.playedCards[G.playedCardCount -1] != card) { //the tested card should be on top of the played card pile
			printf("TEST FAILED: Adventurer is not on top of the played card pile\n");
			result += 1; //increase the count of failed tests by 1
		}

		//Test that the treasures have been removed fromn the player's deck and discard
		if (treasureCount(G, player) != treasureCount(I, player) - treasure) {
			printf("TEST FAILED: The number of treasures in the player's deck and discard should have decreased by %d\n", treasure);
			result += 1; //increase the count of failed tests by 1
		}

		//Test that the treasures have been added to the player's in the player's hand
		if (treasureHand(G, player) != treasureHand(I, player) + treasure) {
			printf("TEST FAILED: The number of treasures in the player's hand should have increased by %d\n", treasure);
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

/************************************************************
 * Gets the number of treasure cards in an deack and discard
 ***********************************************************/
int treasureCount(struct gameState state, int player)
{
	int i; //counter
	int count = 0; //count of treasure cards in pile
	for (i = 0; i < state.deckCount[player]; i++)
	{
		if(state.deck[player][i] == copper || state.deck[player][i] == silver || state.deck[player][i] == gold)
			count++;
	}
	for (i = 0; i < state.discardCount[player]; i++)
	{
		if(state.discard[player][i] == copper || state.discard[player][i] == silver || state.discard[player][i] == gold)
			count++;
	}
	
	return count;
}
/**********************************************************
 * Gets the number of treasure cards in a player's hand
 ********************************************************/
int treasureHand(struct gameState state, int player)
{
	int i; //counter
	int count = 0; //count of treasure cards in pile
	for (i = 0; i < state.handCount[player]; i++)
	{
		if(state.hand[player][i] == copper || state.hand[player][i] == silver || state.hand[player][i] == gold)
			count++;
	}
	
	return count;
}


