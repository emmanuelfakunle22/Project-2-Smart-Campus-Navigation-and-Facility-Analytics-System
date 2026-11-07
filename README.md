# Project-2-Smart-Campus-Navigation-and-Facility-Analytics-System
This project is from class CSCE 2110 Foundations of Data Structures. The team members are Emmanuel Fakunle (Team Leader), Andrew Camargo, &amp; Jacob Beard.
CSCE 2110 Project 2 - Campus Navigator (Milestone 1)
=====================================================

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

CSCE 2110 Project 2 - Design Document (Milestone 1)
===================================================

1. Hash function
----------------
Key: the facility ID after trimming and converting to uppercase (e.g. "F004").
Formula (polynomial rolling hash, base 31):

    h = 0
    for each character c in key:
        h = (h * 31 + ASCII(c)) mod capacity
    index = h

Taking mod capacity at every step keeps h small, so it never overflows. Because
the key is uppercased first, "f004" and "F004" always produce the same index.
std::hash is not used.

2. Collision strategy: separate chaining
----------------------------------------
Each bucket is a chain (std::vector<Facility>). A new entry is appended to the
chain at its index. We chose chaining because:
  - chains grow, so there is no fixed maximum number of facilities (HASH-5);
  - the table can never become "full", so no full-table rejection is needed;
  - insert, lookup, and duplicate detection are simple: scan one short chain.
Lookup only searches the single bucket the key hashes to; it never scans
all facilities.

Duplicate keys: insert() scans the chain first. If the ID already exists it
returns false, the original entry is unchanged, and no collision is counted.

3. Collision counting
---------------------
A collision = one accepted insertion into a bucket that already holds at
least one entry. Rejected records (including duplicate IDs) never add to the
count. Example (testdata/collision, capacity 11): F001, F013, F025 all hash to
bucket 3, so F013 and F025 each count once -> collisions 2. The duplicate f013
is rejected and the count stays 2.

4. Capacity and full-table behavior
-----------------------------------
Initial capacity: 11 buckets (prime), set by the constant
DEFAULT_HASH_CAPACITY in HashTable.h (also a constructor parameter).
With chaining the table never fills; any number of facilities can be stored.
Resizing is not implemented (optional).

5. Graph representation: adjacency list
---------------------------------------
CampusGraph stores:
  - names: vector<string>, vertex i = i-th accepted building (file order);
  - adj:   vector<vector<int>>, adj[i] = neighbor indices in insertion order;
  - edgeCount: each undirected edge counted once.
Adding edge A-B appends B to adj[A] and A to adj[B], and increments edgeCount
once. Duplicate and reversed connections are caught by checking whether B is
already in adj[A]. We chose an adjacency list because the campus graph is
sparse, it grows naturally as buildings are added, degree = adj[i].size(), and
neighbor insertion order (which determines BFS order) is kept automatically.
Neighbors are never sorted. The same graph object is used for display,
statistics, and BFS.

BFS uses std::queue<int> and a vector<bool> visited. A building is marked
visited when it is enqueued, and dist[next] = dist[current] + 1.

6. Department tree
------------------
General tree: DepartmentNode {name, parent pointer, vector<DepartmentNode*>
children}. Children are kept in insertion order. Search is a recursive
depth-first scan (case-insensitive). Depth = number of parent links to the
root. Hierarchy display and preorder are recursive (node, then each child).
The destructor frees all nodes (postorder delete).

7. Main classes / modules
-------------------------
Utils            trim, case conversion, '|' splitting, comment check, warnings
Facility         struct: id, name, building, category
HashTable        custom separate-chaining table keyed by facility ID
FacilityDatabase loads facilities.txt, file-order list + HashTable, ID search
CampusGraph      loads buildings/connections, display, counts/degrees, BFS
DepartmentTree   loads departments.txt, add, search, hierarchy, preorder
main.cpp         reads the data-directory argument, loads files, runs the menu


===== Campus Navigator (Milestone 1) =====
1. View all facilities
2. Search facility by ID
3. View campus graph
4. Display graph counts and degrees
5. Find reachable buildings (BFS)
6. Add department
7. Search department
8. Display department hierarchy
9. Run preorder traversal
0. Exit
Enter choice:
