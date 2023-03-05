#include "common.h"
#include "game.cpp"

using namespace std;

int main()
{
   Game game;
   printf("Cat in the box with %d players!!\n", game.numPlayers);
   game.play();
}