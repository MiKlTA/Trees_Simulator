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
    
    
    int width() {return 200;};
    int height() {return 50;};
    
private:
    Rect *m_rect;
    Tile ***m_tiles;
};



#endif // H_FIELD
