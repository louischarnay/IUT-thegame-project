#include <iostream>
#include <thread>
#include <sys/time.h>
#include "socket.h"
#include "talk.h"
#include <ncurses.h>
#include "Player.h"
#include "Stack.h"
#include <string>

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[])
{
    WINDOW *boite;

    Player player(0);

    char *title = "[ THE GAME ]";
    char str4[2];
    char str5[2];
    char str6[2];
    int cpt = 0;
    char lobby_mesg1[] = "Créer une partie (c)";
    char lobby_mesg2[] = "Rejoindre une partie (j)";
    char lobby_mesg3[] = "Indiquez la lettre correspondante à l'action c/j: ";
    char lobby_mesg4[] = "Indiquez le nombre max de participants: ";
    char lobby_mesg5[] = "Indiquez l\'identifiant du salon que vous souhaitez rejoindre: ";

    bool lobby = true;

    initscr();
    while (lobby){
        clear();
        cpt = 0;
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        mvprintw(LINES / 4 + cpt, (strlen(lobby_mesg1) / 2) - (strlen(lobby_mesg1) / 2), lobby_mesg1);
        cpt++;
        mvprintw(LINES / 4 + cpt, (strlen(lobby_mesg2) / 2) - (strlen(lobby_mesg2) / 2), lobby_mesg2);
        cpt++;
        mvprintw(LINES / 4 + cpt, (strlen(lobby_mesg3) / 2) - (strlen(lobby_mesg3) / 2), lobby_mesg3);
        cpt++;
        getstr(str4);
        if (strcmp(str4, "c") == 0){
            mvprintw(LINES / 4 + cpt, (strlen(lobby_mesg4) / 2) - (strlen(lobby_mesg4) / 2), lobby_mesg4);
            cpt++;
            getstr(str5);
            lobby = false;
            //crée la room avec str5 participants max
        }
        else if (strcmp(str4, "j") == 0){
            //affichage des rooms
            mvprintw(LINES / 4 + cpt, (strlen(lobby_mesg5) / 2) - (strlen(lobby_mesg5) / 2), lobby_mesg5);
            cpt++;
            getstr(str6);
            lobby = false;
            //rejoint la room str6
        }
    }

    char *loading = "Loading...";

    //Attente participants
    
    while(1) {
        clear();
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        mvprintw(LINES / 2 - 2, (COLS / 2) - (strlen(loading) / 2), loading);
        refresh();
        if(getch() != 410)
            break;
    }

    //Le joueur reçoit les cartes
    player.dealCard(1);
    player.dealCard(2);
    player.dealCard(3);

    string stack = "[" + to_string(player.getStacks(0)->getTopCard()) + "]- [" + to_string(player.getStacks(1)->getTopCard()) + "]+ [" + to_string(player.getStacks(2)->getTopCard()) + "]- [" + to_string(player.getStacks(3)->getTopCard()) + "]+";
    char stacks[300] = "";
    strcpy(stacks, stack.c_str());

    char cards[300] = "Vos cartes sont: ";
    char mesg1[] = "Choisissez une stack: ";
    char mesg12[] = "Merci de jouer une stack valide !";
    char mesg2[] = "Choisissez une carte: ";
    char mesg22[] = "Merci de jouer une carte valide !";
    char mesg3[] = "Carte jouée !";
    char mesg4[] = "Impossible de jouer cette carte sur cette stack !";
    char mesg5[] = "Veuillez patienter, ce n'est pas votre tour.";
    char mesg6[] = "Vous pouvez envoyer un message aux autres participants.";
    char str1[2];
    char str2[2];
    char str3[250];

    strcat(cards, player.getCards().c_str());

    bool game = true;
    bool turn = true;
    bool card = true;

    while(game){
        clear();
        cpt = 0;
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        mvprintw(LINES / 4 + cpt, (strlen(stacks) / 2) - (strlen(stacks) / 2), stacks);
        cpt++;
        mvprintw(LINES / 4 + cpt, (strlen(cards) / 2) - (strlen(cards) / 2), cards);
        cpt++;
        while (!turn){
            mvprintw(LINES / 4 + cpt, (strlen(mesg5) / 2) - (strlen(mesg5) / 2), mesg5);
            cpt++;
            mvprintw(LINES / 4 + cpt, (strlen(mesg6) / 2) - (strlen(mesg6) / 2), mesg6);
            getstr(str3);
            cpt++;
            //Le joueur envoie un message contenu dans str3
        }

        while (turn){ //player.getCanPlay()
            card = true;
            mvprintw(LINES / 4 + cpt, (strlen(mesg1) / 2) - (strlen(mesg1) / 2), mesg1);
            getstr(str1);
            cpt++;
            if (strcmp(str1, "1") == 0 || strcmp(str1, "2") == 0 || strcmp(str1, "3") == 0 || strcmp(str1, "4") == 0){
                while (card){
                    mvprintw(LINES / 4 + cpt, (strlen(mesg2) / 2) - (strlen(mesg2) / 2), mesg2);
                    getstr(str2);
                    cpt++;
                    for (int i = 0; i < player.getDeckSize(); i++){
                        if (strcmp(str2, player.getCards(i).c_str()) == 0){
                            if (player.isMoveValid(atoi(str2), atoi(str1) - 1)){
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
                turn = false;
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