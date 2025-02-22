#include "hftree.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

std::string hfTree::executeMultiChar(const std::string &text)
{
    std::map<std::string, uint64_t> char2Count;

    uint64_t length = text.length();

    bool isCounted = false;
    for (uint64_t i = 0; i < length - 1; ++i) {
        std::string key = text.substr(i, 2);
        // 如果key是两个相同字母，为了避免重复计数
        // TODO: 这里的逻辑有问题，比如"aaa"，会被计数两次
        if (key[0] == key[1]) {
            if (isCounted) {
                isCounted = false;
                continue;
            } else {
                if (char2Count.find(key) != char2Count.end()) {
                    // character exists in map, increment count
                    char2Count[key]++;
                } else {
                    // character does not exist in map, add it with count 1
                    char2Count[key] = 1;
                }
                isCounted = true;
                continue;
            }
        } else {
            isCounted = false;
        }
        if (char2Count.find(key) != char2Count.end()) {
            // character exists in map, increment count
            char2Count[key]++;
        } else {
            // character does not exist in map, add it with count 1
            char2Count[key] = 1;
        }
    }

    // 加入三个计数最多的2-char，如果相同计数的2-char有多个，则都加入
    std::vector<std::string> max3multiChar;
    if (char2Count.size() <= 3) {
        // 如果没有2-char,把所有的char加入
        for (auto it = char2Count.begin(); it != char2Count.end(); ++it) {
            max3multiChar.push_back(it->first);
        }
        std::string result = "";
        for (auto it = max3multiChar.begin(); it != max3multiChar.end(); ++it) {
            result += *it;
        }
        return result;
    }
    std::vector<std::pair<std::string, uint64_t>> countChar2Vec;

    for (auto it = char2Count.begin(); it != char2Count.end(); ++it) {
        countChar2Vec.push_back(*it);
    }

    sort(countChar2Vec.begin(), countChar2Vec.end(),
         [](const std::pair<std::string, uint64_t> &a, const std::pair<std::string, uint64_t> &b) {
             return a.second > b.second;
         });
    // select the 3 max char2
    auto it = countChar2Vec.begin();

    int countMax3 = 0, countMax2 = 0, countMax1 = 0;
    for (int i = 0; i < 3; ++i) {
        if (it != countChar2Vec.end()) {
            max3multiChar.push_back(it->first);
            if (i == 2) {
                countMax3 = it->second;
            }
            if (i == 1) {
                countMax2 = it->second;
            }
            if (i == 0) {
                countMax1 = it->second;
            }
            it++;
        }
    }
    while (it != countChar2Vec.end() && it->second == countMax3) {
        max3multiChar.push_back(it->first);
        it++;
    }

    std::string changeMultiChar[3];
    std::string result = "";
    if (max3multiChar.size() == 3) {
        result = max3multiChar[0] + max3multiChar[1] + max3multiChar[2];
        return result;
    } else if (countMax2 > countMax3) {
        result += max3multiChar[0] + max3multiChar[1];
        auto it = std::find(max3multiChar.begin(), max3multiChar.end(), max3multiChar[0]);
        if (it != max3multiChar.end()) {
            max3multiChar.erase(it);
        }
        it = std::find(max3multiChar.begin(), max3multiChar.end(), max3multiChar[0]);
        if (it != max3multiChar.end()) {
            max3multiChar.erase(it);
        }

    } else if (countMax1 > countMax2) {
        result += max3multiChar[0];
        auto it = std::find(max3multiChar.begin(), max3multiChar.end(), max3multiChar[0]);
        if (it != max3multiChar.end()) {
            max3multiChar.erase(it);
        }
    }

    std::sort(max3multiChar.begin(), max3multiChar.end());
    for (auto it = max3multiChar.begin(); result.size() < 6; ++it) {
        result += *it;
    }
    return result;
}

hfTree::hfTree(const std::string &text, const Option op)
{
    // TODO: Your code here

    // use map to get the number every character occur
    std::map<std::string, uint64_t> charCount;
    std::map<std::string, uint64_t> char2Count;

    uint64_t length = text.length();

    std::string multiChar[3];
    int numMultiChar = 0;
    // if the option is MultiChar, count the number of the 2-char occurs
    if (op == Option::MultiChar) {
        std::string temp = executeMultiChar(text);
        if (temp.size() == 6) {
            multiChar[0] = temp.substr(0, 2);
            multiChar[1] = temp.substr(2, 2);
            multiChar[2] = temp.substr(4, 2);
            numMultiChar = 3;
        } else if (temp.size() == 4) {
            multiChar[0] = temp.substr(0, 2);
            multiChar[1] = temp.substr(2, 2);
            numMultiChar = 2;
        } else if (temp.size() == 2) {
            multiChar[0] = temp.substr(0, 2);
            numMultiChar = 1;
        }
    }

    // count the number of the 1-char / 2-char occurs
    bool isMet = false;
    for (uint64_t i = 0; i < length; ++i) {
        if (op == Option::MultiChar) {
            std::string key2 = text.substr(i, 2);
            for (int j = 0; j < numMultiChar; ++j) {
                if (key2 == multiChar[j]) {
                    i++;
                    isMet = true;
                    if (charCount.find(key2) != charCount.end()) {
                        charCount[key2]++;
                    } else {
                        charCount[key2] = 1;
                    }
                    continue;
                }
            }
        }
        if (isMet) {
            isMet = false;
            continue;
        }
        std::string key = text.substr(i, 1);
        if (charCount.find(key) != charCount.end()) {
            // character exists in map, increment count
            charCount[key]++;
        } else {
            // character does not exist in map, add it with count 1
            charCount[key] = 1;
        }
    }

    // create node for every character and build a priority queue
    // Add this line to include the <functional> header
    comp = [](Node *a, Node *b) {
        if (a->count == b->count) {
            return a->order > b->order;
        }
        return a->count > b->count;
    };
    pq = std::priority_queue<Node *, std::vector<Node *>, decltype(comp)>(comp);

    for (auto it = charCount.begin(); it != charCount.end(); ++it) {
        pq.push(new Node(it->first, it->second, it->first[0]));
    }
    // build the huffman tree
    while (pq.size() > 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        int order = left->order < right->order ? left->order : right->order;
        Node *parent = new Node(left->key + right->key, left->count + right->count, order);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    // 打印huffman tree
    // getCodingTable();
    // printTree(root);
}

void hfTree::printTree(Node *root)
{
    if (root == nullptr) {
        return;
    }
    std::cout << root->key << " " << root->count << std::endl;
    printTree(root->left);
    printTree(root->right);
}

std::map<std::string, std::string> hfTree::getCodingTable()
{
    // TODO: Your code here

    // use recursive to get the coding table
    std::string code = "";

    getTable(pq.top(), code);
    return codingTable;
}

void hfTree::getTable(Node *root, std::string code)
{
    if (root == nullptr) {
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        codingTable[root->key] = code;
    }
    getTable(root->left, code + "0");
    getTable(root->right, code + "1");
}