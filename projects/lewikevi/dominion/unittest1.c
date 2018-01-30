/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
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

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int main() {
    // Basic set up for the game
    int i;
    int seed = 1000;
    int numPlayer = 4; // max number of players
    int maxBonus = 10;
    int p, r, card; supply;
    int handCount = 5;
    int k[10] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall};
    int testCardSize = 16; // Number of cards to test gaining
    int testCards[testCardSize] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall, estate, duchy, province, copper, silver, gold};
    struct gameState G;
    int maxHandCount = 5;
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
    
    printf ("TESTING gainCard():\n");
    for (p = 0; p < numPlayer; p++)
    {
        for (card = 0; card <= testCardSize; card++){
            for (supply = 0; supply <= 1; supply++){
                
#if (NOISY_TEST == 1)
                printf("Test player %d drawing card %s sith supply %d bonus.\n", p, testCards[card], supply);
#endif
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                gameState->supplyCount[testCards[card]] = supply// set the number of cards on hand
                G.handCount[p] = handCount;
                gainCard(
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);
#endif
                assert(G.coins == handCount * 1 + bonus); // check if the number of coins is correct
                
                memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
                updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);
#endif
                assert(G.coins == handCount * 2 + bonus); // check if the number of coins is correct
                
                memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(p, &G, bonus);
#if (NOISY_TEST == 1)
                printf("G.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);
#endif
                assert(G.coins == handCount * 3 + bonus); // check if the number of coins is correct
            }
        }
    }
    
    printf("All tests passed!\n");
    
    return 0;
}
