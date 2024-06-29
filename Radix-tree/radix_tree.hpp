
#include "tree.hpp"
#include <cstdint>

class RadixTreeNode
{
  public:
    RadixTreeNode(){
        for (int i = 0; i < 4; i++)
        {
            children[i] = nullptr;
        }
        height = 1;
        isLeaf = false;
        bit = 0;
    };
    RadixTreeNode(uint8_t bit, uint8_t height = 1, bool isLeaf = false) : bit(bit), height(height), isLeaf(isLeaf)
    {
        for (int i = 0; i < 4; i++)
        {
            children[i] = nullptr;
        }
    };
    ~RadixTreeNode(){
        for (int i = 0; i < 4; i++)
        {
            if (children[i] != nullptr)
            {
                delete children[i];
            }
        }
    };
    RadixTreeNode *children[4];
    uint8_t bit;
    uint8_t height;
    bool isLeaf;
};

class RadixTree : public Tree
{
    // To Be Implemented
  private:
    RadixTreeNode *root;
    uint32_t _size;
    
    
  public:
    RadixTree();
    ~RadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
};
