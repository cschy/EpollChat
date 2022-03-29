#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    for (int i = 0; i < size; ++i) {
        threads.emplace_back(std::thread([this]() {
            threadBusy[std::this_thread::get_id()] = false;
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this]() {
                        return stop || !tasks.empty();
                        });
                    if (stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                }
                threadBusy[std::this_thread::get_id()] = true;
                task();
                threadBusy[std::this_thread::get_id()] = false;
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for (std::thread& th : threads) {
        if (th.joinable())
            th.join();
    }
    threadBusy.clear();
}