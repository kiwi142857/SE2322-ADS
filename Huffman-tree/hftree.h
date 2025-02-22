#ifndef HFTREE_H
#define HFTREE_H

#include <algorithm>
#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>
class hfTree
{

    class Node
    {
      public:
        std::string key; // 字符或者字符串
        uint64_t count;  // 字符出现的次数
        Node *left;      // 左子树
        Node *right;     // 右子树
        char order;      // 字典序记录
        Node(std::string key, uint64_t count, char order)
            : key(key), count(count), order(order), left(nullptr), right(nullptr)
        {
        }
    };
    std::map<std::string, std::string> codingTable;

  private:
    // TODO: Your code here
    std::function<bool(Node *, Node *)> comp;
    std::priority_queue<Node *, std::vector<Node *>, decltype(comp)> pq;

  public:
    enum class Option {
        SingleChar,
        MultiChar
    };
    std::string executeMultiChar(const std::string &text);
    hfTree(const std::string &text, const Option op);
    std::map<std::string, std::string> getCodingTable();
    void printTree(Node *root);
    void getTable(Node *root, std::string code);
};

#endif