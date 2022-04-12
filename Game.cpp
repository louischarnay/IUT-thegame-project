#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Game.h"
#include "Player.h"

using namespace std;

Game::Game(vector<Player*>& players)
{
    srand(time(0));
    init(players);
}

void Game::init(vector<Player*>& Players){
    //init players
    for (int i = 0; i < Players.size(); i++) {
        players.push_back(Players.at(i));
        //connexion
        players.at(i)->sendMessage("START" + to_string(players.at(i)->getId()));
        string response = players.at(i)->readMessage();
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
            players.at(i)->sendMessage("DISTR" + to_string(card));
            players.at(i)->readMessage();
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

void Game::turnPlayer(int playerIndex) {
    Player *player = players.at(playerIndex);
    sendMessageToEveryone("TURNP" + to_string(player->getId()));
    cout << "Turn Player " << to_string(player->getId()) << endl;

    int cardValue;
    int stackIndex;
    int nbCardsPlayed = 0;
    string message = "";

    //player->startTurn(stacksList);
    while (true) {
        //check if player can play
        player->setCanPlay();
        if (!player->getCanPlay()) {
            if (nbCardsPlayed < minCardsToPlace)
            {
                cout << "niqué fin de game" << endl;
                gameState = 1; // end of game
            }
            return;
        }

        //get player's move
        player->sendMessage("PLAYT1");
        message = player->readMessage();
        if (getMessagePrefix(message) == "PLAYT") {
            int tmp = getMessageSuffix(message);
            cardValue = tmp % 100;
            stackIndex = tmp / 100;

            //place card
            if(cardValue > 9)
            {
                sendMessageToEveryone("POSTT" + to_string(stackIndex) + to_string(cardValue));
            } else
            {
                sendMessageToEveryone("POSTT" + to_string(stackIndex) + "0" + to_string(cardValue));
            }
            stacksList[stackIndex]->setTopCard(cardValue);
            player->placeCard(cardValue, stackIndex);
            nbCardsPlayed++;
            cout << "Card placed" << endl;
        } else if (getMessagePrefix(message) == "ENDTU") {
            if (nbCardsPlayed >= minCardsToPlace) {
                player->sendMessage("ENDTU1");
                player->readMessage();
                cout << "End of turn player " << to_string(player->getId()) << endl << "nb cards played : " << to_string(nbCardsPlayed) << endl;
                //deals cards
                for (int i = 0; i < nbCardsPlayed; ++i) {
                    int card = dealRandomCard();
                    players.at(playerIndex)->dealCard(card);
                    players.at(playerIndex)->sendMessage("DISTR" + to_string(card));
                    players.at(playerIndex)->readMessage();
                }
                return;
            } else {
                player->sendMessage("ENDTU0");
            }
        }
    }
}

int Game::dealRandomCard(){
    if(deck.size() == 0)
    {
        minCardsToPlace = 1;
        return -1;
    }
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
    sendMessageToEveryone("ENDGA " + to_string(nbCardsRemaining));
    ///ENDGA idPlayer nbCardsRemaining
    cout << "End of game ! " << endl << "Points : " << to_string(nbCardsRemaining) << endl;
}

void Game::sendMessageToEveryone(string message)
{
    for (int i = 0; i < players.size(); ++i) {
        players.at(i)->sendMessage(message);
        players.at(i)->readMessage();
    }
}

string Game::getMessagePrefix(string message)
{
    return message.substr(0, 5);
}

int Game::getMessageSuffix(string message)
{
    message = message.substr(5, message.length() - 5);
    cout << message << endl;
    return stoi(message);
}

Game::~Game(){
    for (int i = 0; i <= 3; i++)
    {
        delete stacksList[i];
    }
}