#ifndef H_TILE
#define H_TILE



#include <vector>



struct DnaCodon_t
{
    int x, y, spec;
};
using Dna_t = std::vector<DnaCodon_t>;



struct Tile
{
    int spec; // (-1) - seed, 0 - wood, ..., (maxGrn() - 1) - leaf
    int storedEnergy;
    bool isFalling;
    
    Dna_t dna;
    
    
    
    static int maxGrn() {return 8;}
    static int maxStoredEnergy() {return 1000;}
};



#endif // H_TILE
