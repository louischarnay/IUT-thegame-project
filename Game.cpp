#include <iostream>
#include <stdlib.h>
#include<time.h>
#include "Game.h"

using namespace std;

Game::Game(vector<Player*>& players){
    init(players);
}

void Game::init(vector<Player*>& players){
    //init players
    for (int i = 0; i < players.size(); i++)
    {
        this->players.push_back(players.at(i));
    }
    //init stacks
    for (int i = 0; i < 2; i++)
    {
        stacksList[i] = new Stack(i, true);
        stacksList[i + 2] = new Stack(i, false);
    }
    //init deck
    for (int i = 0; i <= 99; i++)
    {
        deck.push_back(i);
    }
    gameState = 0;
    minCardsToPlace = 2;
    //distrib cards
    for (int i = 0; i < players.size(); ++i)
    {
        for (int j = 0; j < NB_CARDS_START; ++j) {
            players.at(i)->dealCard(dealRandomCard());
        }
    }
    turnPlayer(0);
}

void Game::turnPlayer(int playerIndex)
{
    Player* player = players.at(playerIndex);
    bool isMoveValid = false;
    bool isPlaying = true;
    int card;
    int stack;
    int nbCardsPlayed = 0;
    while (isPlaying){
        while(!isMoveValid){
            //wait player's move
            while(player->getCardToPlace() == -1 && player->getStackToPlace() == -1){}
            card = player->getCardToPlace();
            stack = player->getStackToPlace();
            if(canPlaceCard(card, stack)){
                isMoveValid = true;
            }
            player->resetChoice(isMoveValid);
        }
        stacksList[stack]->setTopCard(card);
        if(!player->getIsTurn()){
            if(nbCardsPlayed >= minCardsToPlace){
                isPlaying = false;
            } else {
                //can't end turn
            }
        }
    }
}

bool Game::canPlaceCard(int card, int stack)
{
    //crescent stack && value >
    if(stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() < card){
        return true;
    }
    //decrescent stack && value <
    else if(!stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() > card){
        return true;
    }
    //crescent stack && value < 10
    else if(!stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() == card + 10){
        return true;
    }
    //decrescent stack && value > 10
    else if(!stacksList[stack]->getIsCrescent() && stacksList[stack]->getTopCard() == card - 10){
        return true;
    }
    return false;
}

int Game::dealRandomCard(){
    srand(time(0));
    int index = rand() % this->deck.size();
    int result = this->deck.at(index);
    this->deck.erase(this->deck.begin() + index);
    return result;
}

Game::~Game(){
    for (int i = 0; i <= 3; i++)
    {
        delete stacksList[i];
    }
}