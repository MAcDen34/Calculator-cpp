#pragma once
#include <stdexcept>
#include <string>

class Calculator {
    public:
    // Basic Arithmetic
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    double modulo(double a, double b);
    double power(double base, double exp);

    // Utility
    double evaluate(double a, const std::string& op, double b);

    // Scientific
    double calc_sin(double radians);
    double calc_cos(double radians);
    double calc_tan(double radians);
    double calc_sec(double radians);
    double calc_csc(double radians);
    double calc_cot(double radians);

    // Trigonometric with degrees
    double calc_sin_deg(double degrees);
    double calc_cos_deg(double degrees);
    double calc_tan_deg(double degrees);
    double calc_sec_deg(double degrees);
    double calc_csc_deg(double degrees);
    double calc_cot_deg(double degrees);

    // Logarithmic
    double calc_ln(double value);
    double calc_log10(double value);

    // Exponential
    double calc_exp(double exponent);

    // Other
    double calc_sqrt(double value);
    double calc_abs(double value);
};

class CalcError : public std::runtime_error {
    public:
    explicit CalcError(const std::string& message) : std::runtime_error(message) {}
};
