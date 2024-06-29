#include "radix_tree.hpp"
#include <cstdint>
#include <algorithm>
#include <iostream>
RadixTree::RadixTree()
{
    // To Be Implemented
    root = new RadixTreeNode();
    _size = 1;
}

RadixTree::~RadixTree()
{
    // To Be Implemented
    delete root;
}

void RadixTree::insert(int32_t value)
{
    // To Be Implemented
    // 1. find the leaf node to insert
    // 2. insert the value to the leaf node
    // 3. update the height of the tree
    // 4. update the size of the tree
    RadixTreeNode *node = root;
    // 2 bits per iteration
    for (int8_t i = 15; i >= 0; i--)
    {
        uint8_t bit = (value >> 2*i ) & 0x3;
        if (node->children[bit] == nullptr)
        {
            node->children[bit] = new RadixTreeNode(bit, node->height + 1, i == 0);
            _size++;
        }
        node = node->children[bit];
    }
    
}

void RadixTree::remove(int32_t value)
{
    // To Be Implemented
    // 1. find the leaf node to remove
    // 2. remove the value from the leaf node
    // 3. update the height of the tree
    // 4. update the size of the tree
    RadixTreeNode *node = root;
    // 2 bits per iteration
    RadixTreeNode *parent[16];
    for (int8_t i = 15; i >= 0; i--)
    {
        uint8_t bit = (value >> 2*i ) & 0x3;
        if (node->children[bit] == nullptr)
        {
            std::cout << "Value not found" << std::endl;
            return;
        }
        node = node->children[bit];
        parent[15-i] = node;
    }
    _size--;
    delete node;
    parent[14]->children[value & 0x3] = nullptr;

    // if any parent node has only one child, remove it
    for (int8_t i = 14; i >= 0; i--)
    {
        uint8_t bit = (value >> 2*i ) & 0x3;
        if(parent[i]->children[bit] == nullptr){
            for(uint8_t j = 0;j<4;++j){
                if(parent[i]->children[j] != nullptr){
                    break;
                }
                if(j == 3){
                    if(i == 0){
                        delete parent[i];
                        _size--;
                        root->children[value & 0xc0000000] = nullptr;
                        break;
                    }
                    parent[i-1]->children[parent[i]->bit] = nullptr;
                    delete parent[i];
                    _size--;
                }
            }
        }
        else{
            break;
        }
    }
}


bool RadixTree::find(int32_t value)
{
    // To Be Implemented
    // 1. find the leaf node to find
    // 2. check if the value is in the leaf node
    RadixTreeNode *node = root;
    // 2 bits per iteration
    for (int8_t i = 15; i >= 0; i--)
    {
        uint8_t bit = (value >> 2*i ) & 0x3;
        if (node->children[bit] == nullptr)
        {
            return false;
        }
        node = node->children[bit];
    }

    return true;
}

uint32_t RadixTree::size()
{
    // To Be Implemented
    return _size;
}

uint32_t RadixTree::height()
{
    // To Be Implemented
    
    return _size == 1 ? 1 : 17;
}
