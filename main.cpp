#include "window.h"



int main()
{
    Window *w = Window::inst();
    
    Field *f = new Field;
    SimulationCore *sc = new SimulationCore(f);
    w->setField(f);
    w->setSimcore(sc);
    
    w->startWindowCycle();
    
    return 0;
}
