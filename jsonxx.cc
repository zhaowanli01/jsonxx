// Author: Hong Jiang <hong@hjiang.net>

#include "jsonxx.h"

#include <iostream>
#include <cctype>

namespace jsonxx {

void eat_whitespaces(std::istream& input) {
    char ch;
    do {
        input.get(ch);
    } while(isspace(ch));
    input.putback(ch);
}

// Try to consume characters from the input stream and match the
// pattern string. Leading whitespaces from the input are ignored if
// ignore_ws is true.
bool match(const std::string& pattern, std::istream& input,
           bool ignore_ws = true) {
    if (ignore_ws) {
        eat_whitespaces(input);
    }
    std::string::const_iterator cur(pattern.begin());
    char ch(0);
    while(input && !input.eof() && cur != pattern.end()) {
        input.get(ch);
        if (ch != *cur) {
            input.putback(ch);
            return false;
        } else {
            cur++;
        }
    }
    return cur == pattern.end();
}

bool parse_string(std::istream& input) {
    if (!match("\"", input))  {
        return false;
    }
    char ch;
    while(!input.eof() && input.good()) {
        input.get(ch);
        if (ch == '"') {
            break;
        }
    }
    if (input && ch == '"') {
        return true;
    } else {
        return false;
    }
}

bool parse_number(std::istream& input) {
    eat_whitespaces(input);
    char ch;
    int num_digits = 0;
    while(input && !input.eof()) {
        input.get(ch);
        if (!isdigit(ch)) {
            input.putback(ch);
            break;
        }
        num_digits++;
    }
    if (num_digits > 0) {
        return true;
    } else {
        return false;
    }
}

bool Object::parse(std::istream& input) {
    if (!match("{", input)) {
        return false;
    }
    if (!parse_string(input)) {
        return false;
    }
    if (!match(":", input)) {
        return false;
    }
    if (!parse_number(input)) {
        return false;
    }
    if (!match("}", input)) {
        return false;
    }
    return true;
}

}  // namespace jsonxx
