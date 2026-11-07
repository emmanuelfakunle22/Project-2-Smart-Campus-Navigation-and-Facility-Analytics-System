// FacilityDatabase.h
// Purpose: Loads facilities.txt, keeps facilities in file order, and
//          provides ID search through the custom HashTable.
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

    // True if id (already uppercase) is 'F' followed by exactly 3 digits.
    static bool isValidId(const std::string& id);

    // Prints one facility as "ID | Name | Building | Category".
    static void printFacility(const Facility& f);

private:
    std::vector<Facility> facilities;   // file order, for display
    HashTable table;                    // ID lookup
};

#endif
