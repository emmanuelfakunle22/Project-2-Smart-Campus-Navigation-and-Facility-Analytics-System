// CampusGraph.h
// Purpose: Undirected, unweighted campus graph stored as an adjacency list,
//          with BFS reachability, shortest path, and connectivity report.
// Author:  Andrew (Member B - campus graph / BFS)
#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <string>
#include <vector>

class CampusGraph {
public:
    CampusGraph();

    // Loads buildings.txt (one name per line, unique ignoring case).
    void loadBuildings(const std::string& path, const std::string& displayName);

    // Loads connections.txt (A|B). Both buildings must exist; rejects
    // self-connections and duplicates in either direction.
    void loadConnections(const std::string& path, const std::string& displayName);

    // Returns the index of a building (case-insensitive, trimmed), or -1.
    int findBuilding(const std::string& name) const;

    // Returns the stored spelling of building i.
    const std::string& getBuildingName(int i) const { return names[i]; }

    int buildingCount() const { return static_cast<int>(names.size()); }
    int connectionCount() const { return edgeCount; }

    // Prints each building and its neighbors in insertion order.
    void display() const;

    // Prints total buildings, total connections, and each degree.
    void displayCounts() const;

    // BFS from the named building, printing visit order and distances.
    void bfsReachable(const std::string& startName) const;

    // BFS with parent tracking; prints a path with the fewest connections.
    // Ties are broken by neighbor insertion order.
    void shortestPath(const std::string& startName, const std::string& destName) const;

    // Totals, buildings tied for highest degree, isolated buildings,
    // and number of connected components.
    void connectivityReport() const;

private:
    // Runs BFS from start, marking every reached vertex in visited.
    void markComponent(int start, std::vector<bool>& visited) const;

    // Adds an undirected edge (both directions), counted once.
    void addEdge(int a, int b);

    // True if a and b are already neighbors.
    bool hasEdge(int a, int b) const;

    std::vector<std::string> names;          // vertex i's name, file order
    std::vector<std::vector<int> > adj;      // adj[i] = neighbor indices, insertion order
    int edgeCount;                           // each undirected edge counted once
};

#endif
