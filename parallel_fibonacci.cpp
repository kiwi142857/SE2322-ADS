#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <fstream>

std::mutex mtx; // 全局互斥锁
int n = 0;      // 全局变量
int max;
const int N = 40;

int fibonacci(int x)
{
    if (x <= 1)
        return x;
    else {
        mtx.lock();
        if (n < max) {
            n++;
            mtx.unlock();
            std::future<int> fut1 = std::async(std::launch::async, fibonacci, x - 1);
            std::future<int> fut2 = std::async(std::launch::async, fibonacci, x - 2);
            int result = fut1.get() + fut2.get();
            mtx.lock();
            n--;
            mtx.unlock();
            return result;
        } else {
            mtx.unlock();
            return fibonacci(x - 1) + fibonacci(x - 2);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }
    int max = std::stoi(argv[1]);

    auto start = std::chrono::high_resolution_clock::now();  // Start timing

    int result = fibonacci(N);

    auto end = std::chrono::high_resolution_clock::now();  // End timing
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::ofstream outfile("record.txt", std::ios::app);
    outfile << "N = " << max << std::endl;
    outfile << "Fibonacci(" << max << ") = " << result << std::endl;
    outfile << "Execution time: " << elapsed.count() << " milliseconds" << std::endl;
    outfile << std::endl;
    outfile.close();

    return 0;
}