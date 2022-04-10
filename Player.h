#include <vector>
#include <string>
#include "Stack.h"
#include "socket.h"

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;
using namespace  stdsock;

class Player
{
private:
    StreamSocket* socket;
    int id;
    vector<int> deck;
    Stack *stacksList[4];
    bool isTurn;
    bool canPlay;
    int cardToPlace;
    int stackToPLace;
public:
    Player(int, StreamSocket*);
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
    void sendMessage(string message){socket->send(message);}
    string readMessage();

    void setCardAndStackTEST();
    bool isEndOfTurnTEST();
};


#endif