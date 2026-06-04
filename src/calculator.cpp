#include "calculator.h"
#include <cmath>
#include <stdexcept>

double Calculator::add(double a, double b) {
    return a + b;
}

double Calculator::subtract(double a, double b) {
    return a - b;
}

double Calculator::multiply(double a, double b) {
    return a * b;
}

double Calculator::divide(double a, double b) {
    if (b == 0) {
        throw CalcError("Division by zero is not allowed.");
    }
    return a / b;
}

double Calculator::modulo(double a, double b) {
    if (b == 0) {
        throw CalcError("Modulo by zero is not allowed.");
    }
    return std::fmod(a, b);
}

double Calculator::power(double base, double exp) {
    return std::pow(base, exp);
}

double Calculator::evaluate(double a, const std::string& op, double b) {
    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "*") return multiply(a, b);
    if (op == "/") return divide(a, b);
    if (op == "%") return modulo(a, b);
    if (op == "^") return power(a, b);
    throw CalcError("Invalid operator: " + op);
}
