#ifndef H_FIELD
#define H_FIELD



#include <set>
#include <queue>

#include "opengl.h"

#include "tile.h"
#include "rect.h"



struct Plate
{
    int bottom, top;
    std::set<Plate *> parentPlates, childPlates;
    
    bool operator>(const Plate &p)
    {
        return top > p.top;
    }
};

using Layer = std::priority_queue<Plate *>;



class Field
{
public:
    Field();
    ~Field();
    
    void render(const glm::mat4 &view, const glm::mat4 &proj);
    
    
    
    int width() const {return 200;};
    int height() const {return 50;};
    
private:
    Rect *m_rect;
    Tile ***m_tiles;
    
    Layer *m_layers;
};



#endif // H_FIELD
