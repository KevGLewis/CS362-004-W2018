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

#define TESTCARD "mine"

int main() {
    int count = 0;
    int assertTotal = 0;
    int deckCount = 0;
    int handCount = 0;
    int discardCount = 0;
    int oldCardPresent, newCardPresent;
    int m, supply, oldCardExpected, newCardExpected;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
    int tCardSizeOne = 4;
    int tCardSize = 6; // Number of cards to test gaining
    int tCardsOne[4] = {copper, silver, gold, feast};
    int tCards[6] = {great_hall, duchy, province, copper, silver, gold};
    
    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    //Set the initial Hnad to all estates
    G.handCount[thisPlayer] = 5;
    for(int i = 0; i < 5; i++){
        G.hand[thisPlayer][i] = estate;
    }
    
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for(int i = 0; i < tCardSizeOne; i++){
        for(int j = 0; j < tCardSize; j++){
            count++;
            // ----------- TEST 1: +2 coin cards at top of deck --------------
            printf("TEST %d: = %d card for %d\n", count, tCardsOne[i], tCards[j]);

            // copy the game state to a test case
            memcpy(&testG, &G, sizeof(struct gameState));
            
            // We will be trying to trash the card in hand position 1 and make sure that there is enough supply.
            handpos = 1;
            choice1 = handpos;
            choice2 = tCards[j];
            testG.hand[thisPlayer][handpos] = tCardsOne[i];
            testG.supplyCount[tCards[j]] = 10;
            
            cardEffect(mine, choice1, choice2, choice3, &testG, handpos, &bonus);
            
            handCount = G.handCount[thisPlayer];
            deckCount = G.deckCount[thisPlayer];
            discardCount = G.discardCount[thisPlayer];
            
            if((getCost(tCards[j]) <= getCost(tCardsOne[i]) + 3) &&
               (tCardsOne[i] == copper || tCardsOne[i] == silver || tCardsOne[i] == gold) && (tCards[j] == copper || tCards[j] == silver || tCards[j] == gold) ){
                supply = 9;
                if(tCardsOne[i] == tCards[j])
                {
                    newCardExpected = true;
                    oldCardExpected = true;
                }
        
                else
                {
                    newCardExpected = true;
                    oldCardExpected = false;
                }
                
            }else{
                supply = 10;
                newCardExpected = false;
                oldCardExpected = true;
                
                if(tCardsOne[i] == tCards[j])
                {
                    newCardExpected = true;
                    oldCardExpected = true;
                }
            }
            
            //Test if the old card is present and if the new card is present
            oldCardPresent = newCardPresent = false;
            
            for(m = 0; m < testG.handCount[thisPlayer]; m++){
                if(testG.hand[thisPlayer][m] == tCardsOne[i])
                {
                    oldCardPresent = true;
                }
                if(testG.hand[thisPlayer][m] == tCards[j]){
                    newCardPresent = true;
                }
                
            }
            
            printf("New card cost = %d, max cost = %d\n", getCost(tCards[j]), getCost(tCardsOne[i]) + 3);
            printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], handCount);
            printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
            printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
            printf("card supply = %d, expected = %d\n", testG.supplyCount[tCards[j]],supply);
            printf("Old card still in hand = %d, expected = %d\n", oldCardPresent, oldCardExpected);
            printf("New card in hand = %d, expected = %d\n", newCardPresent, newCardExpected);
            assertTotal += customAssert(testG.handCount[thisPlayer] == handCount);
            assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
            assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
            assertTotal += customAssert(testG.supplyCount[tCards[j]] == supply);
            assertTotal += customAssert(oldCardPresent == oldCardExpected);
            assertTotal += customAssert(newCardPresent == newCardExpected);
            printf("\n");
        }
    }
    if(assertTotal == 0){
        printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTCARD);
    }
    else{
        printf("\n >>>>> FAILURE: Testing complete %s <<<<<\n\n", TESTCARD);
    }

	return 0;
}


