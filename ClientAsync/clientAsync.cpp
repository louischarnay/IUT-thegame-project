#include <iostream>
#include <thread>
#include <sys/time.h>
#include <string>
#include <ncurses.h>
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
void coutStacks();

int DECK_SIZE = 6;

Player *player;
char cards[300] = "Vos cartes sont: ";
char mesg1[] = "Choisissez une stack: ";
char mesg12[] = "Merci de jouer une stack valide !";
char mesg2[] = "Choisissez une carte: ";
char mesg22[] = "Merci de jouer une carte valide !";
char mesg3[] = "Carte jouée !";
char mesg4[] = "Impossible de jouer cette carte sur cette stack !";
char mesg5[] = "Veuillez patienter, ce n'est pas votre tour.";
char mesg6[] = "Vous pouvez envoyer un message aux autres participants.";
char mesg7[] = "Impossible de jouer.";
char mesg8[50] = "Tour du joueur ";
char str1[2];
char str2[2];
char str3[250];
int cpt = 0;

int main(int argc, char* argv[])
{
    WINDOW *boite;

    char *title = "[ THE GAME ]";
    char str4[2];
    char str5[2];
    char str6[2];


    int port;

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

    int err = socket->connect();
    if (err != 0) {
        delete socket;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server.\n";

    player = new Player(0, socket);

    bool lobby = true;

    initscr();

    char *loading = "Loading...";

    //Attente participants

    clear();
    mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
    mvprintw(LINES / 2 - 2, (COLS / 2) - (strlen(loading) / 2), loading);
    refresh();
    string message;
    message = player->readMessage();
    connexion(message);
    clear();

    //Le joueur reçoit les cartes
    fillDeck();



    strcat(cards, player->getCards().c_str());

    bool game = true;
    bool turnP = true;
    bool card = true;


    bool isGameEnd = false;
    while(!isGameEnd)
    {
        isGameEnd = turn();
    }
    while(game){
        clear();
        cpt = 0;
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        while (!turnP){
            mvprintw(LINES / 4 + cpt, (strlen(mesg5) / 2) - (strlen(mesg5) / 2), mesg5);
            cpt++;
            mvprintw(LINES / 4 + cpt, (strlen(mesg6) / 2) - (strlen(mesg6) / 2), mesg6);
            getstr(str3);
            cpt++;
            //Le joueur envoie un message contenu dans str3
        }

        while (turnP){ //player->getCanPlay()
            card = true;
            mvprintw(LINES / 4 + cpt, (strlen(mesg1) / 2) - (strlen(mesg1) / 2), mesg1);
            getstr(str1);
            cpt++;
            if (strcmp(str1, "1") == 0 || strcmp(str1, "2") == 0 || strcmp(str1, "3") == 0 || strcmp(str1, "4") == 0){
                while (card){
                    mvprintw(LINES / 4 + cpt, (strlen(mesg2) / 2) - (strlen(mesg2) / 2), mesg2);
                    getstr(str2);
                    cpt++;
                    for (int i = 0; i < player->getDeckSize(); i++){
                        if (strcmp(str2, player->getCards(i).c_str()) == 0){
                            if (player->isMoveValid(atoi(str2), atoi(str1) - 1)){
                                message = "PLAYT";
                                message += str2 + to_string(atoi(str1) - 1);
                                socket->send(message);
                                //le joueur joue la carte str2 sur la stack str1 - 1
                                card = false;
                            }
                            else {
                                mvprintw(LINES / 4 + cpt, (strlen(mesg4) / 2) - (strlen(mesg4) / 2), mesg4);
                                cpt++;
                            }
                        }
                        else if (strcmp(str2, "/") == 0){
                            card = false;
                        }
                    }
                }    
            }
            else if (strcmp(str1, "/") == 0){
                //Le joueur passe son tour
                turnP = false;
            }
            else {
                mvprintw(LINES / 4 + cpt, (strlen(mesg12) / 2) - (strlen(mesg12) / 2), mesg12);
                cpt++;
            }
        }
    }
    
    endwin();
    
    free(boite);

    return 0;
}

void yourTurn()
{
    string message;
    while(true) {
        player->setCanPlay();
        if (!player->getCanPlay()) {
            clear();
            mvprintw(LINES / 4, (strlen(mesg7) / 2) - (strlen(mesg7) / 2), mesg7);
            refresh();
            return;
        }
        message = player->readMessage();
        if(getMessagePrefix(message).compare("PLAYT") == 0) {
            coutStacks();
            bool card = true;
            mvprintw(LINES / 4 + cpt, (strlen(mesg1) / 2) - (strlen(mesg1) / 2), mesg1);
            getstr(str1);
            cpt++;
            if (strcmp(str1, "1") == 0 || strcmp(str1, "2") == 0 || strcmp(str1, "3") == 0 || strcmp(str1, "4") == 0) {
                while (card) {
                    mvprintw(LINES / 4 + cpt, (strlen(mesg2) / 2) - (strlen(mesg2) / 2), mesg2);
                    getstr(str2);
                    cpt++;
                    for (int i = 0; i < player->getDeckSize(); i++) {
                        if (strcmp(str2, player->getCards(i).c_str()) == 0) {
                            if (player->isMoveValid(atoi(str2), atoi(str1) - 1)) {
                                message = "PLAYT";
                                message += str2 + to_string(atoi(str1) - 1);
                                //socket->send(message);
                                //le joueur joue la carte str2 sur la stack str1 - 1
                                card = false;
                            } else {
                                mvprintw(LINES / 4 + cpt, (strlen(mesg4) / 2) - (strlen(mesg4) / 2), mesg4);
                                cpt++;
                            }
                        } else if (strcmp(str2, "/") == 0) {
                            card = false;
                        }
                    }
                }
            } else if (strcmp(str1, "/") == 0) {
                //Le joueur passe son tour
            } else {
                mvprintw(LINES / 4 + cpt, (strlen(mesg12) / 2) - (strlen(mesg12) / 2), mesg12);
                cpt++;
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
        message = player->readMessage();
        int card = getMessageSuffix(message);
        player->dealCard(card);
        player->sendMessage("DISTR1");
    }
}

void connexion(string message)
{
    if(getMessagePrefix(message) == "START")
    {
        player->setId(getMessageSuffix(message));
        player->sendMessage("START1");
    }
    else
    {
        player->sendMessage("START0");
    }

}

void coutStacks()
{
    string stack = "[" + to_string(player->getStack(0)->getTopCard()) + "]- [" + to_string(player->getStack(1)->getTopCard()) + "]+ [" + to_string(player->getStack(2)->getTopCard()) + "]- [" + to_string(player->getStack(3)->getTopCard()) + "]+";
    char stacks[300] = "";
    strcpy(stacks, stack.c_str());
    mvprintw(LINES / 4 + cpt, (strlen(stacks) / 2) - (strlen(stacks) / 2), stacks);
    cpt++;
    mvprintw(LINES / 4 + cpt, (strlen(cards) / 2) - (strlen(cards) / 2), cards);
    cpt++;
}