/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
 ----------------------------------------------------------------------- */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <stdbool.h>

#define TESTCARD "adventurer"

int customAssert(bool test){
    if (test == false){
        printf(" -- FAILED\n");
        return 1;
    }
    else{
        printf(" ---- SUCCESS\n");
        return 0;
    }
}

int main() {
    int newCards = 0;
    int discarded = 1;
    int shuffledCards = 0;
    int assertTotal = 0;
    int deckCount = 0;
    int discardCount = 0;
    int i, j;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
    int testCards[3] = {copper, silver, gold};

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    // Set up the initial game state so that the player has all smithies everywhere
    
    for (i = 0; i < 5; i++){
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
    
    int countTwo = 0;
    
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for(int count = 0; count < 3; count++)
    {
        // ----------- TEST 1: +2 coin cards at top of deck --------------
        printf("\nTEST %d: = 2 %d cards at top of deck\n", countTwo, testCards[count]);
        countTwo++;
        
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Set up the deck such that the coppers are on top of the deck
        for (j = 0; j < 10; j++)
        {
            testG.deck[thisPlayer][j] = testCards[count];
        }
        
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);

        // Determine how much should have been discarded / how many cards are expected in the deck
        newCards = 2;
        discarded = 1;
        shuffledCards = 0;
        
        deckCount = G.deckCount[thisPlayer] - newCards - shuffledCards;
        discardCount = G.discardCount[thisPlayer] + discarded;
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);

        // ----------- TEST 2: 2 coin cards at bottom of deck cards --------------
        printf("\nTEST %d: = 2 coin cards at bottom of deck\n", countTwo);
        countTwo++;
        
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Set up the deck such that the coins are at the bottom of the deck
        testG.deckCount[thisPlayer] = 10;
        testG.discardCount[thisPlayer] = 10;
        for (j = 0; j < 2; j++)
        {
            //set the bottom two cards to coins
            testG.deck[thisPlayer][j] = testCards[count];
            
        }

        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        // Determine how much should have been discarded / how many cards are expected in the deck
        newCards = 2;
        discarded = 1;
        shuffledCards = 8;
        
        deckCount = G.deckCount[thisPlayer] - newCards - shuffledCards;
        discardCount = G.discardCount[thisPlayer] + discarded + shuffledCards;
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
        
        // ----------- TEST 3: 2 coin cards at top of discard --------------
        printf("\nTEST %d: = 2 coin cards at top of discard\n", countTwo);
        countTwo++;
        
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Set up the deck such that the coins are at the top of the discard
        testG.discard[thisPlayer][8] = testCards[count];
        testG.discard[thisPlayer][9] = testCards[count];
        
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        // Determine how much should have been discarded / how many cards are expected in the deck
        newCards = 2;
        discarded = 1;
        shuffledCards = 10;
        
        deckCount = 0;
        discardCount = G.deckCount[thisPlayer] + discarded;
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
        
        // ----------- TEST 4: 2 coin cards at bottom of discard --------------
        printf("\nTEST %d: = 2 coin cards at bottom of discard\n", countTwo);
        countTwo++;
        
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Set up the deck such that the coins are at the bottom of the discard
        for (j = 0; j < 10; j++)
        {
            //set the bottom two cards to coins
            testG.discard[thisPlayer][j] = testCards[count];
            
        }
        
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        // Determine how much should have been discarded / how many cards are expected in the deck
        newCards = 2;
        discarded = 1;
        shuffledCards = 10;
        
        deckCount = 0;
        discardCount = G.deckCount[thisPlayer] + G.discardCount[thisPlayer] - newCards + discarded;
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
        /*
        // ----------- TEST 5: No coin cards at bottom of discard --------------
        printf("\nTEST %d: = No coin cards at deck or discard\n", countTwo++);
        countTwo++;
        
        // copy the game state to a test case
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // The deck was initially set up with no coins in it
        
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        // Determine how much should have been discarded / how many cards are expected in the deck
        newCards = 0;
        discarded = 1;
        shuffledCards = G.deckCount[thisPlayer] + G.discardCount[thisPlayer];
        
        deckCount = 0;
        discardCount = shuffledCards + discarded;
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        assertTotal += customAssert(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
         */
    }
    if(assertTotal == 0){
        printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
    }
    else{
        printf("\n >>>>> FAILURE: Testing complete %s <<<<<\n\n", TESTCARD);
    }

	return 0;
}


