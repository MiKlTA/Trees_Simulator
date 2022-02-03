#ifndef H_TREE
#define H_TREE



#include <vector>
#include <algorithm>

#include "opengl.h"

#include "field.h"
#include "tile.h"



struct Plate
{
    int left, right;
    int y;
    std::vector<Plate *> m_foundPlates, m_hangingPlates;
    
    bool pointAtThisPlate(Plate *p)
    {
        const auto &f = m_foundPlates;
        const auto &h = m_hangingPlates;
        
        return (std::find(f.begin(), f.end(), p) != f.end())
                || (std::find(h.begin(), h.end(), p) != h.end());
    }
};



class Tree
{
public:
    Tree(Field *f, glm::ivec2 seedPos);
    
    void step();
    
    
    
private:
    Field *m_field;
    glm::ivec2 m_seedPos;
    std::vector<Tile *> m_dna;
    int m_carriage;
    
    std::vector<Tile *> m_parts;
    std::vector<Plate *> m_plates;
    
    
    
    glm::ivec2 toFieldPos(glm::ivec2 p) const {return m_seedPos + p;}
    glm::ivec2 toTreePos(glm::ivec2 p) const {return p - m_seedPos;}
    bool haveFriendCellsNear(glm::ivec2 p) const;
    
    void mergePlates(Plate *rvr, Plate *cip);
    void getPlatesNear(
            glm::ivec2 p, Plate *&l, Plate *&r, Plate *&u, Plate *&d
            );
    bool intoSegment(int l, int r, int x) {return (x >= l && x <= r);}
    
    void add(Tile *t);
    void rebuild();
    void breakPlates();
};



#endif // H_TREE
