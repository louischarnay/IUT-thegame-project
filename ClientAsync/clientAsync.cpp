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
    char *loading = "Loading...";
    
    initscr();
    while(1) {
        clear();
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        mvprintw(LINES / 2 - 2, (COLS / 2) - (strlen(loading) / 2), loading);
        refresh();
        if(getch() != 410)
            break;
    }

    player.dealCard(1);
    player.dealCard(2);
    player.dealCard(3);

    string stack = "[" + to_string(player.getStacks(0)->getTopCard()) + "]+ [" + to_string(player.getStacks(1)->getTopCard()) + "]- [" + to_string(player.getStacks(2)->getTopCard()) + "]+ [" + to_string(player.getStacks(3)->getTopCard()) + "]-";
    char stacks[300] = "";
    strcpy(stacks, stack.c_str());

    char cards[300] = "Vos cartes sont: ";
    int cpt = 0;
    char mesg1[] = "Choisissez une stack: ";
    char mesg12[] = "Merci de jouer une stack valide !";
    char mesg2[] = "Choisissez une carte: ";
    char mesg22[] = "Merci de jouer une carte valide !";
    char mesg3[] = "Carte jouée !";
    char mesg4[] = "Impossible de jouer cette carte sur cette stack !";
    char str1[2];
    char str2[2];
    strcat(cards, player.getCards().c_str());

    bool game = true;
    bool card = true;

    while(game){
        clear();
        cpt = 0;
        mvprintw(0, (COLS / 2) - (strlen(title) / 2), title);
        mvprintw(LINES / 4 + cpt, (strlen(stacks) / 2) - (strlen(stacks) / 2), stacks);
        cpt++;
        mvprintw(LINES / 4 + cpt, (strlen(cards) / 2) - (strlen(cards) / 2), cards);
        cpt++;
        while (true){ //player.getCanPlay()
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
                            if (player.isMoveValid(atoi(str2), atoi(str1))){
                                //le joueur joue la carte str2 sur la stack str1
                                card = false;
                            }
                            else {
                                mvprintw(LINES / 4 + cpt, (strlen(mesg4) / 2) - (strlen(mesg4) / 2), mesg4);
                                cpt++;
                            }
                        }
                    }
                }    
            }
            else {
                mvprintw(LINES / 4 + cpt, (strlen(mesg12) / 2) - (strlen(mesg12) / 2), mesg12);
                cpt++;
            }
            break;
        }
        refresh();
    }
    
    endwin();
    
    free(boite);

    return 0;
}