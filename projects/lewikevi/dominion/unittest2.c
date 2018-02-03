/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 *
 * Testing the buy Card function
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdbool.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1
#define TESTFUNCTION "buyCard()"

int main() {
    // Basic set up for the game
    int i;
    int seed = 1000;
    int assertTotal = 0;
    int numPlayer = 4; // max number of players
    int p, r, card, supply, numCoin, numBuys, bResult;
    int disCount = 0;
    int dResult, supResult, coinResult;
    int handCount = 5;
    int k[10] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall};
    int testCardSize = 16; // Number of cards to test gaining
    int testCards[16] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall, estate, duchy, province, copper, silver, gold};
    struct gameState G;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
    }
    
    printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);
    for (p = 0; p < numPlayer; p++){
        for (card = 0; card <= testCardSize; card++){
            for(supply = 0; supply <= 1; supply++){
                for(numBuys = 0; numBuys <= 1; numBuys++){

                    memset(&G, 23, sizeof(struct gameState));   // clear the game state
                    r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                    G.whoseTurn = p;
                    numCoin = getCost(testCards[card]) + supply - 1; // Either we'll have just enough coins or 1 less than enough
                    G.coins = numCoin;
                    G.numBuys = numBuys;
                    G.whoseTurn = p;
                    G.supplyCount[testCards[card]] = supply;// set the number of cards on hand
                    G.handCount[p] = handCount;
                    G.discardCount[p] = disCount;
                    buyCard(testCards[card], &G); // Buy a card and put it in discard
                    
                    // Determine Expected Results

                    dResult = disCount;
                    coinResult = numCoin;
                    bResult = numBuys;
                    
                    if(supply > 0 && numCoin >= getCost(testCards[card]) && numBuys > 0){
                        supResult = supply - 1;
                        dResult = disCount + 1;
                        coinResult = numCoin - getCost(testCards[card]);
                        bResult = numBuys  - 1;
                    }else if (supply > 0){
                        supResult = supply;
                    }else
                        supResult = 0;
                    
                    
#if (NOISY_TEST == 1)
            printf("Test player %d buying card %d with supply %d, money %d, discard %d. \n", p, testCards[card], supply, numCoin, disCount);
#endif
            
#if (NOISY_TEST == 1)
            printf("G.discardCount = %d, expected = %d\n", G.discardCount[p], dResult);
#endif
            assertTotal += customAssert(G.discardCount[p] == dResult); 

#if (NOISY_TEST == 1)
            printf("G.supplyCount = %d, expected = %d\n", G.supplyCount[testCards[card]], supResult);
#endif
            assertTotal += customAssert(G.supplyCount[testCards[card]] == supResult);
            
#if (NOISY_TEST == 1)
            printf("G.coins = %d, expected = %d\n", G.coins, coinResult);
#endif
            assertTotal += customAssert(G.coins == coinResult);

#if (NOISY_TEST == 1)
            printf("G.numBuys = %d, expected = %d\n", G.numBuys, bResult);
#endif
            assertTotal += customAssert(G.numBuys == bResult);
                }
            }
        }
    }
    
    if(assertTotal == 0){
        printf("\n >>>>> SUCCESS: Testing complete %s <<<<<\n\n", TESTFUNCTION);
    }
    else{
        printf("\n >>>>> FAILURE: Testing complete %s <<<<<\n\n", TESTFUNCTION);
    }
    
    return 0;
}
