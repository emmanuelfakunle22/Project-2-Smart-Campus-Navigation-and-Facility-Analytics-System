// HashTable.cpp
// Purpose: Separate-chaining hash table implementation for facility IDs.
// Author:  Emmanuel (Member A - facilities / hash table)
#include "HashTable.h"
#include "Utils.h"

// Sets up `capacity` empty buckets (at least 1).
HashTable::HashTable(int capacity)
    : buckets(capacity > 0 ? capacity : 1), size(0), collisions(0) {}

// h = 0; for each character c: h = (h * 31 + c) mod capacity.
// Taking the mod each step keeps h small, so it never overflows.
int HashTable::hash(const std::string& key) const {
    std::string k = toUpper(key);
    unsigned long h = 0;
    unsigned long cap = static_cast<unsigned long>(buckets.size());
    for (size_t i = 0; i < k.size(); i++) {
        h = (h * 31 + static_cast<unsigned char>(k[i])) % cap;
    }
    return static_cast<int>(h);
}

// Inserts into the chain at hash(id). Duplicate keys are rejected first,
// so rejected records never add to the collision count.
bool HashTable::insert(const Facility& f) {
    Facility copy = f;
    copy.id = toUpper(trim(f.id));
    int index = hash(copy.id);
    std::vector<Facility>& chain = buckets[index];

    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i].id == copy.id) return false;   // duplicate key
    }
    if (!chain.empty()) collisions++;               // bucket already occupied
    chain.push_back(copy);
    size++;
    return true;
}

// Only searches the one bucket the key hashes to (no full scan).
const Facility* HashTable::find(const std::string& key) const {
    std::string k = toUpper(trim(key));
    const std::vector<Facility>& chain = buckets[hash(k)];
    for (size_t i = 0; i < chain.size(); i++) {
        if (chain[i].id == k) return &chain[i];
    }
    return nullptr;
}

// Convenience wrapper around find().
bool HashTable::contains(const std::string& key) const {
    return find(key) != nullptr;
}
