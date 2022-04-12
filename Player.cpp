#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include "Player.h"

using namespace std;


Player::Player(int Id, StreamSocket* Socket)
{
    srand(time(0));
    id = Id;
    isTurn = false;
    socket = Socket;
    //init stacks
    for (int i = 0; i < 4; i++)
    {
        stacksList[i] = new Stack(i, i % 2);
    }
}

void Player::dealCard(int card)
{
    if(card != -1)
    {
        deck.push_back(card);
        sort(deck.begin(), deck.end());
    }
}

string Player::getCards()
{
    string result = "";
    for (int i = 0; i < deck.size(); ++i)
    {
        result += to_string(deck.at(i)) + " ";
    }
    result += "\n";
    return result;
}

string Player::getCards(int index){
    return to_string(deck.at(index));
}

void Player::startTurn(Stack *stackList[4])
{
    isTurn = true;
    for (int i = 0; i < 4; i++) {
        this->stacksList[i] = stackList[i];
    }
}

void Player::placeCard(int cardValue, int stackIndex)
{
    stacksList[stackIndex]->setTopCard(cardValue);
    deck.erase(deck.begin() + getIndex(deck, cardValue));
}

bool Player::isMoveValid(int card, int stack)
{

    if(card <= -1 || stack <= -1)
        return false;

    if(
            (stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() < card) ||
            (!stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() > card) ||
            (!stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() == card + 10) ||
            (stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() == card - 10)
            )
    {
        return true;
    }
    return false;
}

void Player::setCanPlay()
{
    for (int i = 0; i < deck.size(); ++i)
    {
        for (int j = 0; j < 4; ++j) {
            if(isMoveValid(i, j))
                canPlay = true;
        }
    }

    //cout << "can play : " << to_string(this->canPlay) << endl;
}

int Player::getIndex(vector<int> v, int value)
{
    auto it = find(v.begin(), v.end(), value);

    if (it != v.end())
    {
        return it - v.begin();
    }
    return -1;
}

string Player::readMessage()
{
    string result = " ";
    while(result == " ")
    {
        socket->read(result);
    }
    cout << "Receive Player " << to_string(getId()) << " : " << result << endl;
    result = result.substr(1, result.length() -  1);
    return result;
}

Player::~Player()
{
    for (int i = 0; i <= 3; i++)
    {
        delete stacksList[i];
    }
}