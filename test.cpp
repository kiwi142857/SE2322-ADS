#include "quickSelect.hpp"
#include "LinearSelect.hpp"
#include "select.hpp"
#include <chrono>

bool testSelect(int size = 10, int k = 3, bool isOrdered = false, bool isLinear = false)
{
    Select *qs;
    if (isLinear)
        qs = new LinearSelect();
    else
        qs = new QuickSelect();
    std::vector<int> nums;
    nums.resize(size);
    for (int i = 0; i < size; i++) {
        nums[i] = i;
    }
    // shuffle the array
    if (!isOrdered)
        std::random_shuffle(nums.begin(), nums.end());
    int result = qs->findKthSmallest(nums, k);
    delete qs;
    if (result == k - 1) {
        return true;
    } else
        std::cout << "Expected: " << k - 1 << " Got: " << result << std::endl;
    return false;
}

int main(int argc, char **argv)
{
    if (argc != 6) // argv are filename, size, k, isOrdered, testTime
    {
        std::cout << "Usage: ./test size k isOrdered testTime isLinear" << std::endl;
        return 1;
    }

    int size = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    bool isOrdered = std::stoi(argv[3]);
    int testTimes = std::stoi(argv[4]);
    bool isLinear = std::stoi(argv[5]);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < testTimes; i++) {
        if (!testSelect(size, k, isOrdered, isLinear)) {
            std::cout << "Test failed" << std::endl;
            return 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Test passed" << std::endl;
    // std::cout << "Elapsed time: " << elapsed.count() / 1000000000.0 << " seconds" << std::endl;
    std::cout << "Per element time: " << elapsed.count() / testTimes / 1000.0 << " ms" << std::endl;

    return 0;
}