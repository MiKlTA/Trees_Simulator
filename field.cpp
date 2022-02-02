#include "field.h"



Field::Field()
{
    m_rect = new Rect;
    
    m_tiles = new Tile**[height()];
    for (int y = 0; y < height(); ++y)
    {
        m_tiles[y] = new Tile*[width()];
        for (int x = 0; x < width(); ++x)
        {
            m_tiles[y][x] = new Tile;
        }
    }
}

Field::~Field()
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            delete m_tiles[y][x];
        }
        delete[] m_tiles[y];
    }
    delete[] m_tiles;
    
    delete m_rect;
}


#include <random>
void Field::render(const glm::mat4 &view, const glm::mat4 &proj)
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            m_rect->setPos(glm::vec2(x, y));
            m_rect->setColor(glm::vec3(rand()/(RAND_MAX * 1.0f), 0.5f, 0.7f));
            m_rect->render(view, proj);
        }
    }
}
