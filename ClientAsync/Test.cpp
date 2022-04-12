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
void turn(StreamSocket);
bool isYourTurn(string);

int DECK_SIZE = 6;
Player* player;

int main(int argc, char* argv[])
{
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
    }

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

    turn(*socket);
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

void turn(StreamSocket socket)
{
    string message;
    message = player->readMessage();
    player->sendMessage("TURN1");
    if(isYourTurn(message))
    {
        int card = -1;
        int stack = 0;
        while(!player->isMoveValid(card, stack))
        {
            cout << "card : ";
            cin >> card;
            cout << endl << "stack : ";
            cin >> stack;
        }
        player->sendMessage("PLAYT" + to_string(stack) + to_string(card));
        socket.read(message);
        cout << message << endl;
    }
}

bool isYourTurn(string message)
{
    if(getMessageSuffix(message) == player->getId())
        return true;
    return false;
}