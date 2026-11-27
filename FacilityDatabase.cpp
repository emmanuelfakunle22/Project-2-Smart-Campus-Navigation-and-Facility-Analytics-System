// FacilityDatabase.cpp
// Purpose: Facility loading, display, hash-table ID search, name and
//          category search, and the category report.
// Author:  Emmanuel (Member A - facilities / hash table)
#include "FacilityDatabase.h"
#include "CampusGraph.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
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
            Facility fac;
            fac.id = id;
            fac.name = f[1];
            fac.building = graph.getBuildingName(b);   // buildings.txt spelling
            fac.category = f[3];

            // insert() is the duplicate check: it returns false (and changes
            // nothing) if the ID exists, so the list and table stay in sync.
            if (!table.insert(fac)) {
                printWarning(displayName, lineNum, "duplicate facility ID " + id);
                rejected++; continue;
            }
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

// Simple scan in file order. Both sides are lowercased so capitalization
// does not matter; internal spaces must match exactly.
void FacilityDatabase::searchByName(const std::string& input) const {
    std::string key = toLower(trim(input));
    int matches = 0;
    for (size_t i = 0; i < facilities.size(); i++) {
        if (toLower(facilities[i].name) == key) {
            printFacility(facilities[i]);
            matches++;
        }
    }
    if (matches == 0) std::cout << "No facility named '" << trim(input) << "'.\n";
    std::cout << "Matches: " << matches << "\n";
}

// Same scan as searchByName, but on the category field.
void FacilityDatabase::searchByCategory(const std::string& input) const {
    std::string key = toLower(trim(input));
    int matches = 0;
    for (size_t i = 0; i < facilities.size(); i++) {
        if (toLower(facilities[i].category) == key) {
            printFacility(facilities[i]);
            matches++;
        }
    }
    if (matches == 0) std::cout << "No facilities in category '" << trim(input) << "'.\n";
    std::cout << "Matches: " << matches << "\n";
}

// Builds a list of distinct categories in first-seen order. Two parallel
// vectors: names[i] is the first accepted spelling, counts[i] its count.
void FacilityDatabase::categoryReport() const {
    if (facilities.empty()) {
        std::cout << "No facilities are available.\n";
        std::cout << "Total: 0\n";
        return;   // never divide by zero
    }
    std::vector<std::string> names;
    std::vector<int> counts;
    for (size_t i = 0; i < facilities.size(); i++) {
        std::string key = toLower(facilities[i].category);
        bool found = false;
        for (size_t j = 0; j < names.size(); j++) {
            if (toLower(names[j]) == key) {
                counts[j]++;
                found = true;
                break;
            }
        }
        if (!found) {
            names.push_back(facilities[i].category);
            counts.push_back(1);
        }
    }

    double total = static_cast<double>(facilities.size());
    std::cout << std::left << std::setw(28) << "Category"
              << std::right << std::setw(8) << "Count"
              << std::setw(13) << "Percentage" << "\n";
    std::cout << std::fixed << std::setprecision(1);
    for (size_t j = 0; j < names.size(); j++) {
        std::cout << std::left << std::setw(28) << names[j]
                  << std::right << std::setw(8) << counts[j]
                  << std::setw(12) << (counts[j] * 100.0 / total) << "%\n";
    }
    std::cout << std::left << std::setw(28) << "Total"
              << std::right << std::setw(8) << facilities.size()
              << std::setw(12) << 100.0 << "%\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout << std::setprecision(6);
}
