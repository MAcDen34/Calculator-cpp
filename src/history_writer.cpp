#include "history_writer.h"
#include <fstream>
#include <iostream>

void HistoryWriter::run() {
    while (running_) {
        std::string entry = queue_.pop();
        if (entry == "__STOP__") break;

        std::ofstream file("history.log", std::ios::app);
        file << entry << "\n";
    }
}
