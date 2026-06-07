# Calculator-cpp

A feature-rich C++ calculator with a native Qt6 GUI, a fancy FTXUI terminal UI, scientific functions, function graphing, multithreading, and Docker support.

---

## Features

- **Basic arithmetic** — addition, subtraction, multiplication, division, modulo, power
- **Scientific functions** — sin, cos, tan, sec, csc, cot (radians and degrees), asin, acos, atan, ln, log₁₀, exp, 10ˣ, √, x², 1/x, |x|, n!, π, e
- **Expression parser** — evaluates full expressions like `3 + 4 * sin(45)` with correct operator precedence
- **Function grapher** — plots functions like sin(x), cos(x), x², sqrt(x) using QCustomPlot
- **Calculation history** — grouped by Today / Previous 7 Days, logged asynchronously to `history.log`
- **Two interfaces** — native Qt6 GUI and FTXUI terminal UI
- **Multithreading** — async calculations keep the GUI responsive; background thread writes history
- **Docker support** — containerized CLI and test suite via Docker Compose
- **25 unit tests** — Google Test suite covering all operations and edge cases

---

## Project Structure

```
Calculator-cpp/
├── src/
│   ├── calculator.cpp        # Core arithmetic + scientific functions
│   ├── parser.cpp            # Expression parser (handles precedence, parentheses)
│   ├── history.cpp           # History storage
│   └── history_writer.cpp    # Background thread — async file writer
├── include/
│   ├── calculator.h
│   ├── parser.h
│   ├── history.h
│   ├── history_writer.h      # std::thread + std::mutex + condition_variable
│   ├── calc_worker.h         # std::async + std::future
│   └── thread_safe_queue.h   # Mutex-protected queue
├── cli/
│   └── main.cpp              # FTXUI terminal UI
├── gui/
│   ├── main.cpp              # Qt6 app entry point
│   ├── mainwindow.h/.cpp     # Apple Calculator-style Qt6 GUI
│   ├── graphwindow.h/.cpp    # QCustomPlot function grapher
│   └── qcustomplot/          # QCustomPlot source
├── tests/
│   └── test_calculator.cpp   # Google Test suite (25 tests)
├── CMakeLists.txt
├── Dockerfile
└── docker-compose.yml
```

---

## Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++17 |
| GUI | Qt6 (Widgets, Charts, PrintSupport) |
| Graphing | QCustomPlot 2.1.1 |
| Terminal UI | FTXUI v5.0.0 |
| Testing | Google Test 1.14+ |
| Build | CMake 3.15+ |
| Threading | std::thread, std::mutex, std::async, std::future, std::atomic |
| Containers | Docker + Docker Compose |

---

## Threading Architecture

```
Main Thread (Qt6 GUI)
    ├── CalcWorker          → std::async fires calculation on new thread
    │                         result returned via QMetaObject::invokeMethod
    └── HistoryWriter       → std::thread sleeping on condition_variable
                              wakes when entry pushed to ThreadSafeQueue
                              writes to history.log asynchronously
```

---

## Building Locally

**Requirements:** CMake 3.15+, C++17 compiler, Google Test, Qt6, FTXUI (auto-fetched)

```bash
git clone https://github.com/MAcDen34/Calculator-cpp.git
cd Calculator-cpp
mkdir build && cd build
cmake ..
make
```

**Run the GUI:**
```bash
./calculator_gui
```

**Run the CLI:**
```bash
./calculator
```

**Run tests:**
```bash
./tests
```

---

## Docker

```bash
# Build both images
docker-compose build

# Run the test suite inside Docker
docker-compose up tests

# Run the CLI interactively
docker-compose run --rm cli
```

---

## Scientific Functions Reference

| Function | Input | Description |
|----------|-------|-------------|
| `calc_sin(x)` | radians | Sine |
| `calc_cos(x)` | radians | Cosine |
| `calc_tan(x)` | radians | Tangent |
| `calc_sec(x)` | radians | Secant (1/cos) |
| `calc_csc(x)` | radians | Cosecant (1/sin) |
| `calc_cot(x)` | radians | Cotangent (cos/sin) |
| `calc_sin_deg(x)` | degrees | Sine (degree input) |
| `calc_ln(x)` | x > 0 | Natural logarithm |
| `calc_log10(x)` | x > 0 | Base-10 logarithm |
| `calc_exp(x)` | any | eˣ |
| `calc_sqrt(x)` | x ≥ 0 | Square root |
| `calc_abs(x)` | any | Absolute value |

---

## GUI Features

- Dark theme matching Apple Calculator
- Round buttons with orange operators
- Basic and Scientific modes (toggle via ⊞ menu)
- History panel with slide-up animation (tap ⏱ or click display)
- Function grapher (tap 📈) — plots sin, cos, tan, sqrt, ln, exp, x², 1/x, |x|
- Auto-shrinking display font for long numbers
- All calculations logged asynchronously to `history.log`

---

## Test Suite

```
25 tests across 2 suites:
  CalculatorTest (17) — arithmetic, scientific, error handling
  ParserTest     (8)  — expressions, precedence, parentheses, edge cases
```

Run with:
```bash
cd build && ./tests
```

---

## Roadmap

- [ ] Python reverse project (same calculator rebuilt in Python)
- [ ] Plot history of past results on the graph
- [ ] HTTP server integration (already scaffolded in `src/server.cpp`)
- [ ] CI/CD pipeline with GitHub Actions

---

## Author

**Denzel Ngabo** — [GitHub](https://github.com/MAcDen34)
