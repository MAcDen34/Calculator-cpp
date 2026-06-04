#include <gtest/gtest.h>
#if __has_include(<gtest/gtest.h>)
#include <gtest/gtest.h>
#elif __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#elif __has_include("gtest.h")
#include "gtest.h"
#else
#include "gtest/gtest.h"
#endif

#include "calculator.h"
#include "parser.h"

// ── Fixture — defined OUTSIDE, tests reference it ─────────
class CalculatorTest : public ::testing::Test {
protected:
    Calculator calc;  // ← only member data/setup here
};

// ── Basic operations — use TEST_F for fixture ─────────────
TEST_F(CalculatorTest, Add)      { EXPECT_DOUBLE_EQ(calc.add(3, 4), 7); }
TEST_F(CalculatorTest, Subtract) { EXPECT_DOUBLE_EQ(calc.subtract(10, 4), 6); }
TEST_F(CalculatorTest, Multiply) { EXPECT_DOUBLE_EQ(calc.multiply(3, 4), 12); }
TEST_F(CalculatorTest, Divide)   { EXPECT_DOUBLE_EQ(calc.divide(10, 2), 5); }
TEST_F(CalculatorTest, Modulo)   { EXPECT_DOUBLE_EQ(calc.modulo(10, 3), 1); }
TEST_F(CalculatorTest, Power)    { EXPECT_DOUBLE_EQ(calc.power(2, 8), 256); }

// ── Error handling ────────────────────────────────────────
TEST_F(CalculatorTest, DivideByZero) {
    EXPECT_THROW(calc.divide(5, 0), CalcError);
}
TEST_F(CalculatorTest, ModuloByZero) {
    EXPECT_THROW(calc.modulo(5, 0), CalcError);
}
TEST_F(CalculatorTest, InvalidOperator) {
    EXPECT_THROW(calc.evaluate(1, "@", 2), CalcError);
}

// ── Expression parser — use TEST (no fixture needed) ──────
TEST(ParserTest, SimpleAdd)      { EXPECT_DOUBLE_EQ(Parser("3 + 4").parse(), 7); }
TEST(ParserTest, Precedence)     { EXPECT_DOUBLE_EQ(Parser("3 + 4 * 2").parse(), 11); }
TEST(ParserTest, Parentheses)    { EXPECT_DOUBLE_EQ(Parser("(3 + 4) * 2").parse(), 14); }
TEST(ParserTest, UnaryMinus)     { EXPECT_DOUBLE_EQ(Parser("-5 + 3").parse(), -2); }
TEST(ParserTest, NestedParens)   { EXPECT_DOUBLE_EQ(Parser("((2+3)*4)^2").parse(), 400); }
TEST(ParserTest, DivisionByZero) { EXPECT_THROW(Parser("5 / 0").parse(), std::runtime_error); }

// ── Entry point ───────────────────────────────────────────
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

