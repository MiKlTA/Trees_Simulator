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

void Field::clear()
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            if (m_tiles[y][x] != nullptr)
                delete m_tiles[y][x];
            m_tiles[y][x] = nullptr;
        }
    }
}



void Field::save(std::ofstream &out)
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            if (m_tiles[y][x] == nullptr)
            {
                out << 0 << ' ';
            }
            else
            {
                out << 1 << ' ';
                out << m_tiles[y][x]->spec  << ' ';
                out << m_tiles[y][x]->isFalling << ' ';
                out << m_tiles[y][x]->storedEnergy << ' ';
                out << m_tiles[y][x]->dna.size() << ' ';
                for (DnaCodon_t c : m_tiles[y][x]->dna)
                {
                    out << c.x << ' '
                        << c.y << ' '
                        << c.spec << ' ';
                }
            }
        }
    }
}

void Field::load(std::ifstream &in)
{
    for (int y = 0; y < height(); ++y)
    {
        for (int x = 0; x < width(); ++x)
        {
            if (m_tiles[y][x] != nullptr)
            {
                delete m_tiles[y][x];
                m_tiles[y][x] = nullptr;
            }
            
            bool notEmpty;
            in >> notEmpty;
            if (notEmpty)
            {
                Tile *t = new Tile;
                in >> t->spec;
                in >> t->isFalling;
                in >> t->storedEnergy;
                int dnaSize;
                in >> dnaSize;
                for (int i = 0; i < dnaSize; ++i)
                {
                    DnaCodon_t c;
                    in >> c.x >> c.y >> c.spec;
                    t->dna.push_back(c);
                }
                
                m_tiles[y][x] = t;
            }
        }
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
        if (cur->isSeed())
            color = seedColor();
        else
            color = linearInterp(
                        leafColor(),
                        woodColor(),
                        getTile(p)->spec / (Tile::maxGrn() * 1.f)
                        );
    }
    m_rect->setColor(color);
}
