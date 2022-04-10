#include <iostream>
#include <thread>
#include <string>

#include "../socket.h"
#include "../Player.h"

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[])
{
    int port;
    Player *player;
    string message;

    if (argc != 3)
    {
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
    StreamSocket* socket = new StreamSocket(argv[1], port);

    //connexion
    int err = socket->connect();
    if (err != 0) {
        delete socket;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server.\n";

    //starting game
    socket->read(message);
    int id = message.at(5) - '0';
    cout << message << endl << to_string(id) << endl;
    if(message != "START" + to_string(id))
    {
        cout << "Connection failed" << endl;
        delete socket;
        return 0;
    }
    player = new Player(id, socket);
    player->sendMessage("CONNE1");
    cout << "Game started ! \n" << "id : " << to_string(id) << endl;

    while(true)
    {
        socket->read(message);
        cout << "Server :\t" << message << endl;
        cin >> message;
        socket->send(message);
    }

    //    delete sock;
    return 0;
}
