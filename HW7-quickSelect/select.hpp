#pragma once

#include <vector>
class Select
{
  public:
    virtual int findKthSmallest(std::vector<int> &nums, int k) = 0;

    virtual ~Select() = default;
};