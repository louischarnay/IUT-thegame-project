#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Game.h"
#include "Player.h"

using namespace std;

Game::Game(vector<Player*>& players)
{
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

void Game::turnPlayer(int playerIndex)
{
    Player* player = players.at(playerIndex);
    sendMessageToEveryone("TURNP" + to_string(playerIndex));
    cout << "test wesh";
    //cout << "Turn Player " << to_string(playerIndex) << endl;

    //bool isMoveValid = false;
    bool isPlaying = true;
    int cardValue;
    int stackIndex;
    int nbCardsPlayed = 0;

    cout << "zebi";
    player->startTurn(stacksList);
    while(isPlaying)
    {
        //get player's move

        //player->setCardAndStackTEST(); //TEST
        //player->isEndOfTurnTEST(); //TEST

        if(!player->getCanPlay())
        {
            //end of turn
            if(nbCardsPlayed >= minCardsToPlace)
            {
                isPlaying = false;
            }
            //end of game
            else
            {
                gameState = 1;
                return;
            }
        }
        //if player skip
        if(!player->getIsTurn())
        {
            if(nbCardsPlayed >= minCardsToPlace)
            {
                sendMessageToOne(player->getId(), "ENDTU1");
                cout << "End of turn player " << to_string(playerIndex) << endl;
                isPlaying = false;
            }
            else
            {
                sendMessageToOne(player->getId(), "ENDTU0");
            }
        }
        string move = player->readMessage();
        cout << "Player move : " << move << endl;
        if(getMessagePrefix(move) == "PLAYT")
        {

        }
        else if(getMessagePrefix(move) == "ENDTU")
        {

        }
        while(player->getCardToPlace() == -1 && player->getStackToPlace() == -1) // wait player's turn
        {
            isPlaying = player->getIsTurn();
        }
        cardValue = player->getCardToPlace();
        stackIndex = player->getStackToPlace();

        if(cardValue != -1 && stackIndex != -1)
        {
            //cout << "want to play card : " << to_string(cardValue) << " on stack : " << to_string(stackIndex) << endl;
            if(canPlaceCard(cardValue, stackIndex))
            {
                sendMessageToOne(player->getId(), "PLAYT1");
                sendMessageToEveryone("POSTT" + to_string(stackIndex) + to_string(cardValue));
                //cout << "card placed\n";
                stacksList[stackIndex]->setTopCard(cardValue);
                //cout << this->toString();
                player->resetChoice(true);
                nbCardsPlayed++;
            } else
            {
                sendMessageToOne(player->getId(), "PLAYT0");
                cout << "card not placed\n";
                player->resetChoice(false);
            }
        }
    }
    //cout << "end of turn player : " << player->getId() << endl;
    for (int i = 0; i < nbCardsPlayed; ++i)
    {
        player->dealCard(dealRandomCard());
    }
    //cout << "server : " << endl <<this->toString();
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
    sendMessageToEveryone("ENDGA " + to_string(idPlayer) + " " + to_string(nbCardsRemaining));
    ///ENDGA idPlayer nbCardsRemaining
    cout << "End of game ! " << endl << "Points : " << to_string(nbCardsRemaining) << endl;
}

string Game::sendMessageToEveryone(string message)
{
    cout << message << endl;
    for (int i = 0; i < players.size(); ++i) {
        players.at(i)->sendMessage(message);
        players.at(i)->readMessage();
    }
    cout << "sent to everyone" << endl;
}

string Game::sendMessageToOne(int id, string message)
{
    players.at(id)->sendMessage(message);
    string response = "";
    while(response == "")
    {
        response = players.at(id)->readMessage();
        cout << response << endl;
    }
    return response;
}

string Game::getMessagePrefix(string message)
{
    return message.substr(0, 5);
}

int Game::getMessageSuffix(string message)
{
    return message.at(5) + '0';
}

Game::~Game(){
    for (int i = 0; i <= 3; i++)
    {
        delete stacksList[i];
    }
}