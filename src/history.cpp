#include "history.h"
#include <iostream>
#include <fstream>
#include <iomanip>


void History::add(const std::string& expr, double result) {
    entries_.push_back({expr, result});
}

void History::print() const {
    if (entries_.empty()) {
        std::cout << "No history available.\n";
        return;
    }
    std::cout << "History:\n";
    for (size_t i = 0; i < entries_.size(); ++i) {
        std::cout << i + 1 << ". " 
        << entries_[i].expression 
        << " = " 
        << entries_[i].result << "\n";
    }
    std::cout <<"======================\n";
}

void History::clear() {
    entries_.clear();
    std::cout << "History cleared.\n";
}

void History::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }
    for (const auto& entry : entries_) {
        outFile << entry.expression << "=" << entry.result << "\n";
    }
    std::cout << "History saved to " << filename << "\n";
}

void History::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return;
    }
    entries_.clear();
    std::string line;
    while (std::getline(inFile, line)) {
        size_t eqPos = line.find('=');
        if (eqPos != std::string::npos) {
            std::string expr = line.substr(0, eqPos);
            double result = std::stod(line.substr(eqPos + 1));
            entries_.push_back({expr, result});
        }
    }
    std::cout << "History loaded from " << filename << "\n";
}

const std::vector<HistoryEntry>& History::getEntries() const {
    return entries_;
}
