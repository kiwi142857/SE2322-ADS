#pragma once 

#include "select.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>

class QuickSelect : public Select{
public:
    QuickSelect() = default;
    ~QuickSelect() = default;

    int findKthSmallest(std::vector<int>& nums, int k) override {
        return quickSelect(nums, 0, nums.size() - 1, k);
    }

private:
    int quickSelect(std::vector<int>& nums, int left, int right, int k) {
        if (left == right) {
            return nums[left];
        }

        int pivotIndex = partition(nums, left, right);
        int rank = pivotIndex - left + 1;
        if (rank == k) {
            return nums[pivotIndex];
        } else if (rank < k) {
            return quickSelect(nums, pivotIndex + 1, right, k - rank);
        } else {
            return quickSelect(nums, left, pivotIndex - 1, k);
        }
    }

    int partition(std::vector<int>& nums, int left, int right) {
        int pivotIndex = left + std::rand() % (right - left + 1);
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
};