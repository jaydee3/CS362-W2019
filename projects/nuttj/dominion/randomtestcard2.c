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
#include <time.h>

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
	int i, j, m; //counters
	int card = baron; //this is the card being tested
	int hasEstate; //tells if the player is holding an estate or not
	int estateSupply; //tells if there is a an estate in the supply
	int choice; //holds whether the player chooses to discard an estate or draw one from the supply

	//Seed the random number generator 
	srand(time(0));

	/**********************************************************************************************************************
	 *********************************************************************************************************************/
	printf("Performing card test for Baron\n");

	//Testing loop
	for(m = 1; m <= 100; m++)
	{
		result = 0; //no tests have been faied
		handPos = -1; //player is not yet holding baron
		hasEstate = 0; //player is not holding estate
		
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
				handPos = i; //save position of adventurer
			}
			if(I.hand[player][i] == estate) {
				hasEstate = 1; //player is holding estate
			}
		}
		//if an baron is not in the players hand, assign it to a random position
		if(handPos < 0){
			handPos = rand() % I.handCount[player];
			I.hand[player][handPos] = card;
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

		//determine if there will be an estate in the supply
		estateSupply = rand() % 2;
		//assign the number of estate is supply
		if(estateSupply == 0)
			I.supplyCount[estate] = 0;
		//Determine in there will be between one and three estates in the supply
		else
			I.supplyCount[estate] = rand() % 3 + 1;

		//Determine whether player discards an estate or draws one from the supply
		choice = rand() % 2;


		//Copy the initialized game state to a test game state
		memcpy(&G, &I, sizeof(I)); //copy the initialized game state to a test game state

		//Play card from player's hand
		cardEffect(card, choice, 0, 0, &G, handPos, 0); //play the card from the player's hand
 
		printf("Test %d: ", m);
		if (choice == 0)
			printf("Player draws Estate from supply");
		else 
			printf("Player discards Estate");
		if (hasEstate == 0)
			printf("  Not holding Estate");
		else 
			printf("  Holding Estate");
		if (estateSupply == 0)
			printf("  No Estate in supply\n");
		else 
			printf("  Estate in Supply\n");


		//Number of buys should have increased by one
		if (G.numBuys != I.numBuys + 1) {
			printf("  TEST FAILED: Baron did not increase the number of buys by 1");
			printf("  Starting Buys: %d", I.numBuys);
			printf("  Ending Buys: %d\n", G.numBuys);
			result += 1; //increase the count of failed tests by 1
		}

		/***************************************************************************************************************/
		//If the player has elected to discard an estate while holding an estate
		if(choice == 1 && hasEstate == 1)
		{
			//Number of coins should have increased by 4
			if (G.coins != I.coins + 4) {
				printf("  TEST FAILED: Coins should have increased by 4");
				printf("  Starting coins: %d", I.coins);
				printf("  Ending coins: %d\n", G.coins);
				result += 1; //increase the count of failed tests by 1
			}

			//Baron and Estate should have been played
			if (G.handCount[player] != I.handCount[player] - 2) { 
				printf("  TEST FAILED: Net loss in player's hand count should be 2: Played Baron card and discarded Estate card");
				printf("  Starting handCount: %d", I.handCount[player]);
				printf("  Ending handCount: %d\n", G.handCount[player]);
				result += 1; //increase the count of failed tests by 1
			}

			//No cards should have been drawn
			if (G.deckCount[player] != I.deckCount[player]) { 
				printf("  TEST FAILED: The ended deck count should be equal to the starting deck count");
				printf("  Starting deckCount: %d", I.deckCount[player]);
				printf("  Ending deckCount: %d\n", G.deckCount[player]);
				result += 1; //increase the count of failed tests by 1
			}		

			//The played card should be in the played card pile
			if (G.playedCardCount != I.playedCardCount + 1) { 
				printf("  TEST FAILED: Number of played cards should have increased by 1");
				printf("  Expected # of played cards: 1");
				printf("  playedCardCount: %d\n", G.playedCardCount);
				result += 1; //increase the count of failed tests by 1
			}

			//The played cards should be on the top of the played card pile
			if (G.playedCards[G.playedCardCount -1] != card) {
				printf("  TEST FAILED: Baron is not on top of the played card pile\n");
				result += 1; //increase the count of failed tests by 1
			}

			//The estate card should have been discarded
			if (G.discardCount[player] != I.discardCount[player] + 1) { 
				printf("  TEST FAILED: Number of discarded cards should be 1");
				printf("  Expected count: %d", I.discardCount[player] + 1);
				printf("  discardCount: %d\n", G.discardCount[player]);
				result += 1; //increase the count of failed tests by 1
			}
			
			//The discarded card should be on the top of the played card pile
			if (G.discard[player][G.discardCount[player] - 1] != estate) {
				printf("  TEST FAILED: Estate is not on top of the discarded card pile\n");
				result += 1; //increase the count of failed tests by 1
			}

			//Their should be the same number of estates in the supply
			if (G.supplyCount[estate] != I.supplyCount[estate]) {
				printf("  TEST FAILED: Starting estate supply is not equal to ending Estate supply");
				printf("  Starting Estate supply: %d", I.supplyCount[estate]);
				printf("  Ending Estate supply: %d\n", G.supplyCount[estate]);
				result += 1; //increase the count of failed tests by 1
			}
		}// end if
		/**********************************************************************************************************************
		 *********************************************************************************************************************/
		//Performing card test for Baron with options: player draws an Estate or discards an estate when there are not 
		//estates in their hand, there are Estates in the supply pile
		else if(estateSupply == 1)
		{	
			//Baron should have been played and Estate should have been drawn
			if (G.handCount[player] != I.handCount[player]) { 
				printf("  TEST FAILED: Net loss in player's hand count should be 0: Played Baron card drew Estate card");
				printf("  Starting handCount: %d", I.handCount[player]);
				printf("  Ending handCount: %d\n", G.handCount[player]);
				result += 1; //increase the count of failed tests by 1
			}

			//No cards should have been drawn
			if (G.deckCount[player] != I.deckCount[player]) { 
				printf("  TEST FAILED: The ended deck count should be equal to the starting deck count");
				printf("  Starting deckCount: %d", I.deckCount[player]);
				printf("  Ending deckCount: %d\n", G.deckCount[player]);
				result += 1; //increase the count of failed tests by 1
			}		

			//The played card should be in the played card pile
			if (G.playedCardCount != I.playedCardCount + 1) { 
				printf("  TEST FAILED: Number of played cards should have increased by 1");
				printf("  Expected # of played cards: 1");
				printf("  playedCardCount: %d\n", G.playedCardCount);
				result += 1; //increase the count of failed tests by 1
			}

			//The played card should be on the top of the played card pile
			if (G.playedCards[G.playedCardCount -1] != card) {
				printf("  TEST FAILED: Baron is not on top of the played card pile\n");
				result += 1; //increase the count of failed tests by 1
			}

			//No cards should have been discarded
			if (G.discardCount[player] != I.discardCount[player]) { 
				printf("  TEST FAILED: Number of discarded cards should be equal to the starting number of discarded cards");
				printf("  Expected count: %d", I.discardCount[player]);
				printf("  discardCount: %d\n", G.discardCount[player]);
				result += 1; //increase the count of failed tests by 1
			}

			//Their should be 1 less estate in the supply
			if (G.supplyCount[estate] != I.supplyCount[estate] - 1) {
				printf("  TEST FAILED: Ending Estate supply should be 1 less than starting Estate supply");
				printf("  Starting Estate supply: %d", I.supplyCount[estate]);
				printf("  Ending Estate supply: %d\n", G.supplyCount[estate]);
				result += 1; //increase the count of failed tests by 1
			}
		}//endif

		/**********************************************************************************************************************
		 *********************************************************************************************************************/
		//Performing card test for Baron with options: player draws an Estate or discards an estate when they are not holding
		//an estate, and there are no Estates in the supply pile\n
		else if (estateSupply == 0)
		{
		//Baron and should have been played
		if (G.handCount[player] != I.handCount[player] - 1) { 
			printf("  TEST FAILED: Net loss in player's hand count should be 1: Played Baron card");
			printf("  Starting handCount: %d", I.handCount[player]);
			printf("  Ending handCount: %d\n", G.handCount[player]);
			result += 1; //increase the count of failed tests by 1
		}

		//No cards should have been drawn
		if (G.deckCount[player] != I.deckCount[player]) { 
			printf("  TEST FAILED: The ended deck count should be equal to the starting deck count");
			printf("  Starting deckCount: %d", I.deckCount[player]);
			printf("  Ending deckCount: %d\n", G.deckCount[player]);
			result += 1; //increase the count of failed tests by 1
		}		

		//The played card should be in the played card pile
		if (G.playedCardCount != I.playedCardCount + 1) { 
			printf("  TEST FAILED: Number of played cards should have increased by 1");
			printf("  Expected # of played cards: 1");
			printf("  playedCardCount: %d\n", G.playedCardCount);
			result += 1; //increase the count of failed tests by 1
		}

		//The played card should be on the top of the played card pile
		if (G.playedCards[G.playedCardCount -1] != card) {
			printf("  TEST FAILED: Baron is not on top of the played card pile\n");
			result += 1; //increase the count of failed tests by 1
		}

		//No cards should have been discarded
		if (G.discardCount[player] != I.discardCount[player]) { 
			printf("  TEST FAILED: Number of discarded cards should be equal to the starting number of discarded cards");
			printf("  Expected count: %d", I.discardCount[player]);
			printf("  discardCount: %d\n", G.discardCount[player]);
			result += 1; //increase the count of failed tests by 1
		}

		//There should be no change in the estate supply
		if (G.supplyCount[estate] != I.supplyCount[estate]) {
			printf("  TEST FAILED: Ending Estate supply should the same as starting Estate supply");
			printf("  Starting Estate supply: %d", I.supplyCount[estate]);
			printf("  Ending Estate supply: %d\n", G.supplyCount[estate]);
			result += 1; //increase the count of failed tests by 1
		}
		}//end if

		//Perform test to see if the other player's hands have been altered
		for(i = 1; i < numPlayers; i++)
		{
			//test if the count of cards in the player's hand has changed
			if(G.handCount[i] != I.handCount[i])
			{
				printf("  TEST FAILED: Player %d's hand count has been altered\n", i);\
				result += 1;
			}
			
			//test if the player is holding different cards in their hand postions
			for(j = 0; j < I.handCount[i]; j++)
			{
				if(G.hand[i][j] != I.hand[i][j])
				{
					printf("  TEST FAILED: Player %d has an incorrect card at hand position %d\n", i, j);
					result += 1;
					break;
				}
			}

			//test if the count of cards in the player's deck has changed
			if(G.deckCount[i] != I.deckCount[i])
			{
				printf("  TEST FAILED: Player %d's deck count has been altered\n", i);
				result += 1;
			}
			
			//test if the player's card has different cards in the deck postions
			for(j = 0; j < I.deckCount[i]; j++)
			{
				if(G.deck[i][j] != I.deck[i][j])
				{
					printf("  TEST FAILED: Player %d has an incorrect card at deck position %d\n", i, j);
					result += 1;
					break;
				}
			}
		}
		if(result == 0)
			printf("All tests passed\n\n");
		else
			printf("%d tests failed\n\n", result);
	}//end for loop
	
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
