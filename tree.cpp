#include "tree.h"



Tree::Tree(Field *f)
    : m_field(f)
{
    
}

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
    m_energyStorage = 0;
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
    
    determineEnergy();
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



void Tree::save(std::ofstream &out)
{
    out << m_seedPos.x << ' ';
    out << m_seedPos.y << ' ';
    out << m_dna.size() << ' ';
    for (int i = 0; i < int(m_dna.size()); ++i)
    {
        DnaCodon_t c = m_dna[i];
        out << c.x << ' '
            << c.y << ' '
            << c.spec << ' ';
    }
    out << m_carriage << ' ';
    
    out << m_parts.size() << ' ';
    for (int i = 0; i < int(m_parts.size()); ++i)
    {
        glm::ivec2 p = m_parts[i];
        out << p.x << ' '
            << p.y << ' ';
    }
    
    out << m_seeds.size() << ' ';
    for (int i = 0; i < int(m_seeds.size()); ++i)
    {
        glm::ivec2 p = m_seeds[i];
        out << p.x << ' '
            << p.y << ' ';
    }
    
    out << m_energy << ' ';
    out << m_energyStorage << ' ';
    out << m_dead << ' ';
}

void Tree::load(std::ifstream &in)
{
    in >> m_seedPos.x;
    in >> m_seedPos.y;
    int dnaSize;
    in >> dnaSize;
    m_dna.clear();
    for (int i = 0; i < dnaSize; ++i)
    {
        DnaCodon_t c;
        in >> c.x >> c.y >> c.spec;
        m_dna.push_back(c);
    }
    in >> m_carriage;
    
    int partsCount;
    in >> partsCount;
    m_parts.clear();
    for (int i = 0; i < partsCount; ++i)
    {
        glm::ivec2 p;
        in >> p.x >> p.y;
        m_parts.push_back(p);
    }
    
    int seedsCount;
    in >> seedsCount;
    m_seeds.clear();
    for (int i = 0; i < seedsCount; ++i)
    {
        glm::ivec2 p;
        in >> p.x >> p.y;
        m_seeds.push_back(p);
    }
    
    in >> m_energy;
    in >> m_energyStorage;
    in >> m_dead;
}

void Tree::print(std::ofstream &out)
{
    out << "Position: "
        << m_seedPos.x << ":"
        << m_seedPos.y  << std::endl;
    out << "Codons count: " << m_dna.size() << std::endl;
    out << "Codons: " << std::endl;
    
    for (int i = 0; i < int(m_dna.size()); ++i)
    {
        DnaCodon_t c = m_dna[i];
        out << c.x << ":" << c.y << " - "
            << c.spec << std::endl;
    }
    out << "Carriage: " << m_carriage << std::endl << std::endl;
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

int Tree::countSeedsInDna() const
{
    int count = 0;
    for (auto c : m_dna)
    {
        if (c.isSeed())
            count++;
    }
    return count;
}



void Tree::grow()
{
    const DnaCodon_t c = m_dna[m_carriage];
    
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
    if (c.isSeed())
        m_seeds.push_back(p);
    else
        m_parts.push_back(p);
    
    m_carriage++;
    m_energyStorage += std::max(t->spec, 0) + 1;
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



void Tree::determineEnergy()
{
    int energy = 0;
    for (auto pt : m_parts)
    {
        energy += m_field->getTile(pt)->spec * calcEnergy(pt.x, pt.y + 1);
    }
    energy = subtractEnergy(energy);
    
    if (energy > 0)
    {
        energy /= 2;
        m_energy += energy + (energy % 2);
        m_energy = std::min(m_energyStorage, m_energy);
        
        if (energy != 0)
            distributeEnergy(energy);
    }
    else
    {
        m_energy += energy;
    }
    
    if (m_energy <= 0)
        die();
}

void Tree::distributeEnergy(int energy)
{
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
                cur->storedEnergy = std::min(
                            cur->maxStoredEnergy(), cur->storedEnergy
                            );
            }
        }
    }
}

int Tree::calcEnergy(int x, int startY)
{
    int energy = Tile::maxGrn();
    for (int y = startY; y < m_field->height(); ++y)
    {
        Tile *cur = m_field->getTile({x, y});
        if (cur != nullptr)
            if (!cur->isSeed())
                energy = std::max(0, energy - 2 * (cur->spec + 1));
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
    while (true)
    {
        codon.spec = rand() % Tile::specCount() + Tile::specShift();
        int i = rand() % m_dna.size();
        
        codon.x = m_dna[i].x;
        codon.y = m_dna[i].y;
        
        codon.x += (rand() % 3) - 1;
        codon.y += (rand() % 3) - 1;
        
        int y = toFieldPos({codon.x, codon.y}).y;
        bool invalidCoords = (y >= m_field->height()) || (y < 0);
        bool placeIsOccupied = false;
        for (const DnaCodon_t c : m_dna)
            placeIsOccupied |= c.x == codon.x && c.y == codon.y;
        bool seedIsOnGround = (codon.isSeed() && codon.y == 0);
        if (!placeIsOccupied && !seedIsOnGround && !invalidCoords)
            break;
    }
    
    m_dna.push_back(codon);
}

void Tree::randomChangeInDna()
{
    if (m_dna.size() <= 2)
        return;
    
    int i;
    int spec;
    
    do
    {
        i = rand() % m_dna.size();
        spec = rand() % Tile::specCount() + Tile::specShift();
    }
    while ((m_dna[i].isSeed() && countSeedsInDna() == 1)
           || (spec == -1 && m_dna[i].y <= 0));
    
    m_dna[i].spec = spec;
    
    
    if (m_dna[i].isSeed())
    {
        int a;
        a = 0;
    }
    
}

void Tree::randomRemoveFromDna()
{
    if (m_dna.size() <= 2)
        return;
    
    int i = rand() % m_dna.size();
    while ((m_dna[i].isSeed() && countSeedsInDna() == 1)
           || (m_dna[i].x == 0 && m_dna[i].y == 0))
    {
        i = rand() % m_dna.size();
    }
    m_dna.erase(m_dna.begin() + i);
}
