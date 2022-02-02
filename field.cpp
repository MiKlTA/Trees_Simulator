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


void Field::render(const glm::mat4 &view, const glm::mat4 &proj)
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            m_rect->setPos(glm::vec2(x, y));
            m_rect->render(view, proj);
        }
    }
}
