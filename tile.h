#ifndef H_TILE
#define H_TILE



class Tree;



class Tile
{
public:
    int x, y;
    Tree *owner;
    
    
    
    Tile();
    
    
    
    int spec() const {return m_spec;}
    
    
    
    int numSpec() const {return 8;}
    
private:
    int m_spec; // 0 - leaf, (maxSpec - 1) - wood
};



#endif // H_TILE
