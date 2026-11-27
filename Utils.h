// Utils.h
// Purpose: Shared string helpers used by every module (trimming, case
//          conversion, splitting on '|', comment detection).
// Author:  Emmanuel, Andrew, Jacob (shared work)
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Removes spaces, tabs, \r and \n from both ends of s.
std::string trim(const std::string& s);

// Returns a copy of s with ASCII letters converted to uppercase.
std::string toUpper(const std::string& s);

// Returns a copy of s with ASCII letters converted to lowercase.
std::string toLower(const std::string& s);

// True if a and b are equal ignoring ASCII capitalization.
bool equalsIgnoreCase(const std::string& a, const std::string& b);

// Splits line on '|' and trims every field. "A|B" -> {"A","B"}.
std::vector<std::string> splitFields(const std::string& line);

// True if the line is blank after trimming.
bool isBlank(const std::string& line);

// True if the first non-whitespace character is '#'.
bool isComment(const std::string& line);

// Prints a rejection warning: "WARNING: file, line N: reason".
void printWarning(const std::string& file, int lineNum, const std::string& reason);

// Joins a directory and a filename with '/'.
std::string joinPath(const std::string& dir, const std::string& file);

#endif
