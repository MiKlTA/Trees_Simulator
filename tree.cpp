#include "tree.h"



Tree::Tree(Field *f, glm::ivec2 seedPos)
    : m_field(f),
      m_seedPos(seedPos),
      m_dna(),
      m_carriage(0),
      
      m_parts(),
      m_seeds(),
      
      m_energy(),
      m_dead(false)
{
    Tile *seed = f->getTile(seedPos);
    m_energy = seed->storedEnergy;
    m_dna = seed->dna;
    if (rand() % chanceMutation() == 0)
        mutate();
    
    delete seed;
    f->setTile(seedPos, nullptr);
    grow();
}



void Tree::step()
{
    if (m_dead) return;
    
    harvestEnergy();
    subtractEnergy();
    if (canGrow())
    {
        m_energy -= growCost();
        grow();
    }
    if (allSeedsReady() && m_carriage == m_dna.size())
        die();
}

void Tree::mutate()
{
    int rnd = rand() % chancesSum();
    if (rnd < chanceAdding())
    {
        randomAddToDna();
    }
    else if (rnd < chanceAdding() + chanceChange())
    {
        randomChangeInDna();
    }
    else if (rnd < chanceAdding() + chanceChange() + chanceRemove())
    {
        randomRemoveFromDna();
    }
}



// private:



bool Tree::haveTilesNear(glm::ivec2 p) const
{
    for (auto i : m_parts)
    {
        if (i.x == p.x)
        {
            if (i.y == p.y + 1 || i.y == p.y - 1)
                return true;
        }
        if (i.y == p.y)
        {
            if (i.x == p.x + 1 || i.x == p.x - 1)
                return true;
        }
    }
    return false;
}



void Tree::grow()
{
    auto c = m_dna[m_carriage];
    
    glm::ivec2 p = toFieldPos({c.x, c.y});
    
    if (
            !(
                (haveTilesNear(p) || m_carriage == 0)
                && m_field->getTile(p) == nullptr
                && m_field->inRange(p.y)
                )
            )
    {
        m_carriage++;
        return;
    }
    
    Tile *t = new Tile;
    t->spec = c.spec;
    t->dna = m_dna;
    t->storedEnergy = 0;
    t->isFalling = false;
    
    m_field->setTile(p, t);
    if (c.spec == -1)
        m_seeds.push_back(p);
    else
        m_parts.push_back(p);
    
    m_carriage++;
}

void Tree::die()
{
    m_dead = true;
    
    for (auto p : m_parts)
    {
        if (m_field->getTile(p) != nullptr)
        {
            delete m_field->getTile(p);
            m_field->setTile(p, nullptr);
        }
    }
    
    for (auto s : m_seeds)
    {
        m_field->getTile(s)->isFalling = true;
    }
}



void Tree::harvestEnergy()
{
    int energy = 0;
    for (auto pt : m_parts)
    {
        energy += m_field->getTile(pt)->spec * calcEnergy(pt.x, pt.y + 1);
    }
    
    energy /= 2;
    m_energy += energy + (energy % 2);
    
    if (!m_seeds.empty())
    {
        int energyPerSeed = energy / m_seeds.size();
        int rem = energy % m_seeds.size();
        for (auto s : m_seeds)
        {
            auto cur = m_field->getTile(s);
            if (cur->storedEnergy < cur->maxStoredEnergy())
            {
                if (rem == 0)
                {
                    rem--;
                    cur->storedEnergy++;
                }
                cur->storedEnergy += energyPerSeed;
                cur->storedEnergy = std::max(
                            cur->maxStoredEnergy(), cur->storedEnergy
                            );
            }
        }
    }
}

void Tree::subtractEnergy()
{
    m_energy -= m_parts.size();
    m_energy -= m_seeds.size();
    if (m_energy <= 0)
        m_dead = true;
}

int Tree::calcEnergy(int x, int startY)
{
    int energy = 10;
    for (int y = startY; y < m_field->height(); ++y)
    {
        Tile *cur = m_field->getTile({x, y});
        if (cur != nullptr)
        {
            if (cur->spec != -1)
            {
                energy = std::max(0, energy - 2 * (cur->spec + 1));
            }
        }
    }
    return energy;
}

bool Tree::allSeedsReady()
{
    bool ready = true;
    for (auto s : m_seeds)
    {
        auto cur = m_field->getTile(s);
        ready &= cur->storedEnergy == cur->maxStoredEnergy();
    }
    return ready;
}





void Tree::randomAddToDna()
{
    DnaCodon_t codon;
    codon.spec = rand() % (Tile::maxGrn() + 1);
    while (true)
    {
        int i = rand() % m_dna.size();
        glm::ivec2 randP(m_dna[i].x, m_dna[i].y);
        randP.x += (rand() % 3) - 1;
        randP.y += (rand() % 3) - 1;
        for (auto c : m_dna)
        {
            if (c.x == randP.x && c.y == randP.y)
                goto cycleBreak;
        }
    }
    cycleBreak:
    m_dna.push_back(codon);
}

void Tree::randomChangeInDna()
{
    int i = rand() % m_dna.size();
    m_dna[i].spec = rand() % (Tile::maxGrn() + 1);
}

void Tree::randomRemoveFromDna()
{
    if (m_dna.size() <= 1) return;
    m_dna.erase(m_dna.begin() + rand() % m_dna.size());
}
