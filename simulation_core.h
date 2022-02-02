#ifndef H_SIMULATION_CORE
#define H_SIMULATION_CORE



#include "field.h"



class SimulationCore
{
public:
    SimulationCore(Field *f);
    
    void step();
    
private:
    Field *m_field;
    
    
    
    void calcGravitation();
};



#endif // H_SIMULATION_CORE
