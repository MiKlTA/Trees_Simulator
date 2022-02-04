#include "window.h"



Window *Window::m_instance = nullptr;



Window * Window::inst()
{
    if (m_instance == nullptr)
    {
        m_instance = new Window;
    }
    return m_instance;
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
        
        if (!m_previewIsOn)
            m_field->render(m_viewMat, m_projMat);
        else
            m_previewField->render(m_viewMat, m_projMat);
        
        glfwSwapBuffers(m_window);
    }
}



// private:



Window::Window()
    : m_window(),
      m_monSize(),
      m_camPos(),
      m_camScale(),
      m_viewMat(),
      m_viewSize(),
      m_projMat(),
      
      m_mousePos(0.0f),
      
      m_field(nullptr),
      m_simcore(nullptr),
      m_previewField(nullptr),
      m_previewIsOn(false)
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
    
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, cursorPosCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    
    glfwMakeContextCurrent(m_window);
    glfwMaximizeWindow(m_window);
    glfwSwapInterval(1);
    
    glewExperimental = true;
    glewInit();
    
    m_viewSize = glm::vec2(m_monSize.x/(m_monSize.y * 1.0f), 1.0f);
    m_camScale = 70.0f;
    updProjMat();
    m_camPos = glm::vec2(100.0f, 25.0f);
    updViewMat();
    
    m_previewField = new Field;
}

Window::~Window()
{
    delete m_previewField;
}



void Window::handleInputCameraMove()
{
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_camPos.x += dCamPos();
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_camPos.x -= dCamPos();
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camPos.y += dCamPos();
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camPos.y -= dCamPos();
        updViewMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_camScale *= kCamScale();
        updProjMat();
    }
    if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
    {
        m_camScale /= kCamScale();
        updProjMat();
    }
}

glm::ivec2 Window::getMouseFieldPos()
{
    int height, width;
    glfwGetWindowSize(m_window, &width, &height);
    
    glm::vec2 p(m_mousePos.x, height - m_mousePos.y);
    p.x /= (width * 1.0f);
    p.y /= (height * 1.0f);
    
    glm::vec2 screenSize = m_viewSize * m_camScale;
    p *= screenSize;
    
    p += m_camPos - screenSize / 2.0f;
    return glm::ivec2(p.x, p.y);
}



void Window::setPreview(bool previewIsOn)
{
    if (previewIsOn)
    {
        glm::ivec2 p = getMouseFieldPos();
        if (p.x >= m_field->width() || p.x < 0
                || p.y >= m_field->height() || p.y < 0)
            return;
        Tile *target = m_field->getTile(p);
        if (target == nullptr)
            return;
        
        Tile *t = new Tile;
        t->dna = target->dna;
        t->spec = -1;
        t->isFalling = false;
        t->storedEnergy = 1;
        glm::ivec2 seedPos(100, 0);
        m_previewField->setTile(seedPos, t);
        
        m_previewTree = new Tree(m_previewField, seedPos);
        while (m_previewTree->canCheatingGrow())
            m_previewTree->cheatingGrow();
        
        m_previewIsOn = true;
    }
    else
    {
        m_previewField->clear();
        delete m_previewTree;
        
        m_previewIsOn = false;
    }
}



void Window::updViewMat()
{
    m_viewMat = glm::translate(glm::mat4(1.0f), -glm::vec3(m_camPos, 0.0f));
}

void Window::updProjMat()
{
    m_projMat = glm::ortho(
                -m_viewSize.x/2.f * m_camScale, m_viewSize.x/2.f * m_camScale,
                -m_viewSize.y/2.f * m_camScale, m_viewSize.y/2.f * m_camScale
                           );
}



void Window::keyCallback(
        GLFWwindow *aWindow, int key, int scancode, int action, int mode
        )
{
    Window *w = Window::inst();
    
    switch (key)
    {
    case GLFW_KEY_Z:
        switch (action)
        {
        case GLFW_PRESS:
            w->m_camScale = 70.0f;
            w->updProjMat();
            w->m_camPos = glm::vec2(100.0f, 25.0f);
            w->updViewMat();
            break;
        }
        break;
    case GLFW_KEY_P:
        switch (action)
        {
        case GLFW_PRESS:
            w->m_simcore->pause();
            break;
        }
        break;
    case GLFW_KEY_SPACE:
        switch (action)
        {
        case GLFW_PRESS:
            w->m_simcore->forcedStep();
            break;
        }
        break;
    case GLFW_KEY_K:
        switch (action)
        {
        case GLFW_PRESS:
            std::ofstream out("save.txt");
            if (!out.is_open())
                return;
            w->m_simcore->save(out);
            out.close();
            break;
        }
        break;
    case GLFW_KEY_L:
        switch (action)
        {
        case GLFW_PRESS:
            std::ifstream in("save.txt");
            if (!in.is_open())
                return;
            w->m_simcore->load(in);
            in.close();
            break;
        }
        break;
    case GLFW_KEY_R:
        switch (action)
        {
        case GLFW_PRESS:
            std::ofstream out("data.txt");
            if (!out.is_open())
                return;
            w->m_simcore->print(out);
            out.close();
            break;
        }
        break;
    case GLFW_KEY_V:
        switch (action)
        {
        case GLFW_PRESS:
            w->setPreview(!w->m_previewIsOn);
        }
        break;
    }
}

void Window::mouseButtonCallback(
        GLFWwindow* window, int button, int action, int mods
        )
{
    Window *w = Window::inst();
    
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        switch (action)
        {
        case GLFW_PRESS:
            w->setPreview(true);
            break;
        case GLFW_RELEASE:
            w->setPreview(false);
            break;
        }
        break;
    }
}
