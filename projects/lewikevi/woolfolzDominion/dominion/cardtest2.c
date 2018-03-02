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

#define TESTCARD "remodel"

int main() {
    int count = 0;
    int assertTotal = 0;
    int deckCount = 0;
    int handCount = 0;
    int discardCount = 0;
    int isCardThere;
    int supply, handPosOne;
    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 2;
    int thisPlayer = 0;
	struct gameState G, testG;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
			sea_hag, tribute, smithy, council_room};
    int tCardSize = 15; // Number of cards to test gaining
    int tCards[15] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall, duchy, province, copper, silver, gold};
    
    // initialize a game state and player cards
    initializeGame(numPlayers, k, seed, &G);
    G.whoseTurn = thisPlayer;
    
    //Set the initial Hnad to all estates
    G.handCount[thisPlayer] = 5;
    for(int i = 0; i < 5; i++){
        G.hand[thisPlayer][i] = estate;
    }
    
	printf("----------------- Testing Card: %s ----------------\n", TESTCARD);
    for(int i = 0; i < tCardSize; i++){
        for(int j = 0; j < tCardSize; j++){
            count++;
            // ----------- TEST 1: +2 coin cards at top of deck --------------
            printf("TEST %d: = %d card for %d\n", count, tCards[i], tCards[j]);

            // copy the game state to a test case
            memcpy(&testG, &G, sizeof(struct gameState));
            
            // We will be trying to trash the card in hand position 1 and make sure that there is enough supply.
            handpos = 1;
            choice1 = handpos;
            choice2 = tCards[j];
            testG.hand[thisPlayer][handpos] = tCards[i];
            testG.supplyCount[tCards[j]] = 10;
            
            cardEffect(remodel, choice1, choice2, choice3, &testG, handpos, &bonus);
            
            
            
            if(getCost(tCards[j]) <= getCost(tCards[i]) + 2){
                handCount = G.handCount[thisPlayer] - 1;
                deckCount = G.deckCount[thisPlayer];
                discardCount = G.discardCount[thisPlayer] + 1;
                supply = 9;
                handPosOne = false;
            }else{
                handCount = G.handCount[thisPlayer];
                deckCount = G.deckCount[thisPlayer];
                discardCount = G.discardCount[thisPlayer];
                supply = 10;
                handPosOne = true;
            }
            
            if(testG.hand[thisPlayer][1] == tCards[i]){
                isCardThere = true;
            } else{
                isCardThere = false;
            }
            
            printf("card cost = %d, max allowed = %d\n", getCost(tCards[j]), getCost(tCards[i]) + 2);
            printf("hand count = %d, expected = %d\n", testG.handCount[thisPlayer], handCount);
            printf("deck count = %d, expected = %d\n", testG.deckCount[thisPlayer], deckCount);
            printf("discard count = %d, expected = %d\n", testG.discardCount[thisPlayer],discardCount);
            printf("card supply = %d, expected = %d\n", testG.supplyCount[tCards[j]],supply);
            printf("card still in hand = %d, expected = %d\n", isCardThere, handPosOne);
            assertTotal += customAssert(testG.handCount[thisPlayer] == handCount);
            assertTotal += customAssert(testG.deckCount[thisPlayer] == deckCount);
            assertTotal += customAssert(testG.discardCount[thisPlayer] == discardCount);
            assertTotal += customAssert(testG.supplyCount[tCards[j]] == supply);
            assertTotal += customAssert(isCardThere == handPosOne);
            
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


