#include <iostream>
#include "Player.h"

using namespace std;


void Player::dealCard(int card)
{
    deck.push_back(card);
}

Player::Player(int Id)
{
    id = Id;
    isTurn = false;
    cardToPlace = 2;
    stackToPLace = 1;
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