#include "Utils.h"

string trim(string str) {
    size_t start = str.find_first_not_of(" \t\n\r");

    if (start == string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r");

    return str.substr(start, end - start + 1);
}