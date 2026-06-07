#pragma once
#include <thread>
#include <string>
#include <atomic>
#include "thread_safe_queue.h"

class HistoryWriter {
public:
    HistoryWriter() : running_(true) {
        writer_thread_ = std::thread(&HistoryWriter::run, this);
    }

    ~HistoryWriter() {
        running_ = false;
        queue_.push("__STOP__");
        if (writer_thread_.joinable()) {
            writer_thread_.join();
        }
    }

    void enqueue(const std::string& entry) {
        queue_.push(entry);
    }

private:
    void run();

    ThreadSafeQueue    queue_;
    std::thread        writer_thread_;
    std::atomic<bool>  running_;
};
