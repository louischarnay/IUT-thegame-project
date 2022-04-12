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
void turn();
bool isYourTurn(string);
string toString();

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

    turn();

    while(true)
    {
        cout << player->readMessage();
        string tmp;
        cin >> tmp;
        player->sendMessage(tmp);
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
    message = message.substr(5, message.length() - 5);
    cout << message << endl;
    return stoi(message);
}

void fillDeck()
{
    string message;
    while(player->getDeckSize() < DECK_SIZE)
    {
        message = player->readMessage();
        string tmp = message.substr(6, message.length() - 5);
        int card = getMessageSuffix(message);
        player->dealCard(card);
        player->sendMessage("DISTR1");
    }
}

void turn()
{
    string message;
    message = player->readMessage();
    player->sendMessage("TURN1");
    if(isYourTurn(message))
    {
        while(true)
        {
            message = player->readMessage();
            cout << "message : " << message << endl;
            if(getMessagePrefix(message).compare("PLAYT") == 0)
            {
                int card = -1;
                int stack = -1;
                int var = 1;
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
                    player->placeCard(card, stack);
                    player->sendMessage("POSTT1");
                }
            } else if(getMessagePrefix(message).compare("ENDGA") == 0)
            {
                cout << "End of game" << endl;
                return;
            }

        }
    }
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