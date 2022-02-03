#include "simulation_core.h"



SimulationCore::SimulationCore(Field *f)
    : m_field(f),
      m_trees(),
      m_paused(true)
{
    Tile *t = new Tile;
    t->spec = -1;
    t->storedEnergy = t->maxStoredEnergy();
    t->isFalling = true;
    DnaCodon_t codon;
    codon.x = 0;
    codon.y = 0;
    codon.spec = Tile::maxGrn() - 1;
    t->dna.push_back(codon);
    codon.x = 0;
    codon.y = 1;
    codon.spec = -1;
    t->dna.push_back(codon);
    m_field->setTile({100, 1}, t);
}



void SimulationCore::step()
{
    if (!m_paused)
        forcedStep();
}

void SimulationCore::forcedStep()
{
    for (auto pt : m_trees)
    {
        pt->step();
    }
    
    for (int x = 0; x < m_field->width(); ++x)
    {
        for (int y = 0; y < m_field->height(); ++y)
        {
            glm::ivec2 p(x, y);
            Tile *cur = m_field->getTile(p);
            if (cur != nullptr)
            {
                if (cur->spec == -1 && cur->isFalling)
                {
                    moveSeed(p);
                }
            }
        }
    }
}



// private



void SimulationCore::moveSeed(glm::ivec2 p)
{
    glm::ivec2 next(p.x, p.y - 1);
    Tile *cur = m_field->getTile(p);
    if (m_field->getTile(next) == nullptr)
    {
        m_field->setTile(next, cur);
        m_field->setTile(p, nullptr);
        if (next.y == 0)
        {
            m_trees.push_back(new Tree(m_field, next));
        }
    }
    else
    {
        delete cur;
        m_field->setTile(p, nullptr);
    }
}
