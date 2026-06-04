#pragma once
#include<string>
#include<stdexcept>

class Parser {
    public:
    explicit Parser(const std::string& input);
    double parse();

    char peek();

    char consumer();

private:
    std::string expr_;
    size_t pos_;

    double parseExpression(); //Handles addition and subtraction
    double parseTerm(); // Handles multiplication and division
    double parsePower(); // Handles exponentiation
    double parsePrimary(); // Handles numbers and parentheses

    char peek() const;
    char get();
    void skipWhitespace();
};
