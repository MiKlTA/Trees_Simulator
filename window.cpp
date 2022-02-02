#include "window.h"



Window::Window(Field *f, SimulationCore *sc)
    : m_field(f),
      m_simcore(sc)
{
    glfwInit();
    
    {int x, y;
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(),
                           &x, &y, &m_monSize.x, &m_monSize.y);}
    GLFWwindow *window = glfwCreateWindow(
                m_monSize.x, m_monSize.y,
                "Margarita is the best!",
                nullptr, nullptr
                );
    
    //glfwSetKeyCallback(window, keyCallback);
    
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
    glfwSwapInterval(1);
}



void Window::startWindowCycle()
{
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        
        m_simcore->step();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        m_field->render();
        
        glfwSwapBuffers(m_window);
    }
}
