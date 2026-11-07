// HashTable.h
// Purpose: Custom hash table keyed by facility ID, using separate chaining.
// Author:  Emmanuel (Member A - facilities / hash table)
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include "Facility.h"

// Initial number of buckets. Chains grow, so there is no maximum entry count.
const int DEFAULT_HASH_CAPACITY = 11;

class HashTable {
public:
    // Creates a table with the given number of buckets.
    explicit HashTable(int capacity = DEFAULT_HASH_CAPACITY);

    // Hash function: polynomial rolling hash of the uppercase key, mod capacity.
    int hash(const std::string& key) const;

    // Inserts f (key = uppercase f.id). Returns false if the key already
    // exists; the existing entry is not changed and no collision is counted.
    bool insert(const Facility& f);

    // Looks up a key (normalized to uppercase first).
    // Returns a pointer to the stored facility, or nullptr if not found.
    const Facility* find(const std::string& key) const;

    // True if the key is already in the table.
    bool contains(const std::string& key) const;

    int getCapacity() const { return static_cast<int>(buckets.size()); }
    int getSize() const { return size; }
    int getCollisions() const { return collisions; }

private:
    // Each bucket is a chain of facilities whose IDs hash to that index.
    std::vector<std::vector<Facility> > buckets;
    int size;        // number of stored entries
    int collisions;  // accepted insertions that landed in a nonempty bucket
};

#endif
