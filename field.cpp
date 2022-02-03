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
            m_tiles[y][x] = nullptr;
        }
    }
}

Field::~Field()
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            if (m_tiles[y][x] != nullptr)
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
        for (int x = 0; x < width(); ++x)
        {
            prepareRenderTile({x, y});
            m_rect->render(view, proj);
        }
}



// private:



void Field::prepareRenderTile(glm::ivec2 p)
{
    m_rect->setPos(glm::vec2(p.x + 0.5f, p.y + 0.5f));
    Tile *cur = getTile(p);
    
    glm::vec3 color;
    if (cur == nullptr)
    {
        color = linearInterp(
                    sky2Color(), sky1Color(),
                    p.y / (height() * 1.f)
                    );
    }
    else
    {
        if (cur->spec == -1)
            color = seedColor();
        else
            color = linearInterp(
                        woodColor(),
                        leafColor(),
                        getTile(p)->storedEnergy / (Tile::numSpec() * 1.f)
                        );
    }
    m_rect->setColor(color);
}
