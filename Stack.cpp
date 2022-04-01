#include "Stack.h"

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
