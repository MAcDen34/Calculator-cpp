#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>
#include "calculator.h"
#include "parser.h"
#include "history.h"
 
using namespace ftxui;
 
int main() {
    Calculator calc;
    History history;
    auto screen = ScreenInteractive::Fullscreen();
 
    // ── State ─────────────────────────────────────────────
    std::vector<std::string> menu_entries = {
        "Basic Calculator",
        "Scientific Calculator",
        "View History",
        "Exit"
    };
 
    std::vector<std::string> sci_functions = {
        "sin", "cos", "tan",
        "sec", "csc", "cot",
        "sin_deg", "cos_deg", "tan_deg",
        "ln", "log10", "exp",
        "sqrt", "abs",
        "Type expression directly"
    };
 
    int menu_selected   = 0;
    int sci_selected    = 0;
    std::string input;
    std::string result_display = "No calculation yet";
    std::vector<std::string> history_lines;
    bool show_scientific = false;
 
    // ── Helpers ───────────────────────────────────────────
    auto do_calculate = [&]() {
        if (input.empty()) return;
        try {
            if (show_scientific &&
                sci_selected < (int)sci_functions.size() - 1) {
                // Function mode: e.g. sin with value typed
                double val = std::stod(input);
                double res = calc.evaluate(val, sci_functions[sci_selected], 0);
                result_display = sci_functions[sci_selected] +
                                 "(" + input + ") = " +
                                 std::to_string(res);
            } else {
                // Expression mode: e.g. 3 + 4 or sin(45)
                Parser p(input);
                double res = p.parse();
                result_display = input + " = " + std::to_string(res);
            }
            history.add(input, 0.0);
            history_lines.push_back(result_display);
        } catch (const std::exception& e) {
            result_display = std::string("Error: ") + e.what();
        }
        input.clear();
    };
 
    auto do_clear = [&]() {
        input.clear();
        result_display = "Cleared.";
    };
 
    // ── Components ────────────────────────────────────────
    auto main_menu  = Menu(&menu_entries, &menu_selected);
    auto sci_menu   = Menu(&sci_functions, &sci_selected);
    auto input_box  = Input(&input, "Type value or expression...");
    auto btn_clear  = Button("[ Clear ]", do_clear, ButtonOption::Ascii());
    auto btn_calc   = Button("[ Calculate ]", do_calculate, ButtonOption::Ascii());
 
    auto layout = Container::Vertical({
        main_menu,
        sci_menu,
        input_box,
        Container::Horizontal({btn_clear, btn_calc}),
    });
 
    // ── Renderer ──────────────────────────────────────────
    auto renderer = Renderer(layout, [&] {
        // History lines (last 5)
        Elements hist_elements;
        int start = std::max(0, (int)history_lines.size() - 5);
        for (int i = start; i < (int)history_lines.size(); i++)
            hist_elements.push_back(text("  " + history_lines[i]) | dim);
        if (hist_elements.empty())
            hist_elements.push_back(text("  No history yet") | dim);
 
        Elements content;
 
        // Title
        content.push_back(
            text("  Scientific Calculator  ") | bold | color(Color::Cyan) | border
        );
        content.push_back(separator());
 
        // Main menu
        content.push_back(text("Menu:") | bold);
        content.push_back(main_menu->Render() | border);
        content.push_back(separator());
 
        // Scientific submenu (only when selected)
        if (show_scientific) {
            content.push_back(text("Scientific Function:") | bold);
            content.push_back(sci_menu->Render() | border | size(HEIGHT, LESS_THAN, 10));
            content.push_back(separator());
        }
 
        // Input
        content.push_back(
            hbox(text("Input: ") | bold, input_box->Render()) | border
        );
 
        // Buttons
        content.push_back(
            hbox(btn_clear->Render(), text("  "), btn_calc->Render())
        );
        content.push_back(separator());
 
        // Result
        content.push_back(
            text("Result: " + result_display) | color(Color::Green) | bold
        );
        content.push_back(separator());
 
        // History
        content.push_back(text("Recent History:") | bold);
        content.push_back(vbox(hist_elements));
        content.push_back(separator());
 
        // Help bar
        content.push_back(
            text("↑↓ navigate  |  Tab = focus input  |  Enter = select/calculate  |  Esc = quit") | dim
        );
 
        return vbox(content);
    });
 
    // ── Event handling ────────────────────────────────────
    auto component = CatchEvent(renderer, [&](Event event) {
        // Enter on menu
        if (event == Event::Return) {
            if (main_menu->Focused()) {
                if (menu_selected == 0) { show_scientific = false; return true; }
                if (menu_selected == 1) { show_scientific = true;  input_box->TakeFocus(); return true; }
                if (menu_selected == 2) {
                    // Show history
                    if (history_lines.empty())
                        result_display = "No history yet";
                    else
                        result_display = "Showing last " +
                            std::to_string(std::min(5, (int)history_lines.size())) +
                            " entries above";
                    return true;
                }
                if (menu_selected == 3) {
                    screen.ExitLoopClosure()();
                    return true;
                }
            }
                if (event == Event::Return && sci_menu->Focused()) {
                    input_box->TakeFocus();
                    return true;
            }

            // Enter on input box = calculate
            if (input_box->Focused()) {
                do_calculate();
                return true;
            }
        }
 
        // Ctrl+L = clear
        if (event == Event::Special("\x0c")) {
            do_clear();
            return true;
        }
 
        // Escape = quit
        if (event == Event::Escape) {
            screen.ExitLoopClosure()();
            return true;
        }
 
        return false;
    });
 
    screen.Loop(component);
    return 0;
}
 