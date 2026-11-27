CSCE 2110 Project 2 - Smart Campus Navigation and Facility Analytics System
===========================================================================
CSCE 2110 Foundations of Data Structures
Team: Emmanuel Fakunle (Team Leader), Andrew Camargo, Jacob Beard

Purpose
-------
A C++ program that loads campus data from text files and lets the user:
  - look up facilities by ID (custom hash table), by name, or by category;
  - explore the building graph: reachability and shortest paths (BFS);
  - browse and add to the academic department hierarchy (general tree);
  - view a facility category report and a graph connectivity report.

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

Final menu
----------
 1. View all facilities              10. Search department
 2. Search facility by ID            11. Display department hierarchy
 3. Search facility by name          12. Preorder traversal
 4. Search facility by category      13. Postorder traversal
 5. View campus graph                14. Level-order traversal
 6. Display graph counts and degrees 15. Department statistics
 7. Find reachable buildings (BFS)   16. Facility category report
 8. Find shortest path (BFS)         17. Graph connectivity report
 9. Add department                    0. Exit

Module overview
---------------
Utils             String helpers: trim, case conversion, '|' splitting,
                  comment detection, warning output, path joining.
Facility          Struct holding ID (uppercase), name, building, category.
HashTable         Custom hash table keyed by facility ID (separate chaining).
FacilityDatabase  Loads facilities.txt; file-order list + HashTable; ID,
                  name, and category search; category report.
CampusGraph       Loads buildings.txt and connections.txt into an adjacency
                  list; display, degrees, BFS reachability, shortest path,
                  connectivity report.
DepartmentTree    General tree (DepartmentNode with a vector of children);
                  load, add, search, hierarchy, preorder, postorder,
                  level-order, statistics.
main.cpp          Reads the data-directory argument, loads the files in
                  order, and runs the menu.

Hash function
-------------
Polynomial rolling hash of the uppercase ID, base 31:
    h = 0;  for each character c:  h = (h * 31 + c) mod capacity
Capacity is 11 buckets (DEFAULT_HASH_CAPACITY in HashTable.h).

Collision strategy
------------------
Separate chaining: each bucket is a list that grows, so there is no maximum
number of facilities and the table never becomes full. A collision is
counted when an accepted ID is inserted into a nonempty bucket.

Graph representation
--------------------
Adjacency list: vector of building names plus vector<vector<int>> of
neighbor indices in insertion order. Each undirected connection is stored
at both ends and counted once.

Directory layout
----------------
Project2/
 |-- include/            Header files
 |-- src/                Implementation files, including main.cpp
 |-- data/               The four instructor starter files (unchanged)
 |-- testdata/           Our own test cases:
 |     collision/        IDs that collide in the hash table + a duplicate ID
 |     badrecords/       Malformed, duplicate, unknown and empty-field records;
 |                       Windows line endings; lowercase ID; '#' in a name
 |     missingfile/      facilities.txt and departments.txt missing (empty tree)
 |     rootonly/         Root-only department tree, empty facilities
 |     components/       Graph with three connected components
 |     tie/              Two equally short routes; four-way degree tie
 |     categories/       Categories that differ only by capitalization
 |     emptyfiles/       All four files empty
 |-- Makefile
 |-- README.txt
 |-- CONTRIBUTIONS.txt
 |-- CODE_REVIEW.txt
 |-- design_document.txt
 |-- test_results.txt
 |-- .gitignore
