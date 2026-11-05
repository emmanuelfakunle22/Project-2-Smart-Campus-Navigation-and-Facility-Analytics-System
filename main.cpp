#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Utils.h"
#include <queue>
#include <limits>

using namespace std;

// Print Menu Function
int printMenu() {
    int choice;

    cout << "\n===== Smart Campus Navigation System =====" << endl;
    cout << " 1. View all facilities" << endl;
    cout << " 2. Search facility by ID" << endl;
    cout << " 3. View campus graph" << endl;
    cout << " 4. Display graph counts and degrees" << endl;
    cout << " 5. Find reachable buildings (BFS)" << endl;
    cout << " 6. Add department" << endl;
    cout << " 7. Search department" << endl;
    cout << " 8. Display department hierarchy" << endl;
    cout << " 9. Run preorder traversal" << endl;
    cout << " 0. Exit" << endl;
    cout << "Enter your choice: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }

    return choice;
}

vector<string> loadBuildings(const string& dataDir) {

    string filename = dataDir + "/buildings.txt";

    ifstream in(filename);
    string line;

    vector<string> buildings;

    int lineNo = 0;
    int accepted = 0;
    int rejected = 0;

    if (!in) {
        cout << "Error: could not open file " << filename << endl;
        cout << "buildings.txt: 0 accepted, 0 rejected" << endl;
        return buildings;
    }

    while (getline(in, line)) {

        lineNo++;

        line = trim(line);

        if (line.empty()) {
            continue;
        }

        if (line[0] == '#') {
            continue;
        }

        bool isDuplicate = false;

        for (size_t i = 0; i < buildings.size(); i++) {

            if (toUpper(buildings[i]) == toUpper(line)) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": duplicate building \"" << line << "\"" << endl;

            rejected++;
            continue;
        }

        buildings.push_back(line);
        accepted++;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected" << endl;

    return buildings;
}

int findBuildingIndex(const string& name, const vector<string>& buildings) {

    for (size_t i = 0; i < buildings.size(); i++) {

        if (toUpper(buildings[i]) == toUpper(name)) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// Load Connections
void loadConnections(const string& dataDir,
                     const vector<string>& buildings,
                     vector<vector<int>>& adj) {

    string filename = dataDir + "/connections.txt";

    ifstream in(filename);
    string line;

    adj.clear();
    adj.resize(buildings.size());

    int lineNo = 0;
    int accepted = 0;
    int rejected = 0;

    if (!in) {
        cout << "Error: could not open file " << filename << endl;
        cout << filename << ": 0 accepted, 0 rejected" << endl;
        return;
    }

    while (getline(in, line)) {

        lineNo++;

        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        vector<string> parts = split(line, '|');

        if (parts.size() != 2) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": incorrect number of fields" << endl;
            rejected++;
            continue;
        }

        string building1 = trim(parts[0]);
        string building2 = trim(parts[1]);

        if (building1.empty() || building2.empty()) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": missing building name" << endl;
            rejected++;
            continue;
        }

        int index1 = findBuildingIndex(building1, buildings);
        int index2 = findBuildingIndex(building2, buildings);

        if (index1 == -1) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": unknown building \"" << building1 << "\"" << endl;
            rejected++;
            continue;
        }

        if (index2 == -1) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": unknown building \"" << building2 << "\"" << endl;
            rejected++;
            continue;
        }

        if (index1 == index2) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": self-connection" << endl;
            rejected++;
            continue;
        }

        bool isDuplicate = false;

        for (size_t i = 0; i < adj[index1].size(); i++) {
            if (adj[index1][i] == index2) {
                isDuplicate = true;
                break;
            }
        }

        if (isDuplicate) {
            cout << "Warning: " << filename << " line " << lineNo
                 << ": duplicate connection" << endl;
            rejected++;
            continue;
        }

        adj[index1].push_back(index2);
        adj[index2].push_back(index1);

        accepted++;
    }

    cout << filename << ": " << accepted
         << " accepted, " << rejected << " rejected" << endl;
}

void printGraph(const vector<string>& buildings, const vector<vector<int>>& adj) {

    if (buildings.empty()) {
        cout << "No buildings available to display." << endl;
        return;
    }

    for (size_t i = 0; i < buildings.size(); i++) {

        cout << buildings[i] << ": ";

        if (adj[i].empty()) {
            cout << "(no connections)";
        }
        else {
            for (size_t k = 0; k < adj[i].size(); k++) {

                if (k > 0) {
                    cout << ", ";
                }

                cout << buildings[adj[i][k]];
            }
        }

        cout << endl;
    }
}

void printCounts(const vector<string>& buildings, const vector<vector<int>>& adj) {

    if (buildings.empty()) {
        cout << "Total buildings: 0" << endl;
        cout << "Total connections: 0" << endl;
        return;
    }

    int totalDegrees = 0;

    cout << "Total buildings: " << buildings.size() << endl;

    for (size_t i = 0; i < buildings.size(); i++) {

        int degree = adj[i].size();

        cout << buildings[i] << ": degree " << degree << endl;

        totalDegrees += degree;
    }

    int totalConnections = totalDegrees / 2;

    cout << "Total connections: " << totalConnections << endl;
}

void findReachableBuildings(const vector<string>& buildings,
                            const vector<vector<int>>& adj) {

    if (buildings.empty()) {
        cout << "No buildings available." << endl;
        return;
    }

    string startName;

    cout << "Enter starting building: ";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, startName);

    startName = trim(startName);

    if (startName.empty()) {
        cout << "Error: starting building cannot be empty." << endl;
        return;
    }

    int startIndex = findBuildingIndex(startName, buildings);

    if (startIndex == -1) {
        cout << "Error: building \"" << startName << "\" not found." << endl;
        return;
    }

    vector<bool> visited(buildings.size(), false);
    vector<int> distance(buildings.size(), -1);
    queue<int> q;

    visited[startIndex] = true;
    distance[startIndex] = 0;
    q.push(startIndex);

    int reachableCount = 0;

    cout << "\nReachable buildings from "
         << buildings[startIndex] << ":" << endl;

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        cout << buildings[current] << " ("
             << distance[current] << ")" << endl;

        reachableCount++;

        for (size_t i = 0; i < adj[current].size(); i++) {

            int neighbor = adj[current][i];

            if (!visited[neighbor]) {

                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;

                q.push(neighbor);
            }
        }
    }

    cout << "Reachable: " << reachableCount
         << " of " << buildings.size() << " buildings" << endl;
}

int main() {
    
vector<string> buildings = loadBuildings("data");
vector<vector<int>> adj(buildings.size());
loadConnections("data", buildings, adj);   
int usrChoice;

// Print Menu, then ask the user for their choice
    do {
        usrChoice = printMenu();

        switch (usrChoice) {
            case 1:
                cout << "Not implemented yet." << endl;
                break;

            case 2:
                cout << "Not implemented yet." << endl;
                break;

            case 3:
                printGraph(buildings, adj);
                break;

            case 4:
                printCounts(buildings, adj);
                break;

            case 5:
                findReachableBuildings(buildings, adj);
                break;

            case 6:
                cout << "Not implemented yet." << endl;
                break;

            case 7:
                cout << "Not implemented yet." << endl;
                break;

            case 8:
                cout << "Not implemented yet." << endl;
                break;

            case 9:
                cout << "Not implemented yet." << endl;
                break;

            case 0:
                cout << "Goodbye." << endl;
                break;

            default:
                cout << "Please enter a valid choice." << endl;
                break;
        }

    } while (usrChoice != 0);

    return 0;
}
