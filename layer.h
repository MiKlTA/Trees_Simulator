#ifndef H_LAYER
#define H_LAYER



#include <set>

#include "tile.h"



struct Plate
{
    int top, bottom;
    std::set<Plate *> parentPlates, childPlates;
    
    bool operator>(const Plate &p)
    {
        return top > p.top;
    }
};



class Layer
{
public:
    void add(int y);
    void addedToParent(int y);
    void addedToChild(int y);
    void remove(int y);
    void removedToParent(int y);
    void removedToChild(int y);
    
private:
    std::set<Plate *> m_plates;
    
    void merge(Plate *reciever, Plate *recipient);
};


#endif // H_LAYER
