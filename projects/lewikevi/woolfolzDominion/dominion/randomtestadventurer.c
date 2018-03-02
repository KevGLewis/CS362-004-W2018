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

#define TESTCARD "adventurer"

int main() {
    
    srand(time(NULL)); // Seed the random number generator
    int newCards = 0;
    int discarded = 1;
    int assertTotal = 0;
    int deckCount = 0;
    int count = 0;
    int discardCount = 0;
    int handCount, advIndex;
    int deckResult, handResult, discardResult, isInHand;
    int i, preCoinCount, ActCoinCount;
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
        sea_hag, tribute, smithy, council_room, copper, silver, gold, adventurer};
    
    int nTestCoins = 3;
    int testCoins[3] = {copper, silver, gold};
    
    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    
    time_t begin = time(NULL);
    
    double time_spent = 0;
    
    count = 0;
    while(count < 100)
    {
        count++;
        memcpy(&testG, &G, sizeof(struct gameState));
        choice1 = 1;
        
        // Randomly Choose the number of cards in the deck, discard, and hand
        // ensure that the maximum total is less than the max deck
        
        handCount = 1 + rand() % 20;
        deckCount = rand() % 50;
        discardCount = rand() % 50;
        
        testG.handCount[thisPlayer] = handCount;
        testG.deckCount[thisPlayer] = deckCount;
        testG.discardCount[thisPlayer] = discardCount;

        // Now, randomly assign the hand, deck and discard, first, randomly assign which card will be adventurer
        
        advIndex = rand() % handCount;
        handpos = advIndex;

        for (i = 0; i < handCount; i++)
        {
            if(i == advIndex)
            {
                testG.hand[thisPlayer][i] = adventurer;
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
        
        // Note, we need to make the discard all coppers as the shuffling will randomize anything we put in there.
        // Therefore, we wouldn't be able to know what was in the discard after it was shuffled unless they werw all the same
        // Card.
        for (i = 0; i < discardCount; i++)
        {
            testG.discard[thisPlayer][i] = testCoins[rand() % nTestCoins];
        }
        
        // Now determine what the correct output should be
        discardResult = discardCount + 1; //Discarding Adventurer
        deckResult = deckCount;
        handResult = handCount - 1; //Discarding Adventurer
        bothCoins = 0;
        
        for(i = deckCount - 1; i >= 0; i--)
        {
            if(bothCoins == 2)
            {
                break;
            }
            
            if(testG.deck[thisPlayer][i] == copper || testG.deck[thisPlayer][i] == silver ||
               testG.deck[thisPlayer][i] == gold)
            {
                bothCoins++;
                handResult++;
                deckResult--;
            }
            else
            {
                deckResult--;
                discardResult++;
            }
        }
    
        if(bothCoins < 2)
        {
            if(bothCoins == 1)
            {
                deckResult = discardCount - 1;
                discardResult++;
            }
            if(bothCoins == 0)
            {
                deckResult = discardCount - 2;
                deckResult += 2;
            }
        }
        
        // Get the number of coins in the hand before adventurer is played
        preCoinCount = 0;
        for(i = 0; i < handCount; i++)
        {
            if(testG.hand[thisPlayer][i] == copper || testG.hand[thisPlayer][i] == silver ||
                testG.hand[thisPlayer][i] == gold)
            {
                preCoinCount++;
            }
        }
    
        // ----------- TEST Description --------------
        printf("\nTEST %d: Deck = %d, Hand = %d, Discard = %d", count, deckCount, handCount, discardCount);
        
        cardEffect(adventurer, choice1, choice2, choice3, &testG, handpos, &bonus);
        
        //Test if adventurer is still in hand and how many coins there are
        isInHand = 0;
        ActCoinCount = 0;
        for (i = 0; i < testG.handCount[thisPlayer]; i++)
        {
            if(testG.hand[thisPlayer][i] == adventurer)
            {
                isInHand = 1;
            }
            else if(testG.hand[thisPlayer][i] == copper || testG.hand[thisPlayer][i] == silver ||
                    testG.hand[thisPlayer][i] == gold)
            {
                ActCoinCount++;
            }
        }
        
        printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
        printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
        printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
        printf("Adventuerer not in hand\n");
        printf("Coin count = %d, expected = %d\n", ActCoinCount, preCoinCount + 2);
        assertTotal += customAssert(testG.handCount[thisPlayer] == handResult);
        assertTotal += customAssert(testG.deckCount[thisPlayer] == deckResult);
        assertTotal += customAssert(testG.discardCount[thisPlayer] == discardResult);
        assertTotal += customAssert(!isInHand);
        assertTotal += customAssert(ActCoinCount == preCoinCount + 2);

        time_spent = (double)(time(NULL) - begin); // update the time
    }
    return 0;
}


