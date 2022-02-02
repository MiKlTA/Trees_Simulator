#include "window.h"



int main()
{
    Window w;
    
    Field *f = new Field;
    SimulationCore *sc = new SimulationCore;
    w.setField(f);
    w.setSimcore(sc);
    
    w.startWindowCycle();
    
    return 0;
}
