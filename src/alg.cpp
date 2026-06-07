// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "tree.h"

Node::~Node() {
    for (Node* child : children) delete child;
}

int64_t PMTree::getTotalPerms() const {
    return root ? root->subtree_perm_count : 0; 
}

Node* PMTree::createNode(char value, const std::vector<char>& remaining) {
    Node* node = new Node(value);
    if (remaining.empty()) {
        node->subtree_perm_count = 1;
        return node;
    }
    for (char c : remaining) {
        std::vector<char> new_remaining;
        new_remaining.reserve(remaining.size() - 1);
        for (char ch : remaining) {
            if (ch != c) new_remaining.push_back(ch);
        }
        Node* child = createNode(c, new_remaining);
        node->children.push_back(child);
    }
    node->subtree_perm_count = node->children.size() * node->children[0]->subtree_perm_count;
    return node;
}

PMTree::PMTree(const std::vector<char>& in) : root(nullptr), n(in.size()) {
    if (n == 0) {
        root = new Node('\0');
        root->subtree_perm_count = 0;
        return;
    }
    std::vector<char> sorted = in;
    std::sort(sorted.begin(), sorted.end());
    root = new Node('\0');
    for (size_t i = 0; i < sorted.size(); ++i) {
        char c = sorted[i];
        std::vector<char> remaining;
        remaining.reserve(sorted.size() - 1);
        for (size_t j = 0; j < sorted.size(); ++j) {
            if (j != i) remaining.push_back(sorted[j]);
        }
        Node* child = createNode(c, remaining);
        root->children.push_back(child);
    }
    if (!root->children.empty()) {
        root->subtree_perm_count = root->children.size() * root->children[0]->subtree_perm_count;
    }
    else {
        root->subtree_perm_count = 0;
    }
}

PMTree::~PMTree() {
    delete root;
}

void dfsCollect(Node* node, std::vector<char>& current, std::vector<std::vector<char>>& result, bool isRoot) {
    if (!isRoot) current.push_back(node->value);
    if (node->children.empty()) {
        result.push_back(current);
    }
    else {
        for (Node* child : node->children) {
            dfsCollect(child, current, result, false);
        }
    }
    if (!isRoot) current.pop_back();
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    Node* root = tree.getRoot();
    if (!root) return result;
    std::vector<char> current;
    dfsCollect(root, current, result, true);
    return result;
}

bool dfsFind(Node* node, std::vector<char>& current, int& counter, int target, bool isRoot, std::vector<char>& result) {
    if (!isRoot) current.push_back(node->value);
    if (node->children.empty()) {
        ++counter;
        if (counter == target) {
            result = current;
            if (!isRoot) current.pop_back();
            return true;
        }
    }
    else {
        for (Node* child : node->children) {
            if (dfsFind(child, current, counter, target, false, result)) {
                if (!isRoot) current.pop_back();
                return true;
            }
        }
    }
    if (!isRoot) current.pop_back();
    return false;
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
    if (num < 1) return {};
    Node* root = tree.getRoot();
    if (!root || root->subtree_perm_count < num) return {};
    std::vector<char> current, result;
    int counter = 0;
    dfsFind(root, current, counter, num, true, result);
    return result;
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
    if (num < 1) return {};
    Node* root = tree.getRoot();
    if (!root || root->subtree_perm_count < num) return {};
    std::vector<char> result;
    Node* current = root;
    long long n = num;
    while (!current->children.empty()) {
        long long perms_per_child = current->children[0]->subtree_perm_count;
        long long idx = (n - 1) / perms_per_child;
        Node* next = current->children[idx];
        result.push_back(next->value);
        n = (n - 1) % perms_per_child + 1;
        current = next;
    }
    return result;
}
