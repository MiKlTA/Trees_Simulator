#ifndef H_RECT
#define H_RECT



#include "opengl.h"



class Rect
{
public:
    Rect();
    
    void setPos(glm::vec2 p) {m_pos = p; updModelMat();}
    void setSize(glm::vec2 s) {m_size = s; updModelMat();}
    
    glm::vec2 getPos() {return m_pos;}
    glm::vec2 getSize() {return m_size;}
    
    
    
    void setColor(glm::vec3 color) {m_color = color;}
    glm::vec3 getColor() {return m_color;}
    
    
    
    void render(const glm::mat4 &view, const glm::mat4 &proj);
    
private:
    GLuint m_VBO, m_EBO, m_VAO;
    GLuint m_shadProg;
    GLuint m_modelLoc, m_viewLoc, m_projLoc, m_colorLoc;
    glm::mat4 m_model;
    glm::vec3 m_color;
    
    glm::vec2 m_pos;
    glm::vec2 m_size;
    
    void updModelMat();
};



#endif // H_RECT
