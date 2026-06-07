#pragma once
#include <future>
#include <string>
#include <functional>
#include "parser.h"

class CalcWorker {
public:
    using ResultCallback = std::function<void(double, const std::string&)>;

    CalcWorker(ResultCallback callback) : callback_(callback) {}

    void compute(const std::string& expression) {
        future_ = std::async(std::launch::async, [this, expression]() {
            try {
                double result = Parser(expression).parse();  // fill these
                callback_(result, "");
            } catch (const std::exception& e) {
                callback_(0.0, e.what());
            }
        });
    }

private:
    ResultCallback     callback_;
    std::future<void>  future_;
};
