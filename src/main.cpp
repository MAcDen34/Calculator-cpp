#include<iostream>
#include<string>
#include<sstream>
#include "calculator.h"

int main() {
    Calculator calc;
    double num1, num2;
    std::string op;

    std::cout << "===== Simple Calculator =====\n";
    std::cout << "Enter operator (+, -, *, /, %, ^): ";
    std::cout << "Enter: <number1> <operator> <number2> (e.g., 5 + 3 ....)\n";
    std::cout << "Type 'exit' to quit.\n";

    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")  break;

        //Parse input
        std::istringstream iss(input);
        if (!(iss >> num1 >> op >> num2)) {
            std::cerr << "Invalid input format. Please try again.\n";
            continue;
}

        try {
            double result = calc.evaluate(num1, op, num2);
            std::cout << "Result: " << result << "\n";
        } catch (const CalcError& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye!\n";
    return 0;
}
