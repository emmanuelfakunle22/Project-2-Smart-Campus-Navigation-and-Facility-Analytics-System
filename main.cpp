// main.cpp
// Purpose: Program entry point. Loads the four data files in order and runs
//          the Milestone 1 menu. Data-structure logic lives in the classes.
// Author:  Emmanuel, Andrew, Jacoby (shared work)
#include <iostream>
#include <string>
#include "Utils.h"
#include "CampusGraph.h"
#include "FacilityDatabase.h"
#include "DepartmentTree.h"

// Prints the Milestone 1 menu (numbers must match Section 6.9).
static void printMenu() {
    std::cout << "\n===== Campus Navigator (Milestone 1) =====\n"
              << "1. View all facilities\n"
              << "2. Search facility by ID\n"
              << "3. View campus graph\n"
              << "4. Display graph counts and degrees\n"
              << "5. Find reachable buildings (BFS)\n"
              << "6. Add department\n"
              << "7. Search department\n"
              << "8. Display department hierarchy\n"
              << "9. Run preorder traversal\n"
              << "0. Exit\n"
              << "Enter choice: ";
}

// Reads a whole line from cin. Returns false at end of input.
static bool readLine(std::string& out) {
    if (!std::getline(std::cin, out)) return false;
    return true;
}

// Reads a menu choice. Returns -1 for invalid input, -2 at end of input.
static int readChoice(int maxOption) {
    std::string line;
    if (!readLine(line)) return -2;
    line = trim(line);
    if (line.empty() || line.size() > 3) return -1;
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] < '0' || line[i] > '9') return -1;
    }
    int n = std::stoi(line);
    if (n < 0 || n > maxOption) return -1;
    return n;
}

// Prompts for a name (spaces allowed). Returns false if blank or end of input.
static bool promptName(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    if (!readLine(out)) return false;
    out = trim(out);
    if (out.empty()) {
        std::cout << "Error: name cannot be blank.\n";
        return false;
    }
    return true;
}

// Menu option 6: root without a parent prompt when the tree is empty.
static void addDepartment(DepartmentTree& tree) {
    std::string name;
    if (!promptName("Department name: ", name)) return;
    if (tree.isEmpty()) {
        tree.addRoot(name);
        std::cout << "Added '" << name << "' as the root department.\n";
        return;
    }
    if (tree.find(name) != nullptr) {
        std::cout << "Error: duplicate department '" << name << "'.\n";
        return;
    }
    std::string parent;
    if (!promptName("Parent department: ", parent)) return;
    std::string err = tree.addChild(name, parent);
    if (err.empty()) std::cout << "Added '" << name << "' under '"
                               << tree.find(parent)->name << "'.\n";
    else std::cout << "Error: " << err << ". Tree unchanged.\n";
}

int main(int argc, char* argv[]) {
    // Optional data directory argument (default data/).
    std::string dataDir = (argc > 1) ? argv[1] : "data";

    CampusGraph graph;
    FacilityDatabase facilities;
    DepartmentTree departments;

    // Required load order: buildings, connections, facilities, departments.
    std::cout << "Loading data from " << dataDir << "/\n";
    graph.loadBuildings(joinPath(dataDir, "buildings.txt"), "buildings.txt");
    graph.loadConnections(joinPath(dataDir, "connections.txt"), "connections.txt");
    facilities.load(joinPath(dataDir, "facilities.txt"), "facilities.txt", graph);
    departments.load(joinPath(dataDir, "departments.txt"), "departments.txt");

    while (true) {
        printMenu();
        int choice = readChoice(9);
        if (choice == -2) { std::cout << "\nEnd of input. Goodbye.\n"; break; }
        if (choice == -1) {
            std::cout << "Invalid choice. Enter a number from 0 to 9.\n";
            continue;
        }
        if (choice == 0) { std::cout << "Goodbye.\n"; break; }

        std::string input;
        switch (choice) {
            case 1: facilities.displayAll(); break;
            case 2:
                if (promptName("Facility ID: ", input)) facilities.searchById(input);
                break;
            case 3: graph.display(); break;
            case 4: graph.displayCounts(); break;
            case 5:
                if (promptName("Starting building: ", input)) graph.bfsReachable(input);
                break;
            case 6: addDepartment(departments); break;
            case 7:
                if (promptName("Department name: ", input)) departments.search(input);
                break;
            case 8: departments.displayHierarchy(); break;
            case 9: departments.displayPreorder(); break;
        }
    }
    return 0;
}
