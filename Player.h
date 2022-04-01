#include <vector>
#include "string"

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

class Player
{
private:
    int id;
    vector<int> deck;
    bool isTurn;
    int cardToPlace;
    int stackToPLace;
public:
    Player(int);
    ~Player();
    void dealCard(int card);
    string getCards();
    int getCardToPlace(){return cardToPlace;}
    int getStackToPlace(){return stackToPLace;}
    bool getIsTurn(){return isTurn;}
    void resetChoice(bool isvalid){cardToPlace = -1; stackToPLace = -1;}
};


#endif