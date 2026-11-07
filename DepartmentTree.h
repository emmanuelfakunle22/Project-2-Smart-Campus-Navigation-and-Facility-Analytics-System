// DepartmentTree.h
// Purpose: General tree (any number of children) for the department hierarchy.
// Author:  Member C (department tree)
#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>

// One department. Children are kept in insertion order.
struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;
    std::vector<DepartmentNode*> children;

    explicit DepartmentNode(const std::string& n, DepartmentNode* p = nullptr)
        : name(n), parent(p) {}
};

class DepartmentTree {
public:
    DepartmentTree();
    ~DepartmentTree();   // frees every node

    // Loads departments.txt. First valid line must be "Name|NONE" (root).
    void load(const std::string& path, const std::string& displayName);

    bool isEmpty() const { return root == nullptr; }

    // Finds a department by name, ignoring case. Returns nullptr if missing.
    DepartmentNode* find(const std::string& name) const;

    // Creates the root. Returns false if a root already exists.
    bool addRoot(const std::string& name);

    // Adds name under parentName. Returns "" on success or an error message.
    std::string addChild(const std::string& name, const std::string& parentName);

    // Prints name, parent, direct-child count, and depth.
    void search(const std::string& name) const;

    // Preorder with 4 spaces of indentation per depth level.
    void displayHierarchy() const;

    // Preorder sequence without indentation.
    void displayPreorder() const;

private:
    // Recursive helpers.
    DepartmentNode* findRec(DepartmentNode* node, const std::string& lowerName) const;
    void destroy(DepartmentNode* node);
    void hierarchyRec(const DepartmentNode* node, int depth) const;
    void preorderRec(const DepartmentNode* node, std::vector<std::string>& out) const;

    // Number of edges from the root to node.
    static int depthOf(const DepartmentNode* node);

    DepartmentNode* root;

    // No copying (would double-free nodes).
    DepartmentTree(const DepartmentTree&);
    DepartmentTree& operator=(const DepartmentTree&);
};

#endif
