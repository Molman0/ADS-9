// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_
#include <vector>

struct Node {
    char value;
    std::vector<Node*> children;
    int64_t subtree_perm_count;
    explicit Node(char v) : value(v), subtree_perm_count(0) {}
    ~Node();
};

class PMTree {
 private:
    Node* root;
    int n;
    Node* createNode(char value, const std::vector<char>& remaining);
 public:
    explicit PMTree(const std::vector<char>& in);
    ~PMTree();
    Node* getRoot() const { return root; }
    int getN() const { return n; }
    int64_t getTotalPerms() const {}
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(const PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
