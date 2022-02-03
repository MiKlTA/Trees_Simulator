#ifndef H_TREE
#define H_TREE



#include <vector>

#include "opengl.h"

#include "field.h"



class Tree
{
public:
    
    Tree(Field *f, glm::ivec2 seedPos);
    
    void step();
    void mutate();
    
    bool dead() const;
    
private:
    Field *m_field;
    glm::ivec2 m_seedPos;
    Dna_t m_dna;
    unsigned int m_carriage;
    
    std::vector<glm::ivec2> m_parts;
    std::vector<glm::ivec2> m_seeds;
    
    int m_energy;
    bool m_dead;
    
    
    
    glm::ivec2 toFieldPos(glm::ivec2 p) {return m_seedPos + p;}
    bool haveTilesNear(glm::ivec2 p) const;
    
    bool canGrow() {return m_carriage < m_dna.size() && m_energy >= growCost();}
    void grow();
    void die();
    
    void harvestEnergy();
    void subtractEnergy();
    int calcEnergy(int x, int startY);
    
    bool allSeedsReady();
    
    void createSeed(glm::ivec2 p);
    void createPart(glm::ivec2 p);
    
    void randomAddToDna();
    void randomChangeInDna();
    void randomRemoveFromDna();
    
    
    
    static int growCost() {return 200;}
    static int chanceMutation() {return 10;}
    static int chanceAdding() {return 10;}
    static int chanceChange() {return 5;}
    static int chanceRemove() {return 20;}
    static int chancesSum()
    {
        return chanceAdding() + chanceChange() + chanceRemove();
    }
};



#endif // H_TREE
