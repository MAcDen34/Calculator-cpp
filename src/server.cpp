 #include "httplib.h"
#include "parser.h"
#include "history.h"
// Drop dependency on nlohmann/json.hpp to avoid includePath issues.
#include <string>
#include <sstream>

static std::string json_escape(const std::string& s) {
    std::ostringstream o;
    for (auto c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    o << "\\u" << std::hex << (int)c;
                } else o << c;
        }
    }
    return o.str();
}

int main() {
    httplib::Server svr;
    History history;

    // POST /evaluate expects raw body containing the expression string.
    svr.Post("/evaluate", [&history](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string expression = req.body;
            Parser parser(expression);
            double result = parser.parse();
            history.add(expression, result);

            std::ostringstream resp;
            resp << "{\"expression\":\"" << json_escape(expression) << "\",\"result\":" << result << "}";
            res.set_content(resp.str(), "application/json");
        } catch (const std::exception& e) {
            std::ostringstream err;
            err << "{\"error\":\"" << json_escape(e.what()) << "\"}";
            res.status = 400;
            res.set_content(err.str(), "application/json");
        }
    });

    // GET /history returns array of entries
    svr.Get("/history", [&history](const httplib::Request& /*req*/, httplib::Response& res) {
        std::ostringstream out;
        out << '[';
        bool first = true;
        for (const auto& entry : history.getEntries()) {
            if (!first) out << ',';
            first = false;
            out << "{\"expression\":\"" << json_escape(entry.expression) << "\",\"result\":" << entry.result << '}';
        }
        out << ']';
        res.set_content(out.str(), "application/json");
    });

    // DELETE /history
    svr.Delete("/history", [&history](const httplib::Request& /*req*/, httplib::Response& res) {
        history.clear();
        res.set_content("{\"message\": \"History cleared.\"}", "application/json");
    });

    std::cout << "Server is running on http://localhost:8080\n";
    svr.listen("localhost", 8080);
    return 0;
}
