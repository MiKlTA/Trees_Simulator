#include "window.h"



Window::Window()
    : m_window(),
      m_monSize(),
      m_camPos(0.0f),
      m_camScale(1.0f),
      m_viewMat(),
      m_viewSize(),
      m_projMat(),
      
      m_field(nullptr),
      m_simcore(nullptr)
{
    glfwInit();
    
    {int x, y;
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(),
                           &x, &y, &m_monSize.x, &m_monSize.y);}
    m_window = glfwCreateWindow(
                m_monSize.x, m_monSize.y,
                "Margarita is the best!",
                nullptr, nullptr
                );
    
    //glfwSetKeyCallback(window, keyCallback);
    
    glfwMakeContextCurrent(m_window);
    glfwMaximizeWindow(m_window);
    glfwSwapInterval(1);
    
    glewExperimental = true;
    glewInit();
    
    m_viewSize = glm::vec2(m_monSize.x/(m_monSize.y * 1.0f), 1.0f);
    m_projMat = glm::ortho(
                -m_viewSize.x/2.f, m_viewSize.x/2.f,
                -m_viewSize.y/2.f, m_viewSize.y/2.f
                           );
    m_camPos = glm::vec2(0.0f);
    m_camScale = 1.0f;
    updViewMat();
}



void Window::startWindowCycle()
{
    glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
    
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        handleInputCameraMove();
        
        m_simcore->step();
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderBackground();
        m_field->render(m_viewMat, m_projMat);
        
        glfwSwapBuffers(m_window);
    }
}



// private:



void Window::handleInputCameraMove()
{
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_camPos.x += 10.0f;
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_camPos.x -= 10.0f;
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camPos.y += 10.0f;
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camPos.y -= 10.0f;
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_camScale *= 1.1f;
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_camScale /= 1.1f;
        updViewMat();
    }
}



void Window::renderBackground()
{
    
}



void Window::updViewMat()
{
    m_viewMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_camScale));
    m_viewMat = glm::translate(m_viewMat, -glm::vec3(m_camPos, 0.0f));
}
