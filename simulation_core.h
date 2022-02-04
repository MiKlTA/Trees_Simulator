#ifndef H_SIMULATION_CORE
#define H_SIMULATION_CORE



#include <vector>
#include <iostream>
#include <fstream>

#include "field.h"

#include "tree.h"



class SimulationCore
{
public:
    SimulationCore(Field *f);
    
    void pause() {m_paused = !m_paused;}
    bool isPaused() {return m_paused;}
    
    void step();
    void forcedStep();
    
    void save(std::ofstream &out);
    void load(std::ifstream &in);
    void print(std::ofstream &out);
    
private:
    Field *m_field;
    std::vector<Tree *> m_trees;
    
    bool m_paused;
    long long m_stepsCount;
    
    
    
    void moveSeed(glm::ivec2 p);
};



#endif // H_SIMULATION_CORE
