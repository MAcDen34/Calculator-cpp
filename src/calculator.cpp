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

double Calculator::calc_sin(double radians) {
    return std::sin(radians);
}
double Calculator::calc_cos(double radians) {
    return std::cos(radians);
}

double Calculator::calc_tan(double radians) {
    return std::tan(radians);
}

double Calculator::calc_sec(double radians) {
    double cos_val = std::cos(radians);
    if (std::abs(cos_val) < 1e-10) { // Handle near-zero cosine values
        throw CalcError("Secant is undefined for angles where cosine is zero.");
    }
    return 1 / cos_val;
}

double Calculator::calc_csc(double radians) {
    double sin_val = std::sin(radians);
    if (std::abs(sin_val) < 1e-10) { // Handle near-zero sine values
        throw CalcError("Cosecant is undefined for angles where sine is zero.");
    }
    return 1 / sin_val;
}

double Calculator::calc_cot(double radians) {
    double sin_val = std::sin(radians);
    if (std::abs(sin_val) < 1e-10) { // Handle near-zero sine values
        throw CalcError("Cotangent is undefined for angles where tangent is zero.");
    }
     return std::cos(radians) / sin_val;
}

double Calculator::calc_sin_deg(double degrees) {
    return calc_sin(degrees * M_PI / 180.0);
}

double Calculator::calc_cos_deg(double degrees) {
    return calc_cos(degrees * M_PI / 180.0);
}

double Calculator::calc_tan_deg(double degrees) {
    return calc_tan(degrees * M_PI / 180.0);
}

double Calculator::calc_sec_deg(double degrees) {
    return calc_sec(degrees * M_PI / 180.0);
}

double Calculator::calc_csc_deg(double degrees) {
    return calc_csc(degrees * M_PI / 180.0);
}

double Calculator::calc_cot_deg(double degrees) {
    return calc_cot(degrees * M_PI / 180.0);
}

double Calculator::calc_ln(double value) {
    if (value <= 0) {
        throw CalcError("Natural logarithm is undefined for non-positive values.");
    }
    return std::log(value);
}

double Calculator::calc_log10(double value) {
    if (value <= 0) {
        throw CalcError("Logarithm base 10 is undefined for non-positive values.");
    }
    return std::log10(value);
}

double Calculator::calc_exp(double exponent) {
    return std::exp(exponent);
}

double Calculator::calc_sqrt(double value) {
    if (value < 0) {
        throw CalcError("Square root is undefined for negative values.");
    }
    return std::sqrt(value);
}

double Calculator::calc_abs(double value) {
    return std::abs(value);
}

double Calculator::evaluate(double a, const std::string& op, double b) {
    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "*") return multiply(a, b);
    if (op == "/") return divide(a, b);
    if (op == "%") return modulo(a, b);
    if (op == "^") return power(a, b);
    if (op == "sin") return calc_sin(a);
    if (op == "cos") return calc_cos(a);
    if (op == "tan") return calc_tan(a);
    if (op == "sec") return calc_sec(a);
    if (op == "csc") return calc_csc(a);
    if (op == "cot") return calc_cot(a);
    if (op == "sin_deg") return calc_sin_deg(a);
    if (op == "cos_deg") return calc_cos_deg(a);
    if (op == "tan_deg") return calc_tan_deg(a);
    if (op == "sec_deg") return calc_sec_deg(a);
    if (op == "csc_deg") return calc_csc_deg(a);
    if (op == "cot_deg") return calc_cot_deg(a);
    if (op == "ln") return calc_ln(a);
    if (op == "log10") return calc_log10(a);
    if (op == "exp") return calc_exp(a);
    if (op == "sqrt") return calc_sqrt(a);
    if (op == "abs") return calc_abs(a);
    throw CalcError("Invalid operator: " + op);
}
