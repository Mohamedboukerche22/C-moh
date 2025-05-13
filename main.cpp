#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <functional>
#include <cctype>
#include <stdexcept>

using namespace std;
vector<string> tokenize(const string& line) {
    vector<string> tokens;
    string token;
    bool inString = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            token += c;
            inString = !inString;
        } else if (inString) {
            token += c;
        } else if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (c == '=' || c == ':' || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '>' || c == '<') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            string op(1, c);
            if (i + 1 < line.size() && (line[i + 1] == '=')) {
                op += '=';
                i++;
            }
            tokens.push_back(op);
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}
map<string, double> variables;

double evaluate_expression(const vector<string>& tokens, size_t& pos) {
    double result = 0;
    string op = "+";

    while (pos < tokens.size()) {
        string token = tokens[pos];

        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            double value = stod(token);
            if (op == "+") result += value;
            else if (op == "-") result -= value;
            else if (op == "*") result *= value;
            else if (op == "/") result /= value;
        } else if (variables.count(token)) {
            double value = variables[token];
            if (op == "+") result += value;
            else if (op == "-") result -= value;
            else if (op == "*") result *= value;
            else if (op == "/") result /= value;
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            op = token;
        } else {
            break;
        }

        ++pos;
    }

    return result;
}
void execute_line(const vector<string>& tokens, size_t& pos, vector<string>& program, size_t& current_line);

void run_block(vector<string>& program, size_t& current_line, int indent) {
    while (current_line < program.size()) {
        string line = program[current_line];
        int new_indent = 0;
        while (new_indent < line.size() && line[new_indent] == ' ') new_indent++;
        if (new_indent <= indent) break;

        auto tokens = tokenize(line);
        size_t pos = 0;
        execute_line(tokens, pos, program, current_line);
        current_line++;
    }
}
void execute_line(const vector<string>& tokens, size_t& pos, vector<string>& program, size_t& current_line) {
    if (tokens.empty()) return;

    if (tokens[0] == "print") {
        if (tokens[1][0] == '"') {
            string msg = tokens[1].substr(1, tokens[1].length() - 2);
            cout << msg << endl;
        } else {
            string var = tokens[1];
            if (variables.count(var)) {
                cout << variables[var] << endl;
            } else {
                throw runtime_error("Undefined variable: " + var);
            }
        }
    } else if (tokens[0] == "let") {
        string name = tokens[1];
        pos = 3;
        double value = evaluate_expression(tokens, pos);
        variables[name] = value;
    } else if (variables.count(tokens[0]) && tokens[1] == "=") {
        string name = tokens[0];
        pos = 2;
        double value = evaluate_expression(tokens, pos);
        variables[name] = value;
    } else if (tokens[0] == "if") {
        string var = tokens[1];
        string cmp = tokens[2];
        string value = tokens[3];
        bool condition = false;

        if (variables.count(var)) {
            double left = variables[var];
            double right = stod(value);
            if (cmp == "==") condition = (left == right);
            else if (cmp == ">") condition = (left > right);
            else if (cmp == "<") condition = (left < right);
        }

        if (condition && tokens.back() == ":") {
            current_line++;
            run_block(program, current_line, 4);
            current_line--;
        } else {
            current_line++;
            while (current_line < program.size() && program[current_line].find_first_not_of(" ") >= 4) {
                current_line++;
            }
            current_line--;
        }
    } else if (tokens[0] == "while") {
        string var = tokens[1];
        string cmp = tokens[2];
        string value = tokens[3];
        int loop_start = current_line;
        while (true) {
            if (!variables.count(var)) break;
            double left = variables[var];
            double right = stod(value);
            bool condition = false;
            if (cmp == ">") condition = (left > right);
            else if (cmp == "<") condition = (left < right);
            else if (cmp == "==") condition = (left == right);
            if (!condition) break;
            size_t temp_line = loop_start + 1;
            run_block(program, temp_line, 4);
        }
        while (current_line < program.size() && program[current_line + 1].find_first_not_of(" ") >= 4) {
            current_line++;
        }
    }
}
int main() {
    vector<string> program;
    string line;

    cout << "Enter your program (type END to finish):\n";
    while (getline(cin, line)) {
        if (line == "END") break;
        program.push_back(line);
    }

    size_t current_line = 0;
    while (current_line < program.size()) {
        auto tokens = tokenize(program[current_line]);
        size_t pos = 0;
        try {
            execute_line(tokens, pos, program, current_line);
        } catch (exception& e) {
            cerr << "Error on line " << current_line + 1 << ": " << e.what() << endl;
        }
        current_line++;
    }

    return 0;
}
