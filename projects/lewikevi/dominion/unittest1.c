/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 *
 * Testing the gainCard function
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
    int assertTotal = 0;
    int seed = 1000;
    int numPlayer = 4; // max number of players
    int p, r, card, supply, result;
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
    
    printf ("TESTING gainCard():\n");
    for (p = 0; p < numPlayer; p++)
    {
        for (card = 0; card <= testCardSize; card++){
            for (supply = 0; supply <= 1; supply++){
                
#if (NOISY_TEST == 1)
                printf("Test player %d drawing card %d with supply %d.\n", p, testCards[card], supply);
#endif
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                G.supplyCount[testCards[card]] = supply;// set the number of cards on hand
                G.handCount[p] = handCount;
                
                if (supply == 0){ // Set the result as it depends on the supply
                    result = G.discardCount[p];
                }
                else{
                    result = G.discardCount[p] + 1;
                }
                gainCard(testCards[card], &G, 0, p); // Gain a card to the discard
                
#if (NOISY_TEST == 1)
                printf("G.discardCount = %d, expected = %d\n", G.discardCount[p], result);
#endif
                assertTotal += customAssert(G.discardCount[p] == result); // check if the number of cards is correct
        
                G.supplyCount[testCards[card]] = supply; // reset the supply
                // Set the result as it depends on the supply
                if (supply == 0){
                    result = G.deckCount[p];
                }
                else{
                    result = G.deckCount[p] + 1;
                }
                 gainCard(testCards[card], &G, 1, p); // Gain a card to the deck
#if (NOISY_TEST == 1)
                printf("G.deckCount = %d, expected = %d\n", G.deckCount[p], result);
#endif
                assertTotal += customAssert(G.deckCount[p] == result); // check if the number of cards is correct
                
                G.supplyCount[testCards[card]] = supply; // reset the supply
                
                if (supply == 0){
                    result = G.handCount[p];
                }
                else{
                    result = G.handCount[p] + 1;
                }
                gainCard(testCards[card], &G, 2, p); // Gain a card to the hand
#if (NOISY_TEST == 1)
                printf("G.handcount = %d, expected = %d\n", G.handCount[p], result);
#endif
                assertTotal += customAssert(G.handCount[p] == result); // check if the number of cards is correct

                // Test if the supply has been diminished
                result = 0;
#if (NOISY_TEST == 1)
                printf("Card Supply = %d, expected = %d\n", G.supplyCount[testCards[card]], result);
#endif
                assertTotal += customAssert(G.supplyCount[testCards[card]] == result); // check if the supply has been diminished
                
            }
        }
    }
    
    
    if(assertTotal == 0){
        printf("All tests passed!\n");
    }
    
    return 0;
}
