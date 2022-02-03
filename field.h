#ifndef H_FIELD
#define H_FIELD



#include "opengl.h"

#include "tile.h"
#include "rect.h"



class Field
{
public:
    Field();
    ~Field();
    
    void render(const glm::mat4 &view, const glm::mat4 &proj);
    
    Tile * getTile(glm::ivec2 p) {return m_tiles[p.y][p.x];}
    void setTile(Tile *t, glm::ivec2 p) {m_tiles[p.y][p.x] = t;}
    
    int width() const {return 200;};
    int height() const {return 50;};
    
private:
    Rect *m_rect;
    Tile ***m_tiles;
};



#endif // H_FIELD
