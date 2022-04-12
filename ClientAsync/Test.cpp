#include <iostream>
#include <thread>
#include <string>
#include "../socket.h"
#include "../talk.h"
#include "../Player.h"
#include "../Game.h"
#include "../Stack.h"

using namespace std;
using namespace stdsock;

string getMessagePrefix(string);
int getMessageSuffix(string);
void fillDeck();
void connexion(string);
bool turn();
bool isYourTurn(string);
string toString();
void yourTurn();
void otherTurn();

int DECK_SIZE = 6;
Player* player;

int main(int argc, char* argv[])
{
    int port;

    if (argc != 3) {
        printf("usage: %s server_address port\n", argv[0]);
        return 0;
    }

#ifdef _WIN32
    if (sscanf_s(argv[2], "%d", &port) != 1)
#else
    if (sscanf(argv[2], "%d", &port) != 1)
#endif
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 1;
    }

    StreamSocket *socket = new StreamSocket(argv[1], port);

    int err = socket->connect();
    if (err != 0) {
        delete socket;
        perror("[-]Error in connection: ");
        return (err);
    }
    cout << "[+]Connected to Server.\n";

    player = new Player(0, socket);

    string message;
    message = player->readMessage();
    connexion(message);

    fillDeck();
    cout << "Deck : ";
    for (int i = 0; i < player->getDeckSize(); ++i) {
        cout << player->getDeck().at(i) << "\t";
    }
    cout << endl;
    bool isGameEnd = false;
    while(!isGameEnd)
    {
        isGameEnd = turn();
    }
}

void connexion(string message)
{
    cout << getMessagePrefix(message) << " zebi" << endl;
    if(getMessagePrefix(message) == "START")
    {
        player->setId(getMessageSuffix(message));
        player->sendMessage("START1");
    }
    else
    {
        player->sendMessage("START0");
    }
    cout << "Connected to game, id : " << to_string(player->getId()) << endl;

}

string getMessagePrefix(string message)
{
    return message.substr(0, 5);
}

int getMessageSuffix(string message)
{
    return stoi(message.substr(5, message.length() - 5));
}

void fillDeck()
{
    string message;
    while(player->getDeckSize() < DECK_SIZE)
    {
        cout << "deck size : " << to_string(player->getDeckSize()) << endl;
        message = player->readMessage();
        int card = getMessageSuffix(message);
        player->dealCard(card);
        player->sendMessage("DISTR1");
    }
}

bool turn()
{
    string message;
    message = player->readMessage();
    if(getMessagePrefix(message).compare("TURNP") == 0)
    {
        player->sendMessage("TURNP1");
        if(isYourTurn(message)) {
            yourTurn();
        } else
        {
            cout << "Turn player " << to_string(getMessageSuffix(message)) << endl;
        }
    } else if(getMessagePrefix(message).compare("POSTT") == 0)
    {
        int tmp = getMessageSuffix(message);
        player->placeCard(tmp % 100, tmp / 100, false);
        player->sendMessage("POSTT1");

    } else if(getMessagePrefix(message).compare("ENDGA") == 0)
    {
        player->sendMessage("ENDGA1");
        cout << "End of game !\n Total score : " << to_string(getMessageSuffix(message)) << endl;
        return true;
    }
    return false;
}

bool isYourTurn(string message)
{
    if(getMessageSuffix(message) == player->getId())
        return true;
    return false;
}

string toString()
{
    string result = "";
    result += "Stacks : \n";
    for (int i = 0; i < 4; ++i) {
        result += "stack id : " + to_string(i + 1)
                + " is crescent : " + to_string(player->getStack(i)->getIsCrescent())
                + " top card : " + to_string(player->getStack(i)->getTopCard())
                + "\n";
    }
    result += "Deck : \n";
    for (int i = 0; i < player->getDeckSize(); ++i) {
        result += to_string(player->getDeck().at(i)) + "\t";
    }
    result += "\n";
    return result;
}

void yourTurn()
{
    string message;
    while(true)
    {
        player->setCanPlay();
        if(!player->getCanPlay())
        {
            cout << "Can't play" << endl;
            return;
        }
        message = player->readMessage();
        cout << "message : " << message << endl;
        if(getMessagePrefix(message).compare("PLAYT") == 0)
        {
            int card = -1;
            int stack = -1;
            while(!player->isMoveValid(card, stack))
            {
                cout << toString();
                cout << "card : ";
                cin >> card;
                cout << endl << "stack : ";
                cin >> stack;
                stack --;

                //end of turn
                if(card == -2)
                {
                    player->sendMessage("ENDTU");
                    message = player->readMessage();
                    if(getMessageSuffix(message) == 1)
                    {
                        cout << "End of turn" << endl;
                        player->sendMessage("ENDTU1");
                        fillDeck();
                        return;
                        //end of turn
                    } else
                    {
                        cout << "Can't end your turn" << endl;
                    }
                }
            }

            //send card + stack
            if(card > 9)
            {
                player->sendMessage("PLAYT" + to_string(stack) + to_string(card));
            } else
            {
                player->sendMessage("PLAYT" + to_string(stack) + "0" + to_string(card));
            }
            message = player->readMessage();
            if(getMessagePrefix(message).compare("POSTT") == 0)
            {
                player->placeCard(card, stack, true);
                player->sendMessage("POSTT1");
            }
        } else if(getMessagePrefix(message).compare("ENDGA") == 0)
        {
            cout << "End of game" << endl;
            return;
        }

    }
}

void otherTurn()
{
    while (true)
    {
        string message;
        message = player->readMessage();
        cout << message;
        player->sendMessage("zebi");
    }
}