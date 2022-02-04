#include "simulation_core.h"



SimulationCore::SimulationCore(Field *f)
    : m_field(f),
      m_trees(),
      
      m_paused(true),
      m_stepsCount(0)
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
    m_stepsCount++;
    std::cout << m_stepsCount << std::endl;
    if (m_stepsCount == 1804)
    {
        int a = 0;
        a++;
    }
    
    for (int i = m_trees.size() - 1; i >= 0; --i)
    {
        Tree *t = m_trees[i];
        if (t->isDead())
        {
            m_trees.erase(m_trees.begin() + i);
            continue;
        }
        t->step();
    }
    
    for (int x = 0; x < m_field->width(); ++x)
    {
        for (int y = 0; y < m_field->height(); ++y)
        {
            glm::ivec2 p(x, y);
            Tile *cur = m_field->getTile(p);
            if (cur != nullptr)
                if (cur->isSeed() && cur->isFalling)
                {
                    if (y == 0)
                    {
                        m_field->setTile(p, nullptr);
                        delete cur;
                    }
                    else
                        moveSeed(p);
                }
        }
    }
}



void SimulationCore::save(std::ofstream &out)
{
    out << m_stepsCount << std::endl;
    m_field->save(out);
    
    out << m_trees.size() << std::endl;
    for (Tree *t : m_trees)
        t->save(out);
}

void SimulationCore::load(std::ifstream &in)
{
    in >> m_stepsCount;
    m_field->load(in);
    
    for (Tree *t : m_trees)
        delete t;
    m_trees.clear();
    
    int treesCount = 0;
    in >> treesCount;
    for (int i = 0; i < treesCount; ++i)
    {
        Tree *t = new Tree(m_field);
        t->load(in);
        m_trees.push_back(t);
    }
}

void SimulationCore::print(std::ofstream &out)
{
    out << "Trees count: " << m_trees.size() << " |" << std::endl;
    for (Tree *t : m_trees)
        t->print(out);
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
