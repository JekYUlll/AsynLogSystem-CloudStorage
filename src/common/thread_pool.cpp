#include "common/thread_pool.h"

ThreadPool::ThreadPool(size_t threads) {
    if (threads == 0) threads = 1;
    for (size_t i = 0; i < threads; ++i) {
        workers_.emplace_back(&ThreadPool::Worker, this);
    }
}

ThreadPool::~ThreadPool() {
    stop_.store(true);
    cv_.notify_all();
    for (auto& w : workers_) {
        if (w.joinable()) w.join();
    }
}

void ThreadPool::Worker() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mu_);
            cv_.wait(lock, [&] { return stop_.load() || !tasks_.empty(); });
            if (stop_.load() && tasks_.empty()) return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}
