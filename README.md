# Calculator-cpp

A feature-rich C++ calculator with basic arithmetic, scientific functions, expression parsing, and a polished GUI — built with clean OOP architecture, tested with Google Test, and containerized with Docker.

---

## Features

- **Basic Arithmetic** — addition, subtraction, multiplication, division, modulo, power
- **Scientific Functions** — trigonometry (sin, cos, tan, sec, csc, cot) in radians and degrees, logarithms (ln, log₁₀), exponential, square root, absolute value
- **Expression Parser** — evaluates full expressions like `3 + 4 * (2 - 1) / 5 ^ 2` with correct operator precedence and parentheses
- **Calculation History** — stores past expressions and results, grouped by time period
- **Two Interfaces** — a native Qt6 GUI and an FTXUI terminal UI
- **Dockerized** — runs tests and CLI in isolated containers

---

## Interfaces

### Qt6 GUI
A native desktop app styled after the Apple Calculator — dark theme, round buttons, orange operators, and a slide-up history panel.

- Switch between **Basic** and **Scientific** modes via the ⊞ menu
- History panel opens via the ⏱ button or clicking the display
- History grouped into **Today** and **Previous 7 Days**

### FTXUI CLI
An interactive terminal UI with a menu-driven flow.

- Choose between Basic, Scientific, and History modes
- Type expressions directly or select scientific functions from a list
- Clear button and keyboard shortcuts supported

---

## Project Structure

```
Calculator-cpp/
├── src/
│   ├── calculator.cpp     # Core arithmetic + scientific logic
│   ├── parser.cpp         # Expression parser (precedence, parentheses)
│   ├── history.cpp        # Calculation history manager
│   ├── server.cpp         # HTTP server (httplib)
│   └── main.cpp           # Entry point
├── include/
│   ├── calculator.h
│   ├── parser.h
│   ├── history.h
│   └── httplib.h
├── cli/
│   └── main.cpp           # FTXUI terminal UI
├── gui/
│   ├── main.cpp           # Qt6 entry point
│   ├── mainwindow.cpp     # Qt6 main window
│   └── mainwindow.h
├── tests/
│   └── test_calculator.cpp  # Google Test suite (25 tests)
├── Dockerfile
├── docker-compose.yml
└── CMakeLists.txt
```

---

## Tech Stack

| Layer | Technology |
|---|---|
| Language | C++17 |
| GUI | Qt6 (Widgets, Charts) |
| Terminal UI | FTXUI v5.0.0 |
| Testing | Google Test 1.17.0 |
| Build System | CMake 3.15+ |
| Containerization | Docker + Docker Compose |
| HTTP Server | cpp-httplib |

---

## Getting Started

### Prerequisites

- macOS with Xcode Command Line Tools
- [Homebrew](https://brew.sh)
- CMake, Qt6, Google Test

```bash
xcode-select --install
brew install cmake qt googletest
```

### Build & Run (Local)

```bash
git clone https://github.com/MAcDen34/Calculator-cpp.git
cd Calculator-cpp

mkdir build && cd build
cmake ..
make
```

Run the GUI:
```bash
./calculator_gui
```

Run the CLI:
```bash
./calculator
```

Run tests:
```bash
./tests
```

---

## Docker

Build and run everything in containers — no local dependencies needed (GUI excluded).

```bash
# Build all images
docker-compose build

# Run the test suite
docker-compose up tests

# Run the CLI
docker-compose up cli
```

---

## Running Tests

25 tests across 2 suites:

```
CalculatorTest   (17 tests) — arithmetic, scientific functions, error handling
ParserTest       (8 tests)  — expression parsing, precedence, edge cases
```

```bash
cd build && ./tests
# [  PASSED  ] 25 tests.
```

---

## OOP Architecture

The project is built around three core classes:

```
Calculator          — encapsulates all math operations
    └── inherits nothing, composed into GUI/CLI layers

Parser              — recursive descent expression evaluator
    └── uses Calculator internally

History             — thread-safe history log
    └── stores HistoryEntry { expression, result, timestamp }
```

Key C++ concepts demonstrated:
- **Encapsulation** — private fields, public interfaces
- **Inheritance** — extensible calculator hierarchy
- **Polymorphism** — virtual dispatch across calculator types
- **Exception handling** — custom `CalcError` for undefined operations
- **Templates & STL** — `std::vector`, `std::string`, `std::fmod`

---

## Scientific Functions Reference

| Function | Input | Notes |
|---|---|---|
| `sin`, `cos`, `tan` | radians | |
| `sin_deg`, `cos_deg`, `tan_deg` | degrees | auto-converts |
| `sec`, `csc`, `cot` | radians | throws on undefined angles |
| `ln` | value > 0 | natural log |
| `log10` | value > 0 | base-10 log |
| `exp` | any | eˣ |
| `sqrt` | value ≥ 0 | throws on negative |
| `abs` | any | absolute value |

---

## Roadmap

- [ ] Multithreading — async calc worker, history writer thread, thread-safe queue
- [ ] Graphing — plot functions like `sin(x)`, `x²` with QCustomPlot
- [ ] Memory functions — M+, M-, MR, MC
- [ ] Unit converter mode

---

## Author

**Denzel Ngabo** — [@MAcDen34](https://github.com/MAcDen34)
