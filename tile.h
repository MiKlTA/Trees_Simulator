#ifndef H_TILE
#define H_TILE



#include <vector>



struct DnaCodon_t
{
    int x, y, spec;
    bool isSeed() const {return spec == -1;}
};
using Dna_t = std::vector<DnaCodon_t>;



struct Tile
{
    int spec; // (-1) - seed, 0 - wood, ..., (maxGrn() - 1) - leaf
    int storedEnergy;
    bool isFalling;
    
    Dna_t dna;
    
    bool isSeed() const {return spec == -1;}
    
    
    
    
    static int maxGrn() {return 8;}
    static int specCount() {return 9;}
    static int specShift() {return -1;}
    static int maxStoredEnergy() {return 10;}
};



#endif // H_TILE
