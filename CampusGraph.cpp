// CampusGraph.cpp
// Purpose: Adjacency-list graph loading, display, statistics, and BFS.
// Author:  Andrew (Member B - campus graph / BFS)
#include "CampusGraph.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <queue>

// Empty graph.
CampusGraph::CampusGraph() : edgeCount(0) {}

// Linear search comparing names case-insensitively.
int CampusGraph::findBuilding(const std::string& name) const {
    std::string key = toLower(trim(name));
    for (size_t i = 0; i < names.size(); i++) {
        if (toLower(names[i]) == key) return static_cast<int>(i);
    }
    return -1;
}

// Checks a's neighbor list for b.
bool CampusGraph::hasEdge(int a, int b) const {
    for (size_t i = 0; i < adj[a].size(); i++) {
        if (adj[a][i] == b) return true;
    }
    return false;
}

// Updates both endpoints but increments the edge count once.
void CampusGraph::addEdge(int a, int b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
    edgeCount++;
}

// Reads one building name per line.
void CampusGraph::loadBuildings(const std::string& path, const std::string& displayName) {
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
            if (f.size() != 1) {
                printWarning(displayName, lineNum, "expected 1 field, found " +
                             std::to_string(f.size()));
                rejected++; continue;
            }
            if (f[0].empty()) {
                printWarning(displayName, lineNum, "empty building name");
                rejected++; continue;
            }
            if (findBuilding(f[0]) >= 0) {
                printWarning(displayName, lineNum, "duplicate building '" + f[0] + "'");
                rejected++; continue;
            }
            names.push_back(f[0]);
            adj.push_back(std::vector<int>());
            accepted++;
        }
    }
    std::cout << displayName << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

// Reads A|B pairs. Check order: field count, empty, unknown, self, duplicate.
void CampusGraph::loadConnections(const std::string& path, const std::string& displayName) {
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
            if (f.size() != 2) {
                printWarning(displayName, lineNum, "expected 2 fields, found " +
                             std::to_string(f.size()));
                rejected++; continue;
            }
            if (f[0].empty() || f[1].empty()) {
                printWarning(displayName, lineNum, "empty building name");
                rejected++; continue;
            }
            int a = findBuilding(f[0]);
            if (a < 0) {
                printWarning(displayName, lineNum, "unknown building '" + f[0] + "'");
                rejected++; continue;
            }
            int b = findBuilding(f[1]);
            if (b < 0) {
                printWarning(displayName, lineNum, "unknown building '" + f[1] + "'");
                rejected++; continue;
            }
            if (a == b) {
                printWarning(displayName, lineNum, "self-connection '" + f[0] + "'");
                rejected++; continue;
            }
            if (hasEdge(a, b)) {   // catches A|B repeated and B|A reversed
                printWarning(displayName, lineNum, "duplicate connection " +
                             names[a] + " - " + names[b]);
                rejected++; continue;
            }
            addEdge(a, b);
            accepted++;
        }
    }
    std::cout << displayName << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

// "Building: neighbor, neighbor" for every building.
void CampusGraph::display() const {
    if (names.empty()) {
        std::cout << "No buildings are loaded.\n";
        return;
    }
    for (size_t i = 0; i < names.size(); i++) {
        std::cout << names[i] << ": ";
        if (adj[i].empty()) {
            std::cout << "(no connections)";
        } else {
            for (size_t j = 0; j < adj[i].size(); j++) {
                if (j > 0) std::cout << ", ";
                std::cout << names[adj[i][j]];
            }
        }
        std::cout << "\n";
    }
}

// Totals plus degree (= neighbor list length) for each building.
void CampusGraph::displayCounts() const {
    std::cout << "Total buildings: " << names.size() << "\n";
    std::cout << "Total connections: " << edgeCount << "\n";
    if (names.empty()) {
        std::cout << "No buildings are loaded.\n";
        return;
    }
    std::cout << "Degrees:\n";
    for (size_t i = 0; i < names.size(); i++) {
        std::cout << "  " << names[i] << ": " << adj[i].size() << "\n";
    }
}

// Standard BFS. A building is marked visited when it is enqueued,
// so it can never be added to the queue twice.
void CampusGraph::bfsReachable(const std::string& startName) const {
    int start = findBuilding(startName);
    if (start < 0) {
        std::cout << "Error: unknown building '" << trim(startName) << "'\n";
        return;
    }
    std::vector<bool> visited(names.size(), false);
    std::vector<int> dist(names.size(), -1);
    std::queue<int> q;

    visited[start] = true;
    dist[start] = 0;
    q.push(start);
    int reached = 0;

    std::cout << "Reachable from " << names[start] << ":\n";
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        reached++;
        std::cout << "  " << names[cur] << " (distance " << dist[cur] << ")\n";
        for (size_t i = 0; i < adj[cur].size(); i++) {
            int next = adj[cur][i];
            if (!visited[next]) {
                visited[next] = true;
                dist[next] = dist[cur] + 1;
                q.push(next);
            }
        }
    }
    std::cout << "Reachable: " << reached << " of " << names.size() << " buildings\n";
}
