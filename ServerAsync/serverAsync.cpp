#include <iostream>
#include <thread>

#include <sys/time.h>
#include "socket.h"
#include "talk.h"

using namespace std;
using namespace stdsock;

int NB_CLIENTS = 3;

int main(int argc, char *argv[])
{
    Talk com[NB_CLIENTS];
    std::thread threads[NB_CLIENTS];

    int port;
    if(argc!=2 || sscanf(argv[1], "%d", &port)!=1)
    {
        printf("usage: %s port\n", argv[0]);
        // default port, if none provided
        port= 3490;
    }

    ConnectionPoint *server=new ConnectionPoint(port);
    int err= server->init();
    if (err != 0) {
        std::cout << strerror(err) << std::endl;
        exit(err);
    }

    cout << "Waiting clients on port " << port << " ..." << endl;


    StreamSocket *clients[NB_CLIENTS];
    for (int i = 0; i < NB_CLIENTS; ++i) {
        clients[i] = server->accept();
        com[i].setReader(clients[i]->getSockfd());
        com[i].setWriter(-1);
        cout << "connexion client " << i << "\n";
    }

    // creating threads for communications handling
    int cpt = 0;
    for (int i = 0; i < NB_CLIENTS; ++i) {
            threads[i] = std::thread(talk, std::ref(com[i]));
    }

    // waiting end of communication
    cpt = 0;
    for (int i = 0; i < NB_CLIENTS; i++) {
        threads[i].join();
        printf("Deconnexion client %c \n", (i));
        delete(clients[i]);
    }


    // closing connexion point
    delete server;
    cout << "stop\n";
    return 0;
}
