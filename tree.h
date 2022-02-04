#ifndef H_TREE
#define H_TREE



#include <vector>
#include <fstream>

#include "opengl.h"

#include "field.h"



class Tree
{
public:
    Tree(Field *f);
    Tree(Field *f, glm::ivec2 seedPos);
    
    void step();
    void mutate();
    
    void cheatingGrow() {grow();}
    
    bool isDead() const {return m_dead;}
    
    void save(std::ofstream &out);
    void load(std::ifstream &in);
    void print(std::ofstream &out);
    
private:
    Field *m_field;
    glm::ivec2 m_seedPos;
    Dna_t m_dna;
    unsigned int m_carriage;
    
    std::vector<glm::ivec2> m_parts;
    std::vector<glm::ivec2> m_seeds;
    
    int m_energy;
    int m_energyStorage;
    bool m_dead;
    
    
    
    glm::ivec2 toFieldPos(glm::ivec2 p) {return m_seedPos + p;}
    bool haveTilesNear(glm::ivec2 p) const;
    int countSeedsInDna() const;
    
    bool canGrow() {return m_carriage < m_dna.size() && m_energy >= growCost();}
    void grow();
    void die();
    
    void determineEnergy();
    void distributeEnergy(int energy);
    int subtractEnergy(int energy)
    {return energy - m_parts.size() - m_parts.size();}
    int calcEnergy(int x, int startY);
    
    bool allSeedsReady();
    
    void randomAddToDna();
    void randomChangeInDna();
    void randomRemoveFromDna();
    
    
    
    int growCost() {return m_energyStorage;}
    static int chanceMutation() {return 5;}
    static int chanceAdding() {return 25;}
    static int chanceChange() {return 10;}
    static int chanceRemove() {return 5;}
    static int chancesSum()
    {
        return chanceAdding() + chanceChange() + chanceRemove();
    }
};



#endif // H_TREE
