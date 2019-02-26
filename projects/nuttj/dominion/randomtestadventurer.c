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
int treasureCount(int pile[], int size);

int main(int argc, const char* argv[])
{
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G, I; //G is the test game state, I is the initial game state that is used to test against changes in G
	int numPlayers = 4; //holds the number of players in the game
	int result; //holds the number of tests that failed
	int handPos = -1; //the position the card will be played from
	int player = 0; //the player number
	int i, j; //counters
	int deckTreasure = 0; //hold the number of treasures in the deck
	int discardTreasure = 0; //hold the number of treasures in the discard pile
	int treasure; //holds the total count of treasures that should be added to the players hand
	int treasurePos = 0; //This will hold the positions of the second treasure cards in the player's deck
	int card = adventurer; //this is the card being tested
	int drawn; //holds number of cards drawn

	srand(time(0)); 

	printf("Performing card test for Adventurer\n");
	
	//initialize game (numplayers, kingdomCards[10], randomSeed, gamestate)
	result = initializeGame(numPlayers, k, 110, &I);
	testResult(result, 0, "Function initializedGame failed");
	//I.hand[player][handPos] = card; //add the card to the player's hand position
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

/*	//find the position of the second treasure card from the top in the player's deck
	for (i = G.deckCount[player] - 1, j = 0; i >= 0  && j < 2; i--)
	{
		if(G.deck[player][i] == copper || G.deck[player][i] == silver || G.deck[player][i] == gold)
		{
			treasurePos = i; //save the deck position of the treasure
			j++; //increase the count of the found treasure cards
		}

	}
*/
	//See if there are at least two treasures in the deck
	for (i = G.deckCount[player] - 1; i >= 0 && deckTreasure < 2; i--)
	{
		if(G.deck[player][i] == copper || G.deck[player][i] == silver || G.deck[player][i] == gold)
			deckTreasure++; //increase the count of treasure in the deck
	}

	//If tow treasures were not found in the deck, see if two treasure can be reached in the discard pile
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

	printf("TREASURE: deck: %d   discard pile: %d  drawn = %d\n" , deckTreasure, discardTreasure, drawn);

	//Test the number of cards in the players hand 
	if (G.handCount[player] != I.handCount[player] + treasure - 1) { 
		printf("TEST FAILED: Net gain in player's hand count should be %d: %d drawn treasure cards and 1 discarded Adventurer Card\n", treasure - 1, treasure);
		printf("  Starting handCount: %d\n", I.handCount[player]);
		printf("  Ending handCount: %d\n", G.handCount[player]);
		result += 1; //increase the count of failed tests by 1:
	}



	if (deckTreasure == 2){
		if (G.discardCount[player] != I.discardCount[player] + drawn - treasure) { //all the drawn cards except the drawntreasure cards should have been discarded
			printf("TEST FAILED: Number of discarded cards should be the original discard count plus number of cards drawn - 2 treasure cards\n");
			printf("  Expected # of discarded cards: %d\n", I.discardCount[player] + drawn - 2);
			printf("  discardCount: %d\n", G.discardCount[player]);
			result += 1; //increase the count of failed tests by 1
		}
		if (G.deckCount[player] != I.deckCount[player] - drawn) { //All the cards in the deck up to the second treasure card should have been drawn
			printf("TEST FAILED: The expected number of cards were not drawn from the deck\n");
			printf("  Expected deckCount: %d\n", I.deckCount[player] - drawn);
			printf("  Ending deckCount: %d\n", G.deckCount[player]);
			result += 1; //increase the count of failed tests by 1
		}		
	}
	else
	{
		if (G.discardCount[player] != drawn - treasure) { //all the drawn cards except the drawntreasure cards should have been discarded
			printf("TEST FAILED: Number of discarded cards should be the number of cards drawn - 2 treasure cards\n");
			printf("  Expected # of discarded cards: %d\n", drawn - 2);
			printf("  discardCount: %d\n", G.discardCount[player]);
			result += 1; //increase the count of failed tests by 1
		}
		if (G.deckCount[player] != I.deckCount[player] + I.discardCount[player] - drawn) { //discard should have been shuffled into the deck
			printf("TEST FAILED: The expected number of cards were not drawn from the deck\n");
			printf("  Expected deckcount: %d\n", I.deckCount[player]+I.discardCount[player] - drawn);
			printf("  Ending deckCount: %d\n", G.deckCount[player]);
			result += 1; //increase the count of failed tests by 1
	}		
	}

	if (G.playedCardCount != I.playedCardCount + 1) { //The tested card should be in the play pile
		printf("TEST FAILED: Number of played cards should hve increased by 1\n");
		printf("  Expected # of played cards: %d\n", I.playedCardCount + 1);
		printf("  playedCardCount: %d\n", G.playedCardCount);
		result += 1; //increase the count of Tfailed tests by 1
	}

	if (G.playedCards[G.playedCardCount -1] != card) { //the tested card should be on top of the played card pile
		printf("TEST FAILED: Adventurer is not on top of the played card pile\n");
		result += 1; //increase the count of failed tests by 1
	}

	//Test that there are two less treasures in the player's deck
	if (treasureCount(G.deck[player], G.deckCount[player]) != treasureCount(I.deck[player], I.deckCount[player]) - 2) {
		printf("TEST FAILED: The number of treasures in the player's deck should have decreased by 2\n");
		result += 1; //increase the count of failed tests by 1
	}

	//Test that there are two more treasure's in the player's hand
	if (treasureCount(G.hand[player], G.handCount[player]) != treasureCount(I.hand[player], I.handCount[player]) + 2) {
		printf("TEST FAILED: The number of treasures in the player's hand should have increased by 2\n");
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
 * Gets the number of treasure cards in an array
 ********************************************************/
int treasureCount(int pile[], int size)
{
	int i; //counter
	int count = 0; //count of treasure cards in pile
	for (i = 0; i < size; i++)
	{
		if(pile[i] == copper || pile[i] == silver || pile[i] == gold)
			count++;
	}
	
	return count;
}

