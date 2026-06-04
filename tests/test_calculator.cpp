#include <gtest/gtest.h>
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
TEST_F(CalculatorTest, Evaluate)  { EXPECT_DOUBLE_EQ(calc.evaluate(3, "*", 4), 12); }
TEST_F(CalculatorTest, EvaluateInvalidOp) { EXPECT_THROW(calc.evaluate(3, "@", 4), CalcError); }

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
TEST_F(CalculatorTest, LnNegative) {
    EXPECT_THROW(calc.calc_ln(-1), CalcError);
}
TEST_F(CalculatorTest, Log10Zero) {
    EXPECT_THROW(calc.calc_log10(0), CalcError);
}
TEST_F(CalculatorTest, SqrtNegative) {
    EXPECT_THROW(calc.calc_sqrt(-1), CalcError);
}
TEST_F(CalculatorTest, SecUndefined) {
    EXPECT_THROW(calc.calc_sec_deg(90), CalcError);
}
TEST_F(CalculatorTest, CscUndefined) {
    EXPECT_THROW(calc.calc_csc(0), CalcError);
}
TEST_F(CalculatorTest, CotUndefined) {
    EXPECT_THROW(calc.calc_cot(0), CalcError);
}

// ── Expression parser — use TEST (no fixture needed) ──────
TEST(ParserTest, SimpleAdd)      { EXPECT_DOUBLE_EQ(Parser("3 + 4").parse(), 7); }
TEST(ParserTest, Precedence)     { EXPECT_DOUBLE_EQ(Parser("3 + 4 * 2").parse(), 11); }
TEST(ParserTest, Parentheses)    { EXPECT_DOUBLE_EQ(Parser("(3 + 4) * 2").parse(), 14); }
TEST(ParserTest, UnaryMinus)     { EXPECT_DOUBLE_EQ(Parser("-5 + 3").parse(), -2); }
TEST(ParserTest, NestedParens)   { EXPECT_DOUBLE_EQ(Parser("((2+3)*4)^2").parse(), 400); }
TEST(ParserTest, DivisionByZero) { EXPECT_THROW(Parser("5 / 0").parse(), std::runtime_error); }
TEST(ParserTest, InvalidSyntax)  { EXPECT_THROW(Parser("3 + * 4").parse(), std::runtime_error); }
TEST(ParserTest, ComplexExpression) { EXPECT_DOUBLE_EQ(Parser("3 + 4 * (2 - 1) / 5 ^ 2").parse(), 3.16); }

// ── Entry point ───────────────────────────────────────────
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

