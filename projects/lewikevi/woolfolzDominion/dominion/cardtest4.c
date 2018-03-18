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
#include <time.h>
#include <stdbool.h>

#define TESTCARD "smithy"

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
    
    srand(time(NULL)); // Seed the random number generator
    int newCards = 0;
    int discarded = 1;
    int assertTotal = 0;
    int deckCount = 0;
    int count = 0;
    int discardCount = 0;
    int handCount, smithyIndex;
    int deckResult, handResult, discardResult, isInHand;
    int i;
    int drawnCards;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int bothCoins;
    int thisPlayer = 0; // Only Need to test player one
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy, council_room};
    int nTestCards = 13;
    int testCards[13] = {embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, adventurer, council_room, copper, silver, gold, smithy};
    
    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

    count = 0;
    
    while(count < 10)
    {
        count++;
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        drawnCards = 3; // we will be drawing 3 cards
        // Randomly Choose the number of cards in the deck, discard, and hand
        
        handCount = 1 + rand() % 10;
        deckCount = rand() % 10;
        discardCount = rand() % 10;
        
        // Now determine what the correct output should be
        discardResult = discardCount + 1; //smithy
        deckResult = deckCount;
        handResult = handCount - 1; //smithy
        bothCoins = 0;
        
        if(deckCount < 3)
        {
            if(deckCount + discardCount < 3)
            {
                deckResult = 0;
                discardResult = 1;
                handResult += deckCount + discardCount;
            }
            else
            {
                deckResult = discardCount -(3 - deckCount);
                discardResult -= discardCount;
                handResult += 3;
            }
        }
        
        else
        {
            deckResult -= 3;
            handResult += 3;
        }
        
        testG.handCount[thisPlayer] = handCount;
        testG.deckCount[thisPlayer] = deckCount;
        testG.discardCount[thisPlayer] = discardCount;
        
        // Now, randomly assign the hand, deck and discard, first, randomly assign which card will be smithy
        
        smithyIndex = rand() % handCount;
        handpos = smithyIndex; // Set the hand position for the smithy card.
        
        for (i = 0; i < handCount; i++)
        {
            if(i == smithyIndex)
            {
                testG.hand[thisPlayer][i] = smithy;
            }
            else
            {
                testG.hand[thisPlayer][i] = testCards[rand() % (nTestCards - 1)];
            }
        }
        
        for (i = 0; i < deckCount; i++)
        {
            testG.deck[thisPlayer][i] = testCards[rand() % nTestCards];
        }
        
        for (i = 0; i < discardCount; i++)
        {
            testG.discard[thisPlayer][i] = testCards[rand() % nTestCards];
        }
        
        // ----------- TEST Description --------------
        printf("\nTEST %d: Deck = %d, Hand = %d, Discard = %d\n", count, deckCount, handCount, discardCount);
        
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        //Test if smithy is still in hand
        isInHand = 0;
        for (i = 0; i < testG.handCount[thisPlayer]; i++)
        {
            if(testG.hand[thisPlayer][i] == adventurer)
            {
                isInHand = 1;
            }
        }
        
        printf("hand count = %d, expected = %d", testG.handCount[thisPlayer], handResult);
        assertTotal += customAssert(testG.handCount[thisPlayer] == handResult);
        
        printf("deck count = %d, expected = %d", testG.deckCount[thisPlayer], deckResult);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckResult);
        
        printf("discard count = %d, expected = %d", testG.discardCount[thisPlayer],discardResult);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardResult);
        
        printf("Smithy not in hand");
        assertTotal += customAssert(!isInHand);
    }
    return 0;
}


