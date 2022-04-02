#include <vector>
#include <string>
#include "Stack.h"

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

class Player
{
private:
    int id;
    vector<int> deck;
    Stack *stacksList[4];
    bool isTurn;
    bool canPlay;
    int cardToPlace;
    int stackToPLace;
public:
    Player(int);
    ~Player();

    string getCards();
    int getCardToPlace(){return cardToPlace;}
    int getStackToPlace(){return stackToPLace;}
    bool getIsTurn(){return isTurn;}
    int getId(){return id;}
    bool getCanPlay(){return canPlay;}
    int getIndex(vector<int>, int);
    int getDeckSize(){return deck.size();}

    void setCanPlay(bool);

    void dealCard(int card);
    void startTurn(Stack*[4]);
    void resetChoice(bool isValid);
    bool isMoveValid(int, int);

    void setCardAndStackTEST();
    bool isEndOfTurnTEST();
};


#endif