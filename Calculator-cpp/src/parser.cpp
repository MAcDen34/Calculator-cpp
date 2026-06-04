#include "parser.h"
#include <cctype>
#include <cmath>
#include <stdexcept>

Parser::Parser(const std::string& input) : expr_(input), pos_(0) {}

double Parser::parse() {
    double result = parseExpression();
    skipWhitespace();
    if (pos_ != expr_.length()) {
        throw std::runtime_error("Unexpected character: " + std::string(1, expr_[pos_]));
    }
    return result;
}

char Parser::peek() {
    skipWhitespace();
    if (pos_ < expr_.length()) {
        return expr_[pos_];
    }
    return '\0';
}

char Parser::consumer() {
    skipWhitespace();
    if (pos_ < expr_.length()) {
        return expr_[pos_++];
    }
    return '\0';
}

void Parser::skipWhitespace() {
    while (pos_ < expr_.size() && std::isspace(expr_[pos_])) {
        ++pos_;
    }
}

// Addition and Subtraction (lowest precedence)
double Parser::parseExpression() {
    double left = parseTerm();
    while (peek() == '+' || peek() == '-') {
        char op = consumer();
        double right = parseTerm();
        left = (op == '+') ? left + right : left - right;
    }
    return left;
}

// Multiplication and Division (medium precedence)
double Parser::parseTerm() {
    double left = parsePower();
    while (peek() == '*' || peek() == '/' || peek() == '%') {
        char op = consumer();
        double right = parsePower();
        if ((op == '/' || op == '%') && right == 0) {
            throw std::runtime_error("Division by zero");
        }
        if (op == '*') {
            left *= right;
        } else if (op == '/') {
            left /= right;
        } else {
            left = std::fmod(left, right);
        }
    }
    return left;
}

//Power (right-associative)
double Parser::parsePower() {
    double left = parsePrimary();
    if (peek() == '^') {
        consumer(); // consume '^'
        double right = parsePower(); // right-associative
        return std::pow(left, right);
    }
    return left;
}

// Numbers and Parentheses (highest precedence)
double Parser::parsePrimary() {
    char c = peek();
    if (c == '+' || c == '-') {
        char op = consumer();
        double value = parsePrimary();
        return (op == '-') ? -value : value;
    }
// Handle parentheses
    if (c == '(') {
        consumer();
        double value = parseExpression();
        if (peek() != ')') {
            throw std::runtime_error("Expected ')'");
        }
        consumer();
        return value;
    }
//Unary minus

if (c == '-') {
        consumer();
        return -parsePrimary();
    }

// Handle numbers
    if (std::isdigit(c) || c == '.') {
        size_t start = pos_;
        while (pos_ < expr_.size() && (std::isdigit(expr_[pos_]) || expr_[pos_] == '.')) {
            ++pos_;
        }
        return std::stod(expr_.substr(start, pos_ - start));
    }
    throw std::runtime_error(std::string("Unexpected character: ") + c);
}
