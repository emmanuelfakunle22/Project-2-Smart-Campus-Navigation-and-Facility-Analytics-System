// DepartmentTree.cpp
// Purpose: General-tree loading, add, search, hierarchy display, and
//          preorder, postorder, and level-order traversals; statistics.
// Author:  Jacob (Member C - department tree)
#include "DepartmentTree.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <queue>

// Empty tree.
DepartmentTree::DepartmentTree() : root(nullptr) {}

// Releases all nodes.
DepartmentTree::~DepartmentTree() {
    destroy(root);
}

// Postorder delete: children first, then the node.
void DepartmentTree::destroy(DepartmentNode* node) {
    if (node == nullptr) return;
    for (size_t i = 0; i < node->children.size(); i++) destroy(node->children[i]);
    delete node;
}

// Depth-first search for a name (lowerName is already lowercase).
DepartmentNode* DepartmentTree::findRec(DepartmentNode* node,
                                        const std::string& lowerName) const {
    if (node == nullptr) return nullptr;
    if (toLower(node->name) == lowerName) return node;
    for (size_t i = 0; i < node->children.size(); i++) {
        DepartmentNode* hit = findRec(node->children[i], lowerName);
        if (hit != nullptr) return hit;
    }
    return nullptr;
}

// Public find: trims and lowercases the name first.
DepartmentNode* DepartmentTree::find(const std::string& name) const {
    return findRec(root, toLower(trim(name)));
}

// Counts parent links up to the root.
int DepartmentTree::depthOf(const DepartmentNode* node) {
    int d = 0;
    while (node != nullptr && node->parent != nullptr) {
        d++;
        node = node->parent;
    }
    return d;
}

// Creates the root if the tree is empty.
bool DepartmentTree::addRoot(const std::string& name) {
    if (root != nullptr) return false;
    root = new DepartmentNode(trim(name));
    return true;
}

// Adds a child at the end of the parent's children (insertion order).
std::string DepartmentTree::addChild(const std::string& name, const std::string& parentName) {
    if (find(name) != nullptr) return "duplicate department '" + trim(name) + "'";
    DepartmentNode* parent = find(parentName);
    if (parent == nullptr) return "unknown parent '" + trim(parentName) + "'";
    parent->children.push_back(new DepartmentNode(trim(name), parent));
    return "";
}

// Reads Name|Parent lines. The first valid record must be Name|NONE.
void DepartmentTree::load(const std::string& path, const std::string& displayName) {
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
                printWarning(displayName, lineNum, "empty required field");
                rejected++; continue;
            }
            bool parentIsNone = equalsIgnoreCase(f[1], "NONE");

            if (root == nullptr) {
                if (!parentIsNone) {
                    printWarning(displayName, lineNum, "first department must have parent NONE (unknown parent '" + f[1] + "')");
                    rejected++; continue;
                }
                addRoot(f[0]);
                accepted++;
                continue;
            }
            if (parentIsNone) {
                printWarning(displayName, lineNum, "root already exists; '" + f[0] + "' cannot use parent NONE");
                rejected++; continue;
            }
            std::string err = addChild(f[0], f[1]);
            if (!err.empty()) {
                printWarning(displayName, lineNum, err);
                rejected++; continue;
            }
            accepted++;
        }
    }
    std::cout << displayName << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}

// Prints details for one department.
void DepartmentTree::search(const std::string& name) const {
    const DepartmentNode* node = find(name);
    if (node == nullptr) {
        std::cout << "Department '" << trim(name) << "' not found.\n";
        return;
    }
    std::cout << "Name: " << node->name << "\n";
    std::cout << "Parent: " << (node->parent ? node->parent->name : "None") << "\n";
    std::cout << "Direct children: " << node->children.size() << "\n";
    std::cout << "Depth: " << depthOf(node) << "\n";
}

// Recursive preorder print with indentation.
void DepartmentTree::hierarchyRec(const DepartmentNode* node, int depth) const {
    std::cout << std::string(depth * 4, ' ') << node->name << "\n";
    for (size_t i = 0; i < node->children.size(); i++)
        hierarchyRec(node->children[i], depth + 1);
}

// Indented hierarchy.
void DepartmentTree::displayHierarchy() const {
    if (root == nullptr) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    hierarchyRec(root, 0);
}

// Recursive preorder: visit node, then each child left to right.
void DepartmentTree::preorderRec(const DepartmentNode* node,
                                 std::vector<std::string>& out) const {
    out.push_back(node->name);
    for (size_t i = 0; i < node->children.size(); i++)
        preorderRec(node->children[i], out);
}

// Preorder as a plain sequence.
void DepartmentTree::displayPreorder() const {
    if (root == nullptr) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    std::vector<std::string> order;
    preorderRec(root, order);
    std::cout << "Preorder:\n";
    for (size_t i = 0; i < order.size(); i++) std::cout << order[i] << "\n";
}

// Recursive postorder: each child (left to right), then the node.
void DepartmentTree::postorderRec(const DepartmentNode* node,
                                  std::vector<std::string>& out) const {
    for (size_t i = 0; i < node->children.size(); i++)
        postorderRec(node->children[i], out);
    out.push_back(node->name);
}

// Postorder as a plain sequence.
void DepartmentTree::displayPostorder() const {
    if (root == nullptr) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    std::vector<std::string> order;
    postorderRec(root, order);
    std::cout << "Postorder:\n";
    for (size_t i = 0; i < order.size(); i++) std::cout << order[i] << "\n";
}

// Level-order: dequeue a node, print it, enqueue its children in order.
void DepartmentTree::displayLevelOrder() const {
    if (root == nullptr) {
        std::cout << "No departments are loaded.\n";
        return;
    }
    std::queue<const DepartmentNode*> q;
    q.push(root);
    std::cout << "Level-order:\n";
    while (!q.empty()) {
        const DepartmentNode* cur = q.front();
        q.pop();
        std::cout << cur->name << "\n";
        for (size_t i = 0; i < cur->children.size(); i++) q.push(cur->children[i]);
    }
}

// Visits every node once: counts it, counts it as a leaf if it has no
// children, and keeps the largest depth seen as the height.
void DepartmentTree::statsRec(const DepartmentNode* node, int depth,
                              int& count, int& leaves, int& height) const {
    count++;
    if (node->children.empty()) leaves++;
    if (depth > height) height = depth;
    for (size_t i = 0; i < node->children.size(); i++)
        statsRec(node->children[i], depth + 1, count, leaves, height);
}

// Prints statistics; an empty tree shows 0, 0, N/A.
void DepartmentTree::displayStatistics() const {
    if (root == nullptr) {
        std::cout << "No departments are loaded.\n";
        std::cout << "Total departments: 0\n";
        std::cout << "Leaf departments: 0\n";
        std::cout << "Tree height: N/A\n";
        return;
    }
    int count = 0, leaves = 0, height = 0;
    statsRec(root, 0, count, leaves, height);
    std::cout << "Total departments: " << count << "\n";
    std::cout << "Leaf departments: " << leaves << "\n";
    std::cout << "Tree height: " << height << "\n";
}
