// implement a radix tree that support node compressino and store int32_t values
// each parent node has 4 children, representing 2 bits
#include "tree.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>
class CompressedRadixTreeNode
{
  public:
    uint32_t bits;
    uint8_t startBit, endBit;
    uint8_t height;
    bool isLeaf;
    CompressedRadixTreeNode *parent;
    CompressedRadixTreeNode *children[4];

    CompressedRadixTreeNode()
    {
        bits = 0;
        startBit = 0;
        endBit = 0;
        height = 1;
        parent = nullptr;
        isLeaf = false;
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }

    CompressedRadixTreeNode(uint32_t bits, uint8_t startBit, uint8_t endBit, uint8_t height, bool isLeaf,
                            CompressedRadixTreeNode *parent)
    {
        this->bits = bits;
        this->startBit = startBit;
        this->endBit = endBit;
        this->height = height;
        this->isLeaf = isLeaf;
        this->parent = parent;
        for (int i = 0; i < 4; i++) {
            children[i] = nullptr;
        }
    }

    void setBits(uint32_t bits, uint8_t startBit, uint8_t endBit)
    {
        this->bits = bits;
        this->startBit = startBit;
        this->endBit = endBit;
    }
    void setHeight(uint8_t height)
    {
        this->height = height;
    }
    void setIsLeaf(bool isLeaf)
    {
        this->isLeaf = isLeaf;
    }
    bool isAllChildrenNull()
    {
        for (int i = 0; i < 4; i++) {
            if (children[i] != nullptr) {
                return false;
            }
        }
        return true;
    }
    uint8_t firstDiffBits(uint32_t value)
    {
        uint32_t mask = 0xFFFFFFFF;        // Start with all bits set to 1
        mask >>= (31 - endBit);            // Shift right to clear bits from 31 to endBit
        mask <<= (31 - endBit + startBit); // Shift left to clear bits from startBit to 0
        mask >>= startBit;                 // Shift right to get the final mask

        uint32_t diff = (value & mask) ^ (bits & mask); // XOR to find different bits

        // Find the position of the first different bit from the highest bit
        uint8_t position = 31;
        while (position > 0) {
            if ((diff & (1 << position)) != 0) {
                break;
            }
            position--;
        }

        return (31 - position) / 2 * 2;
    }
    bool isEqual(uint32_t value) const
    {
        uint32_t mask = 0xFFFFFFFF;        // Start with all bits set to 1
        mask >>= (31 - endBit);            // Shift right to clear bits from 31 to endBit
        mask <<= (31 - endBit + startBit); // Shift left to clear bits from startBit to 0
        mask >>= startBit;                 // Shift right to get the final mask
        return (value & mask) == (bits & mask);
    };
    void setStartBit(uint8_t startBit)
    {
        this->startBit = startBit;
    }
    void setEndBit(uint8_t endBit)
    {
        this->endBit = endBit;
    }
};

class CompressedRadixTree : public Tree
{
    // To Be Implemented
  private:
    CompressedRadixTreeNode *root;
    uint8_t _size;
    uint8_t _height;
    bool isAllChildrenNull(CompressedRadixTreeNode *node);
    CompressedRadixTreeNode *findNode(int32_t value);
    uint8_t countChildren(CompressedRadixTreeNode *node)
    {
        uint8_t count = 0;
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
                count++;
            }
        }
        return count;
    }
    CompressedRadixTreeNode *getOnlyChild(CompressedRadixTreeNode *node)
    {
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
                return node->children[i];
            }
        }
        return nullptr;
    }
    CompressedRadixTreeNode *getOnlyChild(CompressedRadixTreeNode *node, int &index)
    {
        for (int i = 0; i < 4; i++) {
            if (node->children[i] != nullptr) {
                index = i;
                return node->children[i];
            }
        }
        return nullptr;
    }

  public:
    CompressedRadixTree();
    ~CompressedRadixTree();
    // basic operation
    void insert(int32_t value);
    void remove(int32_t value);
    bool find(int32_t value);
    // statistics
    uint32_t size();
    uint32_t height();
    uint8_t calculateHeight(CompressedRadixTreeNode *node);
    uint8_t calculateSize(CompressedRadixTreeNode *node);
    // 检查是否存在只有一个子节点的非叶子节点
    CompressedRadixTreeNode *checkNode(CompressedRadixTreeNode *node);
};