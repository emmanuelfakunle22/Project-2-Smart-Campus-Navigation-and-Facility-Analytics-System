// FacilityDatabase.h
// Purpose: Loads facilities.txt, keeps facilities in file order, and
//          provides ID search through the custom HashTable, name and
//          category search, and the facility category report.
// Author:  Emmanuel (Member A - facilities / hash table)
#ifndef FACILITYDATABASE_H
#define FACILITYDATABASE_H

#include <string>
#include <vector>
#include "Facility.h"
#include "HashTable.h"

class CampusGraph;

class FacilityDatabase {
public:
    FacilityDatabase();

    // Loads facilities from path, checking buildings against graph.
    // Prints warnings and the load summary (with hash diagnostics).
    void load(const std::string& path, const std::string& displayName,
              const CampusGraph& graph);

    // Prints every facility in file order with the total.
    void displayAll() const;

    // Handles the ID search: invalid format / not found / found.
    void searchById(const std::string& input) const;

    // Scans for facilities whose full name matches (ignoring case and
    // outer whitespace). Prints every match in file order and the count.
    void searchByName(const std::string& input) const;

    // Scans for facilities whose full category matches (ignoring case).
    void searchByCategory(const std::string& input) const;

    // For each distinct category (grouped ignoring case, first spelling
    // kept): count and percentage of all facilities, plus the total.
    void categoryReport() const;

    // True if id (already uppercase) is 'F' followed by exactly 3 digits.
    static bool isValidId(const std::string& id);

    // Prints one facility as "ID | Name | Building | Category".
    static void printFacility(const Facility& f);

private:
    std::vector<Facility> facilities;   // file order, for display
    HashTable table;                    // ID lookup
};

#endif
