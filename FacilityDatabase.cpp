// FacilityDatabase.cpp
// Purpose: Facility loading, display, and hash-table ID search.
// Author:  Emmanuel (Member A - facilities / hash table)
#include "FacilityDatabase.h"
#include "CampusGraph.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <cctype>

// Starts with an empty list and a table of the default capacity.
FacilityDatabase::FacilityDatabase() : table(DEFAULT_HASH_CAPACITY) {}

// Valid ID: exactly 4 characters, 'F' then three digits.
bool FacilityDatabase::isValidId(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') return false;
    for (int i = 1; i < 4; i++) {
        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
    }
    return true;
}

// Prints one facility record on a single line.
void FacilityDatabase::printFacility(const Facility& f) {
    std::cout << f.id << " | " << f.name << " | " << f.building
              << " | " << f.category << "\n";
}

// Reads facilities.txt line by line. Check order: field count, empty fields,
// ID format, unknown building, duplicate ID.
void FacilityDatabase::load(const std::string& path, const std::string& displayName,
                            const CampusGraph& graph) {
    int accepted = 0, rejected = 0;
    std::ifstream in(path.c_str());
    if (!in) {
        std::cout << "ERROR: could not open " << path << " (treated as empty)\n";
    } else {
        std::string line;
        int lineNum = 0;
        while (std::getline(in, line)) {
            lineNum++;
            if (isBlank(line) || isComment(line)) continue;

            std::vector<std::string> f = splitFields(line);
            if (f.size() != 4) {
                printWarning(displayName, lineNum, "expected 4 fields, found " +
                             std::to_string(f.size()));
                rejected++; continue;
            }
            if (f[0].empty() || f[1].empty() || f[2].empty() || f[3].empty()) {
                printWarning(displayName, lineNum, "empty required field");
                rejected++; continue;
            }
            std::string id = toUpper(f[0]);
            if (!isValidId(id)) {
                printWarning(displayName, lineNum, "invalid ID format '" + f[0] +
                             "' (must be F followed by exactly three digits)");
                rejected++; continue;
            }
            int b = graph.findBuilding(f[2]);
            if (b < 0) {
                printWarning(displayName, lineNum, "unknown building '" + f[2] + "'");
                rejected++; continue;
            }
            if (table.contains(id)) {
                printWarning(displayName, lineNum, "duplicate facility ID " + id);
                rejected++; continue;
            }

            Facility fac;
            fac.id = id;
            fac.name = f[1];
            fac.building = graph.getBuildingName(b);   // buildings.txt spelling
            fac.category = f[3];
            table.insert(fac);
            facilities.push_back(fac);
            accepted++;
        }
    }
    std::cout << displayName << ": " << accepted << " accepted, " << rejected
              << " rejected (hash capacity " << table.getCapacity()
              << ", entries " << table.getSize()
              << ", collisions " << table.getCollisions() << ")\n";
}

// Lists facilities in file order.
void FacilityDatabase::displayAll() const {
    if (facilities.empty()) {
        std::cout << "No facilities are loaded.\n";
        return;
    }
    std::cout << "ID | Name | Building | Category\n";
    for (size_t i = 0; i < facilities.size(); i++) printFacility(facilities[i]);
    std::cout << "Total facilities: " << facilities.size() << "\n";
}

// Normalizes the typed ID, then uses the hash table (no scan).
void FacilityDatabase::searchById(const std::string& input) const {
    std::string id = toUpper(trim(input));
    if (!isValidId(id)) {
        std::cout << "Invalid format: an ID is F followed by exactly three digits.\n";
        return;
    }
    const Facility* f = table.find(id);
    if (f == nullptr) {
        std::cout << "Not found: no facility with ID " << id << "\n";
    } else {
        std::cout << "Found: ";
        printFacility(*f);
    }
}
