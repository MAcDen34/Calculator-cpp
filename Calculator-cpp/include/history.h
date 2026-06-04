#pragma once
#include <vector>
#include <string>

struct HistoryEntry {
    std::string expression;
    double result;
};

class History {
    public:
    void add(const std::string& expr, double result);
    void print() const;
    void clear();
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    const std::vector<HistoryEntry>& getEntries() const;

    private:
    std::vector<HistoryEntry> entries_;
};
