#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include "Player.h"

using namespace std;


Player::Player(int Id)
{
    srand(time(0));
    id = Id;
    isTurn = false;
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

void Player::startTurn(Stack *stackList[4])
{
    isTurn = true;
    for (int i = 0; i < 4; i++) {
        this->stacksList[i] = stackList[i];
    }
}

void Player::resetChoice(bool isValid)
{
    if(isValid)
    {
        stacksList[stackToPLace]->setTopCard(cardToPlace);
        deck.erase(deck.begin() + getIndex(deck, cardToPlace));
    }
    cardToPlace = -1;
    stackToPLace = -1;
}

void Player::setCardAndStackTEST()
{
    if(deck.size() == 0)
    {
        setCanPlay(false);
        stackToPLace = -1;
        cardToPlace = -1;
    }
    int stack = rand() % 4;
    for (int i = 0; i < 4; ++i) {
        if(stacksList[stack]->getIsCrescent()){
            for (int j = 0; j < deck.size(); ++j) {
                if(isMoveValid(deck.at(j), stack)) {
                    stackToPLace = stack;
                    cardToPlace = deck.at(j);
                    setCanPlay(true)    ;
                    return;
                }
            }
        } else {
            for (int j = deck.size() - 1; j > -1; j--) {
                if(isMoveValid(deck.at(j), stack)) {
                    stackToPLace = stack;
                    cardToPlace = deck.at(j);
                    setCanPlay(true)    ;
                    return;
                }
            }
        }

        stack++;
        stack = stack % 4;
    }
    setCanPlay(false);
    stackToPLace = -1;
    cardToPlace = -1;
}

bool Player::isMoveValid(int card, int stack)
{
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

bool Player::isEndOfTurnTEST()
{
    isTurn = rand() % 10;
    return (isTurn >= 1);
}

void Player::setCanPlay(bool canPlay)
{
    this->canPlay = canPlay;
    cout << "can play : " << to_string(this->canPlay) << endl;
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