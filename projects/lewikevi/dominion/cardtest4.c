/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 * Include the following lines in your makefile:
 *
 * cardtest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 ----------------------------------------------------------------------- */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

int main() {
    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;
    int assertTotal = 0;
    int deckCount = 0;
    int discardCount = 0;
    int j;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    // Set up the initial game state so that the player has all smithies everywhere
    
    for (int i = 0; i < 5; i++){
        G.hand[thisPlayer][i] = smithy;
    }
    
    G.deckCount[thisPlayer] = 10;
    G.discardCount[thisPlayer] = 10;
    for (j = 0; j < 10; j++)
    {
        
        G.deck[thisPlayer][j] = smithy;
    }
    
    for (j = 0; j < 10; j++)
    {
        G.discard[thisPlayer][j] = smithy;
    }

	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

	// ----------- TEST 1: +3 cards --------------
	printf("TEST 1: = +3 cards\n");

	// copy the game state to a test case
	memcpy(&testG, &G, sizeof(struct gameState));
	choice1 = 1;
    
    // Determine how much should have been discarded / how many cards are expected in the deck
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

    newCards = 3;
    if(G.deckCount[thisPlayer] < newCards)
    {
        deckCount = G.deckCount[thisPlayer] + G.discardCount[thisPlayer] - newCards;
        discardCount = discarded;
    } else{
        deckCount = G.deckCount[thisPlayer] - newCards;
        discardCount = G.discardCount[thisPlayer] + discarded;
    }
    
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
    printf("coins = %d, expected = %d\n", testG.coins, G.coins + xtraCoins) ;
    assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
    assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
    assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);

	// ----------- TEST 2: = +3 cards and no deck --------------
    printf("\nTEST 2: = +3 cards and no deck and 10 cards in discard\n");

	// copy the game state to a test case
    memcpy(&testG, &G, sizeof(struct gameState));
    choice1 = 1;
    
    testG.deckCount[thisPlayer] = 0;
    //Reset the players deck and set the discard pile
    for (j = 0; j < 10; j++)
    {
        testG.deck[thisPlayer][j] = -1;
    }
    
    // Determine what the resultant deck and discard counts should be

    deckCount = testG.discardCount[thisPlayer] - (newCards - testG.deckCount[thisPlayer]);
    discardCount = 1;
    
    
	cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);

    newCards = 3;
    
    printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
    printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
    printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
    assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
    assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
    assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);

    if(assertTotal == 0){
        printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
    }
    else{
        printf("\n >>>>> FAILURE: Testing complete %s <<<<<\n\n", TESTCARD);
    }

	return 0;
}


