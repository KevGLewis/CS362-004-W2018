/* -----------------------------------------------------------------------
 * This code is based on the template provided by Professor Aburas
 *
 * Testing the full deck count function
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
#define TESTFUNCTION "fullDeckCount()"

int main() {
    // Basic set up for the game
    int i;
    int seed = 1000;
    int assertTotal = 0;
    int numPlayer = 4; // max number of players
    int p, r, result, handCards, deckCards, discardCards, card;
    int maxHandCount = 20;
    int maxDiscardCount = maxHandCount;
    int maxDeckCount = maxHandCount;
    int resetHand[maxHandCount];
    int resetDeck[maxDeckCount];
    int resetDiscard[maxDiscardCount];
    int k[10] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int testCardSize = 15; // Number of cards to test gaining
    int testCards[15] = {adventurer, council_room, feast, gardens, mine
        , remodel, smithy, village, great_hall, estate, duchy, province, copper, silver, gold};
    
    for (i = 0; i < maxHandCount; i++)
    {
        resetHand[i] = baron;
    }
    for (i = 0; i < maxDiscardCount; i++)
    {
        resetDiscard[i] = baron;
    }
    for (i = 0; i < maxDeckCount; i++)
    {
        resetDeck[i] = baron;
    }
    
    
    printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);
    for (p = 0; p < numPlayer; p++){
        for (card = 0; card <= testCardSize; card++){
            for(handCards = 0; handCards <= maxHandCount; handCards += 5){
                for(deckCards = 0; deckCards <= maxHandCount; deckCards += 5){
                    for(discardCards = 0; discardCards <= maxHandCount; discardCards += 5){
                    
                        memset(&G, 23, sizeof(struct gameState));   // clear the game state
                        r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                        G.whoseTurn = p;
                        G.handCount[p] = maxHandCount;
                        memcpy(G.hand[p], resetHand, sizeof(int) * maxHandCount); // initialize hand to all baro
                        G.deckCount[p] = maxDeckCount;
                        memcpy(G.deck[p], resetDeck, sizeof(int) * maxDeckCount); // initialize deck to all barons
                        G.discardCount[p] = maxDiscardCount;
                        memcpy(G.discard[p], resetDiscard, sizeof(int) * maxDiscardCount); // initialize discard to all barons
                        // Set up the hand / deck / discard
                    
                        for(i = 0; i < handCards; i++){
                            G.hand[p][i] = testCards[card];
                        }
                        
                        for(i = 0; i < deckCards; i++){
                            G.deck[p][i] = testCards[card];
                        }
                        
                        for(i = 0; i < discardCards; i++){
                            G.discard[p][i] = testCards[card];
                        }
                        
                        result = handCards + deckCards + discardCards;
                        int testResult = fullDeckCount(p, testCards[card], &G);
                        
#if (NOISY_TEST == 1)
                        printf("Test player %d, card %d, with %d deck, %d hand, and %d discard. \n", p, testCards[card], deckCards, handCards, discardCards);
#endif
                        
#if (NOISY_TEST == 1)
                        printf("G.cards = %d, expected = %d\n", testResult, result);
#endif
                        assertTotal += customAssert(testResult == result);
                        
                        
                    }
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
