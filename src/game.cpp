#include "common.h"
#include "player.cpp"
#include "utils.cpp"
#include <algorithm>

class Game
{
public:
    int numPlayers=5;
    int startPlayer=0;
    int maxCard=9;
    int cardCount=9;
    int paradoxCause=-1;
    vector<PlayerState> playerStates;
    vector<Player> players;
    vector<vector<int>> cardPlayer;

    Game(): Game(5) {}
    Game(int n): numPlayers(n), playerStates(), players() {
        playerStates = vector<PlayerState>();
        for(int i=0; i<numPlayers; i++) playerStates.push_back(PlayerState(i));
        for(int i=0; i<numPlayers; i++) players.push_back(Player(&playerStates[i], this));
        int maxCardValues[4] = {5, 6, 8, 9};
        maxCard = maxCardValues[numPlayers-2];
        vector<int> cardCountValues = {10, 10, 10, 9};
        cardCount = cardCountValues[numPlayers-2];
        cardPlayer = vector<vector<int>>(4, vector<int>(maxCard, -1));
    }

    void play(){
        for(int i=0; i<numPlayers; i++){
            startPlayer = i;
            printf("Player %d is the first player\n\n", startPlayer+1);
            printf("Preparing round %d\n", i+1);
            prepare_phase();
            printf("Round %d prepared\n\n", i+1);

            printf("Beginning trick phase for round %d\n", i+1);
            trick_phase();
            printf("Trick phase for round %d complete\n\n", i+1);

            printf("Scoring round %d\n", i+1);
            score_phase();
            printf("Round %d scored\n\n", i+1);
        }
    }

    void clear_states(){
        for(int i=0; i<numPlayers; i++){
            playerStates[i].hand.clear();
            playerStates[i].playedCards.clear();
            playerStates[i].tricks = 0;
            playerStates[i].prediction = 0;
            playerStates[i].discard = 0;
            playerStates[i].canPlay = vector<bool>(4, true);
        }
        paradoxCause = -1;
    }

    void prepare_phase(){
        vector<int> deck = build_deck();
        random_shuffle(deck.begin(), deck.end());

        clear_states();

        vector<int> predictions(numPlayers, 0);

        for(int i_=0; i_<numPlayers; i_++){
            int i = (startPlayer+i_)%numPlayers;
            playerStates[i].hand = vector<int>(deck.begin() + i*cardCount, deck.begin() + (i+1)*cardCount);
            sort(playerStates[i].hand.begin(), playerStates[i].hand.end());
            playerStates[i].prediction = players[i].make_prediction();
            predictions[i] = playerStates[i].prediction;
        }

        for(int i_=0; i_<numPlayers; i_++){
            int i = (startPlayer+i_)%numPlayers;
            playerStates[i].discard = players[i].discard();
            playerStates[i].hand.erase(find(playerStates[i].hand.begin(), playerStates[i].hand.end(), playerStates[i].discard));
        }
    }

    void trick_phase(){
        int curr_leader = startPlayer;
        while(playerStates[0].hand.size()>1 and paradoxCause<0){
            curr_leader = resolve_trick(curr_leader);
            if(curr_leader >= 0) playerStates[curr_leader].tricks += 1;
        }
    }

    void score_phase(){
        UnionFind uf(4*maxCard);
        vector<int> bonusPoints(numPlayers, 0);
        for(int i=0; i<4; i++) for(int j=1; j<maxCard; j++){
            int cp = cardPlayer[i][j];
            if(cp>=0 and cp == cardPlayer[i][j-1]){
                uf.union_sets(i*maxCard+j, i*maxCard+j-1);
                bonusPoints[cp] = max(bonusPoints[cp], uf.size[uf.find_set(i*maxCard+j)]);
            }
        }
        for(int i=1; i<4; i++) for(int j=0; j<maxCard; j++){
            int cp = cardPlayer[i][j];
            if(cp>=0 and cp == cardPlayer[i-1][j]){
                uf.union_sets(i*maxCard+j, (i-1)*maxCard+j);
                bonusPoints[cp] = max(bonusPoints[cp], uf.size[uf.find_set(i*maxCard+j)]);
            }
        }
        for(int i=0; i<numPlayers; i++){
            if(paradoxCause==i){
                playerStates[i].points -= playerStates[i].tricks;
            }
            else{
                playerStates[i].points += playerStates[i].tricks;
                if(playerStates[i].tricks == playerStates[i].prediction){
                    playerStates[i].points += bonusPoints[i];
                }
            }
        }
    }

    int resolve_trick(int leader){
        Trick trick(leader);
        int leadSuit = -1;
        for(int i_=0; i_<numPlayers; i_++){
            int i = (i_+leader)%numPlayers;
            pair<int, int> play = players[i].get_play(trick);
            if(play.first<0){ // paradox
                paradoxCause = i;
                break;
            }
            else{
                playerStates[i].hand.erase(find(playerStates[i].hand.begin(), playerStates[i].hand.end(), play.second));
                if(i_ == 0) leadSuit = play.first;
                cardPlayer[play.first][play.second-1] = i;
                trick.cards.push_back(play);
                if(play.first != leadSuit){
                    playerStates[i].canPlay[leadSuit] = false;
                }
            }
        }
        if(paradoxCause >= 0){
            return -1;
        }
        else{
            int w_ = trick.winner();
            return (w_+numPlayers-leader)%numPlayers;
        }
    }

    vector<int> build_deck(){
        vector<int> deck;
        deck.reserve(maxCard*5);
        for(int i=0; i<5; i++) for(int j=0; j<maxCard; j++) deck.push_back(j+1);
        return deck;
    }
};