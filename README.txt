CSCE 2110 Project 2 - Smart Campus Navigation and Facility Analytics System
===========================================================================
CSCE 2110 Foundations of Data Structures
Team: Emmanuel Fakunle (Team Leader), Andrew Camargo, Jacob Beard

Purpose
-------
A C++ program that loads campus data from text files and lets the user look up
facilities by ID (custom hash table), explore the building graph (adjacency
list + BFS), and browse/add to the department hierarchy (general tree).

Build (UNT CSE CELL machines)
-----------------------------
    make                 # builds ./campus
    make clean           # removes the executable and object files

Without make:
    g++ -std=c++11 -Wall -Iinclude src/*.cpp -o campus

Run
---
    ./campus                         # uses data/ (default)
    ./campus testdata/collision      # optional data-directory argument

Run from the project root so the relative path data/ is found.
Files are loaded in this order: buildings.txt, connections.txt,
facilities.txt, departments.txt.

Directory layout
----------------
Project2/
|-- include/            Header files (Utils, Facility, HashTable,
|                       FacilityDatabase, CampusGraph, DepartmentTree)
|-- src/                Implementation files, including main.cpp
|-- data/               The four instructor starter files (unchanged)
|-- testdata/           Our own test cases:
|     collision/        IDs that collide in the hash table + a duplicate ID
|     badrecords/       Malformed/duplicate/unknown records, CRLF file
|     missingfile/      facilities.txt and departments.txt missing (empty tree)
|     rootonly/         Root-only department tree, empty facilities
|     components/       Graph with three connected components
|-- Makefile
|-- README.txt
|-- CONTRIBUTIONS.txt
|-- design_document.txt
|-- test_results.txt
|-- .gitignore

CSCE 2110 Project 2 - Design Document (Milestone 1)
Hash function
Key: the facility ID after trimming and converting to uppercase (e.g. "F004"). Formula (polynomial rolling hash, base 31):

h = 0
for each character c in key:
    h = (h * 31 + ASCII(c)) mod capacity
index = h
Taking mod capacity at every step keeps h small, so it never overflows. Because the key is uppercased first, "f004" and "F004" always produce the same index. std::hash is not used.

Collision strategy: separate chaining
Each bucket is a chain (std::vector). A new entry is appended to the chain at its index. We chose chaining because:

chains grow, so there is no fixed maximum number of facilities (HASH-5);
the table can never become "full", so no full-table rejection is needed;
insert, lookup, and duplicate detection are simple: scan one short chain. Lookup only searches the single bucket the key hashes to; it never scans all facilities.
Duplicate keys: insert() scans the chain first. If the ID already exists it returns false, the original entry is unchanged, and no collision is counted.

Collision counting
A collision = one accepted insertion into a bucket that already holds at least one entry. Rejected records (including duplicate IDs) never add to the count. Example (testdata/collision, capacity 11): F001, F013, F025 all hash to bucket 3, so F013 and F025 each count once -> collisions 2. The duplicate f013 is rejected and the count stays 2.

Capacity and full-table behavior
Initial capacity: 11 buckets (prime), set by the constant DEFAULT_HASH_CAPACITY in HashTable.h (also a constructor parameter). With chaining the table never fills; any number of facilities can be stored. Resizing is not implemented (optional).

Graph representation: adjacency list
CampusGraph stores:

names: vector, vertex i = i-th accepted building (file order);
adj: vector<vector>, adj[i] = neighbor indices in insertion order;
edgeCount: each undirected edge counted once. Adding edge A-B appends B to adj[A] and A to adj[B], and increments edgeCount once. Duplicate and reversed connections are caught by checking whether B is already in adj[A]. We chose an adjacency list because the campus graph is sparse, it grows naturally as buildings are added, degree = adj[i].size(), and neighbor insertion order (which determines BFS order) is kept automatically. Neighbors are never sorted. The same graph object is used for display, statistics, and BFS.
BFS uses std::queue and a vector visited. A building is marked visited when it is enqueued, and dist[next] = dist[current] + 1.

Department tree
General tree: DepartmentNode {name, parent pointer, vector<DepartmentNode*> children}. Children are kept in insertion order. Search is a recursive depth-first scan (case-insensitive). Depth = number of parent links to the root. Hierarchy display and preorder are recursive (node, then each child). The destructor frees all nodes (postorder delete).

Main classes / modules
Utils trim, case conversion, '|' splitting, comment check, warnings Facility struct: id, name, building, category HashTable custom separate-chaining table keyed by facility ID FacilityDatabase loads facilities.txt, file-order list + HashTable, ID search CampusGraph loads buildings/connections, display, counts/degrees, BFS DepartmentTree loads departments.txt, add, search, hierarchy, preorder main.cpp reads the data-directory argument, loads files, runs the menu

===== Campus Navigator (Milestone 1) =====

View all facilities
1. Search facility by ID
2. View campus graph
3. Display graph counts and degrees
4. Find reachable buildings (BFS)
5. Add department
6. Search department
7. Display department hierarchy
8. Run preorder traversal
9. Exit Enter choice: