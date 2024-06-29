#pragma once
#include "base.hpp"
#include <cstring>
#include <iostream>
#include <vector>

#include <cassert>
#include <queue>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "../util/util.hpp"

namespace HNSWLab
{

class HNSW : public AlgorithmInterface
{

    class Node
    {
      public:
        int *vec;
        uint maxLevel;
        int label;
        // neighbors[level] is the set of neighbors at level
        std::vector<std::unordered_set<Node *>> neighbors;

        Node(int *vec, uint maxLevel, int label) : vec(vec), maxLevel(maxLevel), label(label)
        {
            neighbors.resize(maxLevel+1);
        }
    };

  public:
    // you can add more parameter to initialize HNSW

    uint dimension;
    Node *enterPoint;
    HNSW()
    {
        enterPoint = nullptr;
    }

    HNSW(uint dimension) : dimension(dimension), enterPoint(nullptr)
    {
    }

    void setDimension(uint dimension)
    {
        this->dimension = dimension;
    }
    void insert(const int *item, int label);

    std::vector<int> query(const int *query, int k);

    std::vector<Node *> searchLayer(const int *query, Node *ep, int ef, int level);

    std::unordered_set<Node *> selectNeighbors(const int *query, std::vector<Node *> &nodes, int ef, int lc);

    std::unordered_set<Node *> selectNeighbors(const int *query, std::unordered_set<Node *> &nodes, int ef, int lc);
    ~HNSW()
    {
    }
};

/**
 * input:
 * iterm: the vector to be inserted
 * label: the label(id) of the vector
 */
void HNSW::insert(const int *item, int label)
{
    // TODO
    if (enterPoint == nullptr) {
        enterPoint = new Node(const_cast<int *>(item), 0, label);
        enterPoint->maxLevel = get_random_level();
        for(int i = 0; i < enterPoint->maxLevel; i++){
            enterPoint->neighbors.push_back(std::unordered_set<Node *>());
        }
        return;
    }

    int maxL = enterPoint->maxLevel;
    std::vector<Node *> nearestNodes;
    int level = get_random_level();
    Node *curNode = new Node(const_cast<int *>(item), level, label);
    Node *curEnterPoint = enterPoint;

    for (int lc = maxL; lc > level; lc--) {
        nearestNodes = searchLayer(item, curEnterPoint, 1, lc);
        curEnterPoint = nearestNodes[0];
    }

    for (int lc = level < maxL ? level : maxL; lc >= 0; lc--) {
        nearestNodes = searchLayer(item, curEnterPoint, ef_construction, lc);
        std::unordered_set<Node *> selectedNeighbors = selectNeighbors(item, nearestNodes, M, lc);
        curNode->neighbors[lc] = selectedNeighbors;
        for (auto &neighbor : selectedNeighbors) {
            neighbor->neighbors[lc].insert(curNode);
            if (neighbor->neighbors[lc].size() > M_max) {
                neighbor->neighbors[lc] = selectNeighbors(neighbor->vec, neighbor->neighbors[lc], M_max, lc);
            }
        }
        curEnterPoint = nearestNodes[nearestNodes.size() - 1];
    }

    if (level > maxL) {
        enterPoint = curNode;
    }
}

/**
 * input:
 * query: the vector to be queried
 * k: the number of nearest neighbors to be returned
 *
 * output:
 * a vector of labels of the k nearest neighbors
 */
std::vector<int> HNSW::query(const int *query, int k)
{
    std::vector<int> res;
    if (enterPoint == nullptr) {
        return res;
    }
    Node *curNode = enterPoint;
    for (int lc = enterPoint->maxLevel; lc > 0; lc--) {
        std::vector<Node *> nearestNodes = searchLayer(query, curNode, 1, lc);
        curNode = nearestNodes[0];
    }
    std::vector<Node *> nearestNodes = searchLayer(query, curNode, ef_construction, 0);
    std::unordered_set<Node *> selectedNeighbors = selectNeighbors(query, nearestNodes, k, 0);
    for (auto &neighbor : selectedNeighbors) {
        res.push_back(neighbor->label);
    }
    return res;
}

std::vector<HNSW::Node *> HNSW::searchLayer(const int *query, Node *ep, int ef, int level)
{

    std::unordered_set<Node *> visited;
    std::priority_queue<std::pair<double, Node *>>
        candidateSet; // max heap, the first element is the negative value of the distance
    std::priority_queue<std::pair<double, Node *>>
        nearestSet; // max heap, the first element is the value of the distance

    double epDist = l2distance(query, ep->vec, dimension);
    candidateSet.push(std::make_pair(-epDist, ep));
    nearestSet.push(std::make_pair(epDist, ep));
    visited.insert(ep);
    while (!candidateSet.empty()) {
        auto top = candidateSet.top();
        candidateSet.pop();
        Node *curNode = top.second;
        

        double furthestDist;
        if (nearestSet.empty()) {
            furthestDist = std::numeric_limits<double>::max();
        } else {
            furthestDist = nearestSet.top().first;
        }
        if (-top.first > furthestDist) {
            break;
        } else {
            
            for (auto &neighbor : curNode->neighbors[level]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    double furthestDist = nearestSet.top().first;
                    double neighborDist = l2distance(query, neighbor->vec, dimension);
                    if (nearestSet.size() < ef) {
                        
                        candidateSet.push(std::make_pair(-neighborDist, neighbor));
                        nearestSet.push(std::make_pair(neighborDist, neighbor));
                    } else
                    if (neighborDist < furthestDist) {
                        candidateSet.push(std::make_pair(-neighborDist, neighbor));
                        nearestSet.push(std::make_pair(neighborDist, neighbor));
                        if (nearestSet.size() > ef) {
                            nearestSet.pop();
                        }
                    }
                }
            }
        }
    }
    std::vector<Node *> res;
    while (!nearestSet.empty()) {
        res.push_back(nearestSet.top().second);
        nearestSet.pop();
    }
    return res;
}

/*
 * input:
 * query: the query vector
 * nodes: the set of nodes to be selected
 * lc: the number of levels to be selected
 *
 * output:
 * a set of nodes selected as the nearest
 */
std::unordered_set<HNSW::Node *> HNSW::selectNeighbors(const int *query, std::vector<Node *> &nodes, int ef, int lc)
{
    std::unordered_set<Node *> res;
    std::priority_queue<std::pair<double, Node *>> nearestNodes;
    for (auto &node : nodes) {
        double dist = -l2distance(query, node->vec, dimension);
        nearestNodes.push(std::make_pair(dist, node));
    }
    while (!nearestNodes.empty()) {
        auto top = nearestNodes.top();
        nearestNodes.pop();
        res.insert(top.second);
        if (res.size() >= ef) {
            break;
        }
    }
    return res;
}

std::unordered_set<HNSW::Node *> HNSW::selectNeighbors(const int *query, std::unordered_set<Node *> &nodes, int ef,
                                                       int lc)
{
    std::unordered_set<Node *> res;
    std::priority_queue<std::pair<double, Node *>> nearestNodes;
    for (auto &node : nodes) {
        double dist = -l2distance(query, node->vec, dimension);
        nearestNodes.push(std::make_pair(dist, node));
    }
    while (!nearestNodes.empty()) {
        auto top = nearestNodes.top();
        nearestNodes.pop();
        res.insert(top.second);
        if (res.size() >= ef) {
            break;
        }
    }
    return res;
}
} // namespace HNSWLab