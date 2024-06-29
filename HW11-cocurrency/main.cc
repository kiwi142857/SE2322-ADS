#include "waiting_once_api.hpp"
#include <cassert>
#include <chrono>
#include <thread>
#include <vector>

#define my_improved_version
// #define std_call_once
// #define naive_version

#ifdef my_improved_version

std::mutex m;
bool is_init = false;
std::unique_lock<std::mutex> lock(m, std::defer_lock);
void test_call_once()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    waiting_once wo;
    std::jthread threads[8];
    is_init = false;
    for (auto &t : threads)
        t = std::jthread([&]() {
            if (!is_init) {
                lock.lock();
                if (is_init) {
                    assert(cnt_accu == 1);
                    assert(cnt_now == 0);
                    lock.unlock();
                    return;
                }
                wo.call_once_waiting([&]() {
                    ++cnt_now;
                    ++cnt_accu;
                    assert(cnt_now == 1);
                    --cnt_now;
                });
                is_init = true;
                lock.unlock();
                return;
            }
            if (is_init) {
                assert(cnt_accu == 1);
                assert(cnt_now == 0);
            }
        });
}

void test_long_init()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    waiting_once wo;
    std::jthread threads[8];
    is_init = false;
    for (auto &t : threads)
        t = std::jthread([&]() {
            if (!is_init) {
                lock.lock();
                if (is_init) {
                    assert(cnt_accu == 1);
                    assert(cnt_now == 0);
                    lock.unlock();
                    return;
                }
                wo.call_once_waiting([&]() {
                    ++cnt_now;
                    ++cnt_accu;
                    assert(cnt_now == 1);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    --cnt_now;
                });
                is_init = true;
                lock.unlock();
                return;
            }
            if (is_init) {
                assert(cnt_accu == 1);
                assert(cnt_now == 0);
            }
        });
}
#endif

#ifdef std_call_once
void test_call_once()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    std::once_flag flag;
    std::jthread threads[8];
    for (auto &t : threads)
        t = std::jthread([&]() {
            std::call_once(flag, [&]() {
                ++cnt_now;
                ++cnt_accu;
                assert(cnt_now == 1);
                --cnt_now;
            });
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
        });
}

void test_long_init()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    std::once_flag flag;
    std::jthread threads[8];
    for (auto &t : threads)
        t = std::jthread([&]() {
            std::call_once(flag, [&]() {
                ++cnt_now;
                ++cnt_accu;
                assert(cnt_now == 1);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                --cnt_now;
            });
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
        });
}

#endif

#ifdef naive_version
std::mutex m;
bool is_init = false;
std::unique_lock<std::mutex> lock(m, std::defer_lock);
void test_call_once()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    waiting_once wo;
    std::jthread threads[8];
    is_init = false;
    for (auto &t : threads)
        t = std::jthread([&]() {
            lock.lock();
            if (!is_init) {
                wo.call_once_waiting([&]() {
                    ++cnt_now;
                    ++cnt_accu;
                    assert(cnt_now == 1);
                    --cnt_now;
                });
                is_init = true;
                lock.unlock();
                return;
            }
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
            lock.unlock();
        });
}

void test_long_init()
{
    std::atomic<int> cnt_accu = 0, cnt_now = 0;
    waiting_once wo;
    std::jthread threads[8];
    is_init = false;
    for (auto &t : threads)
        t = std::jthread([&]() {
            lock.lock();
            if (!is_init) {
                wo.call_once_waiting([&]() {
                    ++cnt_now;
                    ++cnt_accu;
                    assert(cnt_now == 1);
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    --cnt_now;
                });
                is_init = true;
                lock.unlock();
                return;
            }
            lock.unlock();
            assert(cnt_accu == 1);
            assert(cnt_now == 0);
        });
}
#endif
#if __cpp_lib_print >= 202207L
#include <print>
void report_throughput(double seconds, long ops_per_second)
{
    std::print("time: {}s\nthroughput: {} ops/s", seconds, ops_per_second);
}
#else
#include <iostream>
void report_throughput(double seconds, long ops_per_second)
{
    std::cout << "time: " << seconds
              << "s\n"
                 "throughput: "
              << ops_per_second << " ops/s\n";
}
#endif

void test_throughput(int times_per_thread, size_t threads_num)
{
    waiting_once wo;
    wo.call_once_waiting([]() {}); // init done
    auto t1 = std::chrono::system_clock::now();
    {
        std::vector<std::jthread> threads(threads_num);
        for (auto &t : threads)
            t = std::jthread(
                [&](int times) {
                    for (int i = 0; i < times; ++i)
                        wo.call_once_waiting([]() {});
                },
                times_per_thread);
    }
    auto t2 = std::chrono::system_clock::now();
    auto duration = t2 - t1;
    auto duration_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    report_throughput(duration_seconds, times_per_thread * threads_num / duration_seconds);
}

int main()
{
    // test_call_once();
    // test_long_init();

    std::cout << "Throughput test\n";
    std::cout << "threads_num: 1\n";
    test_throughput(10000000, 1);
    std::cout << "threads_num: 2\n";
    test_throughput(10000000, 2);
    std::cout << "threads_num: 4\n";
    test_throughput(10000000, 4);
    std::cout << "threads_num: 8\n";
    test_throughput(10000000, 8);
}
