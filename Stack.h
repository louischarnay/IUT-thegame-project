#include <string>

#ifndef STACK_H
#define STACK_H

using namespace std;

class Stack
{
private:
    int id;
    bool isCrescent;
    int topCard;
public:
    Stack(int id, bool isCrescent);
    int getId() { return id; }
    bool getIsCrescent() { return isCrescent; }
    int getTopCard() { return topCard; }
    void setTopCard(int card) { topCard = card; }
    string toString();
};

#endif