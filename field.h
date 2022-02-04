#ifndef H_FIELD
#define H_FIELD



#include <fstream>

#include "opengl.h"

#include "tile.h"
#include "rect.h"



class Field
{
public:
    Field();
    ~Field();
    
    void render(const glm::mat4 &view, const glm::mat4 &proj);
    
    Tile * getTile(glm::ivec2 p) {return m_tiles[p.y][normalizeXCoord(p.x)];}
    void setTile(glm::ivec2 p, Tile *t)
    {m_tiles[p.y][normalizeXCoord(p.x)] = t;}
    void clear();
    
    int normalizeXCoord(int x)
    {
        if (x >= 0)
            return x % width();
        else
            return (width() - ((-x) % width())) % width();
    }
    
    glm::vec3 linearInterp(glm::vec3 v1, glm::vec3 v2, float t)
    {return t * v1 + (1.f - t) * v2;}
    
    void save(std::ofstream &out);
    void load(std::ifstream &in);
    
    
    
    static constexpr glm::vec3 sky1Color()
    {return glm::vec3(0.75f, 1.f, 1.f);}
    static constexpr glm::vec3 sky2Color()
    {return glm::vec3(0.5f, 0.65f, 0.7f);}
    static constexpr glm::vec3 leafColor()
    {return glm::vec3(0.3f, 1.f, 0.25f);}
    static constexpr glm::vec3 woodColor()
    {return glm::vec3(0.3f, 0.2f, 0.1f);}
    static constexpr glm::vec3 seedColor()
    {return glm::vec3(0.75f, 0.7f, 0.6f);}
    
    static int width() {return 200;};
    static int height() {return 50;};
    bool inRange(int y) {return (y >= 0 && y < height());}
    
private:
    Rect *m_rect;
    Tile ***m_tiles;
    
    void prepareRenderTile(glm::ivec2 p);
};



#endif // H_FIELD
