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

#define TESTCARD "remodel"

int main() {
    
    srand(time(NULL)); // Seed the random number generator
    int assertTotal = 0;
    int deckCount = 0;
    int count = 0;
    int discardCount = 0;
    int handCount;
    int deckResult, handResult, discardResult;
    int i, remodelIndex, supplyResult, IsInHand, supplyNumb;
    int ExpectedInHand;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0; // Only Need to test player one
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
        sea_hag, tribute, smithy, council_room};
    int nTestCards = 13;
    int testCards[13] = {embargo, village, minion, smithy, cutpurse,
        sea_hag, tribute, adventurer, council_room, copper, silver, gold, mine};
    
    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    
    time_t begin = time(NULL);
    
    double time_spent = 0;
    
    while(time_spent < 1)
    {
        printf("time spent = %f", time_spent);
        count++;
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Randomly Choose the number of cards in the deck, discard, and hand
        // ensure that the maximum total is less than the max deck
        
        handCount = 2 + rand() % 10;
        deckCount = rand() % 50;
        discardCount = rand() % 50;
        
        testG.handCount[thisPlayer] = handCount;
        testG.deckCount[thisPlayer] = deckCount;
        testG.discardCount[thisPlayer] = discardCount;
        
        // Now, randomly assign the hand, deck and discard, first, randomly assign which card will be adventurer
        
        remodelIndex = rand() % handCount;
        handpos = remodelIndex;
        
        choice2 = testCards[rand() % nTestCards]; // choice 2 is the card we are going to get
        supplyNumb = rand() % 2;
        testG.supplyCount[choice2] = supplyNumb; // either there is 1 or 0 cards in the supply
        
        choice1 = rand() % handCount; // choice 1 is the card we are going to trash
        while(choice1 == handpos)
        {
            choice1 = rand() % handCount; // Ensure choice 1 is different than handpos 1
        }
        
        for (i = 0; i < handCount; i++)
        {
            if(i == remodelIndex)
            {
                testG.hand[thisPlayer][i] = remodel;
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

        // Determine the results
        
        if(getCost(choice2) <= getCost(testG.hand[thisPlayer][choice1]) + 2){
            handResult = testG.handCount[thisPlayer] - 2;
            deckResult = testG.deckCount[thisPlayer];
            discardResult = testG.discardCount[thisPlayer] + 2;
            supplyResult = testG.supplyCount[choice2] - 1;
            ExpectedInHand = 0;
        }else{
            handResult = testG.handCount[thisPlayer] - 1;
            deckResult = testG.deckCount[thisPlayer];
            discardResult = testG.discardCount[thisPlayer] + 1;
            supplyResult = testG.supplyCount[choice2];
            ExpectedInHand = 1;
        }
        
        // We will be trying to trash the card in hand position 1 and make sure that there is enough supply.
        cardEffect(remodel, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        // ----------- TEST Description --------------
        printf("\n\nTEST %d: Deck = %d, Hand = %d, Discard = %d, Supply = %d\n", count, deckCount, handCount, discardCount, supplyNumb);
        
        cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        //Test if smithy is still in hand
        IsInHand = 0;
        for (i = 0; i < testG.handCount[thisPlayer]; i++)
        {
            if(testG.hand[thisPlayer][i] == remodel)
            {
                IsInHand = 1;
            }
        }
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], handResult);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckResult);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer], discardResult);
        printf("supply count = %d, expected = %d\n", testG.supplyCount[choice2], supplyResult);
        printf("Smithy in hand = %d, expected = %d\n", IsInHand, ExpectedInHand);
        assertTotal += customAssert(testG.handCount[thisPlayer] == handResult);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckResult);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardResult);
        assertTotal += customAssert(testG.supplyCount[choice2] == supplyResult);
        assertTotal += customAssert(IsInHand == ExpectedInHand);
        
        time_spent = (double)(time(NULL) - begin); // update the time
    }
    return 0;
}


