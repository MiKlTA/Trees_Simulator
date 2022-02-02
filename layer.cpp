#include "layer.h"



void Layer::add(int y)
{
    
}



// private



void Layer::merge(Plate *reciever, Plate *recipient)
{
    if (reciever->top > recipient->top)
    {
        reciever->bottom = recipient->bottom;
    }
    else
    {
        reciever->top = recipient->top;
    }
    // TODO:
}
