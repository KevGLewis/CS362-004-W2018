/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 *
 * Testing the upDate Coins function
 *
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


int customAssert(bool test){
    if (test == false){
        printf("TEST FAILED");
        return 1;
    }
    else{
        printf("TEST SUCCESSFULLY COMPLETED");
        return 0;
    }
}

int main() {
    // Basic set up for the game
    int i;
    int seed = 1000;
    int assertTotal = 0;
    int numPlayer = 4; // max number of players
    int p, r, result, nCop, nSil, nGold, bonus;
    int handCount = 5;
    int barons[handCount];
    int maxBonus = 5;
    int k[10] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    for (i = 0; i < handCount; i++)
    {
        barons[i] = baron;
    }
    
    printf ("TESTING updateCOins():\n");
    for (p = 0; p < numPlayer; p++){
        for (nCop = 0; nCop <= handCount; nCop++){
            for(nSil = 0; nSil <= handCount - nCop; nSil++){
                for(nGold = 0; nGold <= handCount - nCop - nSil; nGold++){
                    for(bonus = 0; bonus <= maxBonus; bonus++){
                    
                        memset(&G, 23, sizeof(struct gameState));   // clear the game state
                        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                        G.handCount[p] = handCount;
                        memcpy(G.hand[p], barons, sizeof(int) * handCount); // initialize hand to all barons

                        // Set up the hand
                        for(i = 0; i < nCop; i++){
                            G.hand[p][i] = copper;
                        }
                    
                        for(i = nCop; i < nCop + nSil; i++){
                            G.hand[p][i] = silver;
                        }
                    
                        for(i = nCop + nSil; i < nCop + nSil + nGold; i++){
                            G.hand[p][i] = gold;
                        }
                    
                        result = nCop + (2 * nSil) + (3 * nGold) + bonus;
                    
#if (NOISY_TEST == 1)
                        printf("Test player %d, with %d copper, %d silver, %d gold, %d bonus. \n", p, nCop, nSil, nGold, bonus);
#endif
                        
#if (NOISY_TEST == 1)
                        printf("G.coins = %d, expected = %d\n", G.coins, result);
#endif
                        assertTotal += customAssert(G.coins == result);
                        

                    }
                }
            }
        }
    }
    
    if(assertTotal == 0){
        printf("All tests passed!\n");
    }
    
    return 0;
}
