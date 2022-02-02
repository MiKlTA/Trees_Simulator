#ifndef H_WINDOW
#define H_WINDOW



#include <set>

#include "opengl.h"

#include "keyboard_listener.h"
#include "simulation_core.h"
#include "field.h"



class Window
{
public:
    Window(Field *f, SimulationCore *sc);
    
    int getMonitorWidth() const {return m_monSize.x;};
    int getMonitorHeight() const {return m_monSize.y;};
    
    void startWindowCycle();
    
private:
    GLFWwindow *m_window;
    
    glm::ivec2 m_monSize;
    
    
    
    Field *m_field;
    SimulationCore *m_simcore;
    
    std::set<KeyboardListener *> m_keyboardLs;
};



#endif // H_WINDOW
