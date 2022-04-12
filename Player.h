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
public:
    Player(int, StreamSocket*);
    ~Player();

    string getCards();
    string getCards(int index);
    Stack* getStack(int index){return stacksList[index];}
    bool getIsTurn(){return isTurn;}
    int getId(){return id;}
    bool getCanPlay(){return canPlay;}
    int getIndex(vector<int>, int);
    int getDeckSize(){return deck.size();}
    vector<int> getDeck(){return deck;}

    void setCanPlay();
    void setId(int Id){id = Id;}

    void dealCard(int card);
    void startTurn(Stack*[4]);
    void placeCard(int, int);
    bool isMoveValid(int, int);
    void sendMessage(string message){socket->send(message); cout << "send : " << message << endl;}
    string readMessage();
};


#endif