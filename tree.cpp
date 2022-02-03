#include "tree.h"




Tree::Tree(Field *f, glm::ivec2 seedPos)
    : m_field(f),
      m_seedPos(seedPos),
      m_dna(),
      m_carriage(0),
      
      m_plates()
{
    
}



void Tree::step()
{
    
}



// private:



bool Tree::haveFriendCellsNear(glm::ivec2 p) const
{
    
}



void Tree::mergePlates(Plate *rvr, Plate *cip)
{
    if (rvr->left < cip->left)
    {
        rvr->right = cip->right;
    }
    else
    {
        rvr->left = cip->right;
    }
    
    for (Plate *fp : cip->m_foundPlates)
        rvr->m_foundPlates.push_back(fp);
    
    for (Plate *hp : cip->m_hangingPlates)
        rvr->m_foundPlates.push_back(hp);
}

void Tree::getPlatesNear(
        glm::ivec2 p, Plate *&l, Plate *&r, Plate *&u, Plate *&d
        )
{
    l = r = u = d = nullptr;
    for (auto pp : m_plates)
    {
        if (intoSegment(pp->left, pp->right, p.x))
        {
            if (pp->y == p.y + 1) u = pp; 
            if (pp->y == p.y - 1) d = pp; 
        }
        
        if (pp->y == p.y)
        {
            if (pp->right == p.x - 1) l = pp;
            if (pp->left == p.x + 1) r = pp;
        }
    }
}



void Tree::add(Tile *t)
{
    glm::ivec2 p(t->x, t->y);
    
    Plate *l, *r, *u, *d;
    getPlatesNear(p, l, r, u, d);
    bool eL = (l != nullptr),
         eR = (r != nullptr),
         eU = (u != nullptr),
         eD = (d != nullptr);
    
    if (eL && eR)
    {
        mergePlates(l, r);
        eR = false;
    }
    else if (eL)
    {
        
    }
    else if (eR)
    {
        
    }
    else
    {
        
    }
}

void Tree::rebuild()
{
    for (auto pt : m_plates)
    {
        delete pt;
    }
    m_plates.clear();
    
    bool wereChanges = true;
    bool checkedParts[m_parts.size()];
    for (unsigned int i = 0; i < m_parts.size(); ++i) checkedParts[i] = false;
    while (wereChanges)
    {
        wereChanges = false;
        for (unsigned int i = 0; i < m_parts.size(); ++i)
        {
            glm::ivec2 p(m_parts[i]->x, m_parts[i]->y);
            if ((!checkedParts[i]) && haveFriendCellsNear(p))
            {
                checkedParts[i] = true;
                wereChanges = true;
                add(m_field->getTile(toFieldPos(p)));
            }
        }
    }
    
    for (unsigned int i = m_parts.size() - 1; i >= 0; --i)
    {
        m_parts.erase(m_parts.begin() + i);
    }
}

void Tree::breakPlates()
{
    
}
