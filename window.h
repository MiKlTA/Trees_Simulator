#ifndef H_WINDOW
#define H_WINDOW



#include <set>

#include "opengl.h"

#include "simulation_core.h"
#include "field.h"



class Window
{
public:
    static Window * inst();
    
    int getMonitorWidth() const {return m_monSize.x;};
    int getMonitorHeight() const {return m_monSize.y;};
    
    
    void setCamPos(glm::vec2 p) {m_camPos = p; updViewMat();}
    void setCamSize(float s) {m_camScale = s; updViewMat();}
    
    
    void setField(Field *f) {m_field = f;}
    void setSimcore(SimulationCore *sc) {m_simcore = sc;}
    
    
    
    void startWindowCycle();
    
private:
    Window();
    static Window *m_instance;
    
    GLFWwindow *m_window;
    glm::ivec2 m_monSize;
    
    glm::vec2 m_camPos;
    float m_camScale;
    glm::mat4 m_viewMat;
    glm::vec2 m_viewSize;
    glm::mat4 m_projMat;
    
    
    
    Field *m_field;
    SimulationCore *m_simcore;
    
    
    
    void handleInputCameraMove();
    
    void renderBackground();
    
    void updViewMat();
    void updProjMat();
    
    
    
    static void keyCallback(
            GLFWwindow *aWindow, int key, int scancode, int action, int mode
            );
    
    static float dCamPos() {return 7.f;}
    static float kCamScale() {return 1.07f;}
};



#endif // H_WINDOW
