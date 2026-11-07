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
