#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include <vector>
#include "socket.h"
#include "../talk.h"
#include "../Game.h"
#include "../Player.h"

using namespace std;
using namespace stdsock;

void startGame(vector<Player*>);

int NB_PLAYERS = 2;

int main()
{
    vector<vector<Player*>> lobbiesList;
    ConnectionPoint *server = new ConnectionPoint(3490);
    int nbPlayers = 0;

    int err= server->init();
    std::cout << server->getIP() << ":" << server->getPort() << std::endl;
    if (err != 0) {
#ifdef _WIN32
        char buf[100];
        std::cout << strerror_s(buf,sizeof(buf),err) << std::endl;
#else
        std::cout << strerror(err) << std::endl;
#endif
        exit(err);
    }

    while (true)
    {
        StreamSocket* client = server->accept();

        if (!client->valid())
        {
            delete client;
            continue;
        }
        else
        {
            //add new player in lobbiesList
            if(nbPlayers == 0 || nbPlayers % NB_PLAYERS == 0)
            {
                vector<Player*> vector;
                lobbiesList.push_back(vector);
            }
            lobbiesList.at(lobbiesList.size() - 1).push_back(new Player(nbPlayers, client));
            nbPlayers ++;

            //create new Game
            if(nbPlayers % NB_PLAYERS == 0)
            {
                cout << "New game" << endl;
                thread(startGame, lobbiesList.at(lobbiesList.size() - 1)).detach();
            }
        }
    }
    delete server;
    return 0;
}

void startGame(vector<Player*> playersList)
{
    new Game(playersList);
}
