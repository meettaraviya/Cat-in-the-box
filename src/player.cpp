#include "common.h"
#include "trick.cpp"
#include <algorithm>

class PlayerState
{
public:
    vector<int> hand;
    vector<pair<int, int>> playedCards;
    vector<bool> canPlay = vector<bool>(4, true);
    int tricks = 0;
    int prediction = 0;
    int points = 0;
    int discard = 0;
    int index = -1;

    PlayerState() {}
    PlayerState(int i): index(i), hand(), playedCards() {}
};

class Game;

class Player
{
public:
    PlayerState* state;
    Game* game;
    Player(PlayerState* s, Game* g): state(s), game(g) {}

    int make_prediction(){
        return 2;
    }
    int discard(){
        while(true){
            printf("Your hand:\n");
            for(int card: state->hand){
                printf("%d", card);
            }
            printf("\nChoose a card to discard: ");
            int discard;
            scanf("%d", &discard);
            if(find(state -> hand.begin(), state -> hand.end(), discard) != state -> hand.end()){
                return discard;
            }
            printf("\nInvalid choice, try again:\n");
        }
        return state -> hand[0];
    }
    pair<int, int> get_play(Trick trick){
        return {0, state -> hand[0]};
    }
};