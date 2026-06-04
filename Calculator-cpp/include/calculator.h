#pragma once
#include <stdexcept>
#include <string>


class Calculator {
    public:

    // Basic Arithematic

    double  add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a,double b);
    double divide(double a, double b);
    double modulo(double a, double b);
    double power(double base, double exp);

    //Utility 

    double evaluate(double a, const std::string& op, double b); //"+","-","","/","%","^"
};

//Custom exception 
class CalcError : public std::runtime_error {
    public:
    explicit CalcError(const std::string& message) : std::runtime_error(message) {}
};
