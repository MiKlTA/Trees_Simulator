#include <iostream>
#include <random>

#include "window.h"



int main()
{
    int seed;
    std::cin >> seed;
    std::srand(seed);
    
    Window *w = Window::inst();
    
    Field *f = new Field;
    SimulationCore *sc = new SimulationCore(f);
    w->setField(f);
    w->setSimcore(sc);
    
    w->startWindowCycle();
    
    return 0;
}
