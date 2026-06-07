#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class ThreadSafeQueue {
public:
    void push(const std::string& value) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(value);
        cond_var.notify_one();
    }

    std::string pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cond_var.wait(lock, [this] { return !q.empty(); });
        std::string value = std::move(q.front());
        q.pop();
        return value;
    }

private:
    std::queue<std::string>  q;
    std::mutex               mtx;
    std::condition_variable  cond_var;
};
