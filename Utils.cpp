#include "Utils.h"
#include <cctype>
#include <vector>

string trim(string str) {
    size_t start = str.find_first_not_of(" \t\n\r");

    if (start == string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r");

    return str.substr(start, end - start + 1);
}

string toUpper(const string& s) {
    string result = s;

    for (int i = 0; i < result.length(); i++) {
        result[i] = toupper(result[i]);
    }

    return result;
}

vector<string> split(const string& line, char delimiter) {

    vector<string> result;
    string current;

    for (size_t i = 0; i < line.length(); i++) {

        if (line[i] == delimiter) {
            result.push_back(current);
            current.clear();
        }
        else {
            current += line[i];
        }
    }

    result.push_back(current);

    return result;
}
