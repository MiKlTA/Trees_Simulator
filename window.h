#ifndef H_WINDOW
#define H_WINDOW



#include <set>
#include <fstream>

#include "opengl.h"

#include "simulation_core.h"
#include "field.h"



class Window
{
public:
    static Window * inst();
    
    int getMonitorWidth() const {return m_monSize.x;};
    int getMonitorHeight() const {return m_monSize.y;};
    
    void setField(Field *f) {m_field = f;}
    void setSimcore(SimulationCore *sc) {m_simcore = sc;}
    
    
    
    void startWindowCycle();
    
private:
    Window();
    ~Window();
    static Window *m_instance;
    
    GLFWwindow *m_window;
    glm::ivec2 m_monSize;
    
    glm::vec2 m_camPos;
    float m_camScale;
    glm::mat4 m_viewMat;
    glm::vec2 m_viewSize;
    glm::mat4 m_projMat;
    
    glm::vec2 m_mousePos;
    
    Field *m_field;
    SimulationCore *m_simcore;
    Field *m_previewField;
    Tree *m_previewTree;
    bool m_previewIsOn;
    
    
    
    void handleInputCameraMove();
    glm::ivec2 getMouseFieldPos();
    
    void setPreview(bool previewIsOn);
    
    void updViewMat();
    void updProjMat();
    
    
    
    static void keyCallback(
            GLFWwindow *aWindow, int key, int scancode, int action, int mode
            );
    static void cursorPosCallback(GLFWwindow* window, double x, double y)
    {Window::inst()->m_mousePos = glm::vec2(x, y);}
    static void mouseButtonCallback(
            GLFWwindow* window, int button, int action, int mods
            );
    
    static float dCamPos() {return 1.f;}
    static float kCamScale() {return 1.01f;}
};



#endif // H_WINDOW
