#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

private:
    void Worker();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mu_;
    std::condition_variable cv_;
    std::atomic<bool> stop_{false};
};

template <class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
    using Ret = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<Ret()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    {
        std::lock_guard<std::mutex> lock(mu_);
        tasks_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return task->get_future();
}
