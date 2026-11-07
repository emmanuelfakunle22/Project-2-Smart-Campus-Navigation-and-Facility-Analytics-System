// Facility.h
// Purpose: Facility record (one line of facilities.txt).
// Author:  Emmanuel (Member A - facilities / hash table)
#ifndef FACILITY_H
#define FACILITY_H

#include <string>

// One campus facility. id is always stored uppercase (e.g. "F004").
struct Facility {
    std::string id;
    std::string name;
    std::string building;   // spelling taken from buildings.txt
    std::string category;
};

#endif
