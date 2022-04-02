#include "Stack.h"
#include <string>

using namespace std;

Stack::Stack(int id, bool isCrescent)
{
    this->id = id;
    this->isCrescent = isCrescent;
    if(isCrescent)
    {
        this->topCard = 1;
    } else 
    {
        this->topCard = 100;
    }
}

string Stack::toString()
{
    string result = "id : " + to_string(id) + " | is crescent : " + to_string(isCrescent) + " | top card : " +
    to_string(topCard) + "\t";
    return result;
}
