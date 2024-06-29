#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "select.hpp"

class LinearSelect : public Select
{
public:
    LinearSelect() = default;
    ~LinearSelect() = default;

    const int dividedSize = 5;
public:
    int findKthSmallest(std::vector<int> &nums, int k) override
    {
        return findKthSmallest(nums, 0, nums.size() - 1, k);
    }

private:
    int findKthSmallest(std::vector<int> &nums, int left, int right, int k)
    {
        if (left == right)
            return nums[left];

        int pivotIndex = partition(nums, left, right);
        if(pivotIndex == -1)
            return -1;
        int pivotRank = pivotIndex - left + 1;

        if (pivotRank == k)
            return nums[pivotIndex];
        else if (k < pivotRank)
            return findKthSmallest(nums, left, pivotIndex - 1, k);
        else
            return findKthSmallest(nums, pivotIndex + 1, right, k - pivotRank);
    }

    int partition(std::vector<int> &nums, int left, int right)
    {
        int pivotIndex = findPivot(nums, left, right);
        
        std::swap(nums[pivotIndex], nums[right]);

        int i = left;
        for (int j = left; j < right; ++j) {
            if (nums[j] < nums[right]) {
                std::swap(nums[i++], nums[j]);
            }
        }
        std::swap(nums[i], nums[right]);
        return i;
    }

    int findPivot(std::vector<int> &nums, int left, int right)
    {   
        if (right - left + 1 <= dividedSize)
        {
            return findMedian(nums, left, right);
        }
        
        std::vector<int> medians;
        for (int i = left; i <= right; i += dividedSize)
        {
            int groupRight = std::min(i + dividedSize - 1, right);
            medians.push_back(medianOf5(nums, i, groupRight));
        }

        return findKthSmallest(medians, 0, medians.size() - 1, medians.size() / 2);
    }
    
    int medianOf5(std::vector<int> &nums, int left, int right)
    {
        int size = right - left + 1;
        if (size <= 5)
        {
            return findMedian(nums, left, right);
        }

        for (int i = left; i <= right; i += 5)
        {
            int groupRight = std::min(i + 4, right);
            int medianIndex = findMedian(nums, i, groupRight);
            std::swap(nums[medianIndex], nums[left + (i - left) / 5]);
        }

        return findKthSmallest(nums, left, left + size / 5, size / 10);
    }

    int findMedian(std::vector<int> &nums, int left, int right)
    {
        std::sort(nums.begin() + left, nums.begin() + right + 1);
        return left + (right - left) / 2;
    }
};
