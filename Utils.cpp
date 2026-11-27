// Utils.cpp
// Purpose: Implementation of the shared string helpers.
// Author:  Emmanuel, Andrew, Jacob (shared work)
#include "Utils.h"
#include <iostream>
#include <cctype>

// Characters treated as whitespace when trimming (includes \r for Windows files).
static bool isWs(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

// Removes leading and trailing whitespace.
std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && isWs(s[start])) start++;
    size_t end = s.size();
    while (end > start && isWs(s[end - 1])) end--;
    return s.substr(start, end - start);
}

// Uppercases ASCII letters.
std::string toUpper(const std::string& s) {
    std::string out = s;
    for (size_t i = 0; i < out.size(); i++)
        out[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(out[i])));
    return out;
}

// Lowercases ASCII letters.
std::string toLower(const std::string& s) {
    std::string out = s;
    for (size_t i = 0; i < out.size(); i++)
        out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
    return out;
}

// Case-insensitive equality.
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toLower(a) == toLower(b);
}

// Splits on '|' and trims each field. An empty field stays as "".
std::vector<std::string> splitFields(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == '|') {
            fields.push_back(trim(current));
            current.clear();
        } else {
            current += line[i];
        }
    }
    fields.push_back(trim(current));
    return fields;
}

// Blank line check.
bool isBlank(const std::string& line) {
    return trim(line).empty();
}

// Comment line check: first non-whitespace char is '#'.
bool isComment(const std::string& line) {
    std::string t = trim(line);
    return !t.empty() && t[0] == '#';
}

// Prints a standard rejection warning.
void printWarning(const std::string& file, int lineNum, const std::string& reason) {
    std::cout << "WARNING: " << file << ", line " << lineNum << ": " << reason << "\n";
}

// Builds "dir/file", avoiding a double slash.
std::string joinPath(const std::string& dir, const std::string& file) {
    if (dir.empty()) return file;
    if (dir[dir.size() - 1] == '/') return dir + file;
    return dir + "/" + file;
}
