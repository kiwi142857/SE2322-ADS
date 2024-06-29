#include "compressed_radix_tree.hpp"
#include <algorithm>
#include <cstdint>
CompressedRadixTree::CompressedRadixTree()
{
    // To Be Implemented
    root = new CompressedRadixTreeNode();
    _size = 1;
    _height = 1;
}

CompressedRadixTree::~CompressedRadixTree()
{
    // To Be Implemented
    delete root;
}

CompressedRadixTreeNode *CompressedRadixTree::findNode(int32_t value)
{
    CompressedRadixTreeNode *current = root;
    while (!(current->endBit == 31)) {
        uint8_t bit = (value >> (29 - current->endBit)) & 0x3;
        if (current->children[bit] == nullptr) {
            return current;
        }
        if (current->children[bit]->isEqual(value)) {
            current = current->children[bit];
        } else {
            return current;
        }
    }
    return current;
}

bool CompressedRadixTree::isAllChildrenNull(CompressedRadixTreeNode *node)
{
    for (int i = 0; i < 4; i++) {
        if (node->children[i] != nullptr) {
            return false;
        }
    }
    return true;
}

void CompressedRadixTree::insert(int32_t value)
{
    // To Be Implemented
    CompressedRadixTreeNode *current = findNode(value);
    if (current->isLeaf) {
        return;
    }
    uint8_t bit = (value >> (29 - current->endBit)) & 0x3;
    if (current->children[bit] == nullptr) {
        uint8_t startBit_ = current->endBit == 0 ? 0 : current->endBit + 1;
        current->children[bit] = new CompressedRadixTreeNode(value, startBit_, 31, current->height + 1, true, current);
        _size++;
        _height = std::max(_height, current->children[bit]->height);
    } else {
        uint8_t diff = current->children[bit]->firstDiffBits(value);
        uint32_t mask = 0xFFFFFFFF;
        mask >>= (31 - diff);
        mask <<= (31 - diff + current->children[bit]->startBit);
        mask >>= current->children[bit]->startBit;
        uint8_t newBit = (value >> (30 - diff)) & 0x3;
        uint32_t mask2 = 0xFFFFFFFF;
        mask2 >>= (31 - current->children[bit]->endBit);
        mask2 <<= (31 - current->children[bit]->endBit + diff);
        mask2 >>= diff;
        uint8_t newBit2 = (current->children[bit]->bits >> (30 - diff)) & 0x3;

        CompressedRadixTreeNode *newNode = new CompressedRadixTreeNode(
            value, diff, 31, current->children[bit]->height + 1, true, current->children[bit]);
        CompressedRadixTreeNode *newNode2 =
            new CompressedRadixTreeNode(current->children[bit]->bits, diff, current->children[bit]->endBit,
                                        current->children[bit]->height + 1, false, current->children[bit]);
        _size += 2;
        for (int i = 0; i < 4; i++) {
            newNode2->children[i] = current->children[bit]->children[i];
            if (current->children[bit]->children[i] != nullptr)
                current->children[bit]->children[i]->parent = newNode2;
            current->children[bit]->children[i] = nullptr;
        }
        current->children[bit]->children[newBit] = newNode;
        current->children[bit]->children[newBit2] = newNode2;
        current->children[bit]->setEndBit(diff - 1);
        current->children[bit]->setIsLeaf(false);
    }
}

void CompressedRadixTree::remove(int32_t value)
{
    CompressedRadixTreeNode *current = findNode(value);
    if (!(current->endBit == 31)) {
        return;
    }
    if (current->parent != nullptr) {
        CompressedRadixTreeNode *parent = current->parent;
        uint8_t bit = (current->bits >> (29 - parent->endBit)) & 0x3;
        if (countChildren(parent) == 1) {
            // If parent has no other children, delete current and parent
            parent->children[bit] = nullptr;
            delete current;
            _size--;
            current = parent;
        } else if (countChildren(parent) == 2) {
            // If parent has only one child, merge parent and child
            parent->children[bit] = nullptr;
            delete current;
            _size--;
            CompressedRadixTreeNode *sibling = getOnlyChild(parent);
            uint8_t bitSibling = (sibling->bits >> (29 - parent->endBit)) & 0x3;
            uint32_t maskParent = 0xFFFFFFFF;
            maskParent >>= (31 - parent->endBit);
            maskParent <<= (31 - parent->endBit + parent->startBit);
            maskParent >>= parent->startBit;
            uint32_t maskSibling = 0xFFFFFFFF;
            maskSibling >>= (31 - sibling->endBit);
            maskSibling <<= (31 - sibling->endBit + sibling->startBit);
            maskSibling >>= sibling->startBit;
            parent->bits = (parent->bits & maskParent) | (sibling->bits & maskSibling);
            parent->endBit = sibling->endBit;
            parent->children[bitSibling] = nullptr;
            delete sibling;
            _size--;
            current = parent;
        } else {
            // If parent has more than one child, just delete current
            delete current;
            _size--;
            parent->children[bit] = nullptr;
            current = parent;
        }
    }
    // 递归处理节点只有一个子节点的情况
    while (current->parent != nullptr && countChildren(current) <= 1) {
        CompressedRadixTreeNode *parent = current->parent;
        uint8_t bit = (current->bits >> (30 - parent->endBit)) & 0x3;
        // 不用删除结点，只需合并
        if (countChildren(parent) == 1) {
            parent->children[bit] = nullptr;
            uint32_t maskParent = 0xFFFFFFFF;
            maskParent >>= (31 - parent->endBit);
            maskParent <<= (31 - parent->endBit + parent->startBit);
            maskParent >>= parent->startBit;
            uint32_t maskCurrent = 0xFFFFFFFF;
            maskCurrent >>= (31 - current->endBit);
            maskCurrent <<= (31 - current->endBit + current->startBit);
            maskCurrent >>= current->startBit;
            parent->bits = (parent->bits & maskParent) | (current->bits & maskCurrent);
            parent->endBit = current->endBit;

            delete current;
            _size--;
            current = parent;
        } else {
            break;
        }
    }
}

bool CompressedRadixTree::find(int32_t value)
{
    // To Be Implemented
    CompressedRadixTreeNode *node = findNode(value);
    if (node->endBit == 31) {
        return true;
    }
    return false;
}

uint32_t CompressedRadixTree::size()
{
    // To Be Implemented

    return calculateSize(root);
}

uint32_t CompressedRadixTree::height()
{
    // To Be Implemented
    return calculateHeight(root);
}

int countOnes(int32_t mask)
{
    int count = 0;
    while (mask) {
        mask &= (mask - 1);
        count++;
    }
    return count;
}

int findLastOne(int32_t mask)
{
    int position = -1;
    while (mask) {
        mask >>= 1;
        position++;
    }
    return position;
}

uint8_t CompressedRadixTree::calculateHeight(CompressedRadixTreeNode *node)
{

    if (node == nullptr) {
        return 0;
    }
    uint8_t height = 0;
    for (int i = 0; i < 4; i++) {
        height = std::max(height, calculateHeight(node->children[i]));
    }
    return height + 1;
}

uint8_t CompressedRadixTree::calculateSize(CompressedRadixTreeNode *node)
{

    if (node == nullptr) {
        return 0;
    }
    uint8_t size = 0;
    for (int i = 0; i < 4; i++) {
        if (isAllChildrenNull(node) && node->endBit != 31 && node->parent != nullptr) {
            
            node->parent->children[(node->bits >> (29 - node->parent->endBit)) & 0x3] = nullptr;
            delete node;
            return 0;
        }
        if(countChildren(node) == 1 && node->parent != nullptr) {
            int index = 0;
            CompressedRadixTreeNode *child = getOnlyChild(node, index);
            node->endBit = child->endBit;
            node->bits = child->bits;
            node->children[index] = nullptr;
            for(int i = 0; i < 4; i++) {
                node->children[i] = child->children[i];
                if(child->children[i] != nullptr) {
                    child->children[i]->parent = node;
                }
            }
            delete child;
            
            for (int i = 0; i < 4; i++) {
                size += calculateSize(node->children[i]);
            }
            return size + 1;
        }
        size += calculateSize(node->children[i]);
    }
    return size + 1;
}

CompressedRadixTreeNode *CompressedRadixTree::checkNode(CompressedRadixTreeNode *node)
{
    if (countChildren(node) <= 1) {
        return node;
    }
    for (int i = 0; i < 4; i++) {
        CompressedRadixTreeNode *result = checkNode(node->children[i]);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}
