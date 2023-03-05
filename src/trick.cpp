#include "common.h"

class Trick
{
public:
    int leader;
    vector<pair<int, int>> cards;
    Trick(int l): leader(l), cards() {}
    
    int winner(){
        int leadSuite = cards[0].first;
        int best_i = 0;
        for(int i=1; i<cards.size(); i++){
            if(cards[i].first == cards[best_i].first){
                if(cards[i].second > cards[best_i].second){
                    best_i = i;
                }
            }
            else if(cards[i].first == 0){
                best_i = i;
            }
        }
        return best_i;
    }
};
