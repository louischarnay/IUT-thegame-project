#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"

using namespace std;

Game::Game(vector<Player*>& players)
{
    init(players);
}

void Game::init(vector<Player*>& players){
    //init players
    for (int i = 0; i < players.size(); i++)
    {
        this->players.push_back(players.at(i));
    }
    //init stacks
    for (int i = 0; i < 4; i++)
    {
        stacksList[i] = new Stack(i, i % 2);
    }
    //init deck
    for (int i = 2; i <= 99; i++)
    {
        deck.push_back(i);
    }
    gameState = 0;
    minCardsToPlace = 2;
    //distrib cards
    for (int i = 0; i < players.size(); ++i)
    {
        for (int j = 0; j < NB_CARDS_START; ++j) {
            int card = dealRandomCard();
            players.at(i)->dealCard(card);
            ///DISTR card (1)
        }
    }
    int cpt = -1;
    while(gameState == 0)
    {
        cpt ++;
        turnPlayer(cpt % players.size());
    }
    endOfGame(cpt % players.size());

}

void Game::turnPlayer(int playerIndex)
{
    Player* player = players.at(playerIndex);
    ///TURNP playerIndex (everybody)

    //bool isMoveValid = false;
    bool isPlaying = true;
    int cardValue;
    int stackIndex;
    int nbCardsPlayed = 0;

    player->startTurn(stacksList);
    while(isPlaying)
    {
        //get player's move
        player->setCardAndStackTEST(); //TEST
        player->isEndOfTurnTEST(); //TEST
        if(!player->getCanPlay())
        {
            if(nbCardsPlayed >= minCardsToPlace)
            {
                isPlaying = false;
            }
            else
            {

                gameState = 1;
                return;
            }
        }
        if(!player->getIsTurn())
        {
            if(nbCardsPlayed >= minCardsToPlace)
            {
                ///ENDTU 1
                isPlaying = false;
            }
            else
            {
                ///ENDTU 0
            }
        }
        /*while(player->getCardToPlace() == -1 && player->getStackToPlace() == -1) // wait player's turn
        {
            isPlaying = player->getIsTurn();
        }*/
        cardValue = player->getCardToPlace();
        stackIndex = player->getStackToPlace();

        if(cardValue != -1 && stackIndex != -1)
        {
            cout << "want to play card : " << to_string(cardValue) << " on stack : " << to_string(stackIndex) << endl;
            if(canPlaceCard(cardValue, stackIndex))
            {
                ///PLAYT 1 (1)
                ///POSTT stackIndex cardValue everybody
                cout << "card placed\n";
                stacksList[stackIndex]->setTopCard(cardValue);
                cout << this->toString();
                player->resetChoice(true);
                nbCardsPlayed++;
            } else
            {
                ///PLAYT 0 (1)
                cout << "card not placed\n";
                player->resetChoice(false);
            }
        }
    }
    cout << "end of turn player : " << player->getId() << endl;
    for (int i = 0; i < nbCardsPlayed; ++i)
    {
        player->dealCard(dealRandomCard());
    }
    cout << "server : " << endl <<this->toString();
}

bool Game::canPlaceCard(int card, int stack)
{
    //crescent stack && value >, decrescent stack && value <, crescent stack && value < 10, decrescent stack && value > 10
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

int Game::dealRandomCard(){
    if(deck.size() == 0)
    {
        minCardsToPlace = 1;
        return -1;
    }
    srand(time(0));
    int index = rand() % deck.size();
    int result = deck.at(index);
    deck.erase(deck.begin() + index);
    return result;
}

string Game::toString()
{
    string result = "game state : " + to_string(gameState);
    result += "  |  min cards to place : " + to_string(minCardsToPlace);
    result += "  |  deck size : " + to_string(deck.size()) + "\n";
    for (int i = 0; i < 4; ++i) {
        result += stacksList[i]->toString();
    }
    result += "\n";
    return result;
}

void Game::endOfGame(int idPlayer)
{
    int nbCardsRemaining = deck.size();
    for (int i = 0; i < players.size(); ++i) {
        nbCardsRemaining += players.at(i)->getDeckSize();
    }
    ///ENDGA idPlayer nbCardsRemaining
    cout << "End of game ! " << endl << "Points : " << to_string(nbCardsRemaining) << endl;
}

Game::~Game(){
    for (int i = 0; i <= 3; i++)
    {
        delete stacksList[i];
    }
}