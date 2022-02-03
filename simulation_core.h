#ifndef H_SIMULATION_CORE
#define H_SIMULATION_CORE



#include <vector>

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
    
private:
    Field *m_field;
    std::vector<Tree *> m_trees;
    
    bool m_paused;
    
    
    
    void moveSeed(glm::ivec2 p);
};



#endif // H_SIMULATION_CORE
