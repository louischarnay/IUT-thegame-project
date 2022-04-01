#include <vector>
#include "Stack.h"
#include "Player.h"

#ifndef GAME_H
#define GAME_H

using namespace std;

class Game
{
private:
    int NB_CARDS_START = 6;

    vector<int> sockets;
    Stack *stacksList[4];
    vector<int> deck;
    vector<Player*> players;

    int gameState; //0 = in game / 1 = game ended / -1 = game stopped
    int minCardsToPlace;
public:
    Game(vector<Player*>&);
    ~Game();
    vector<int> getDeck(){return this->deck;}
    vector<Player*> getPlayers(){return this->players;}
    void setDeck(vector<int> deck){this->deck = deck;}
    void setPlayers(vector<Player*> players){this->players = players;}

    bool canPlaceCard(int, int);
    void setGameState(int state) { gameState = state; }
    void setMinCardsToPlace(int minCards) { minCardsToPlace = minCards; }
    int dealRandomCard();
    void turnPlayer(int);
    void init(vector<Player*>&);
};


#endif

