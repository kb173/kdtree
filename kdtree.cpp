//
// Created by karl on 31.05.18.
//

#include "kdtree.h"
#include <cstdlib>
#include <iostream>

kdtree::node* kdtree::insert_rec(kdtree::node *root, int *point, int depth) { // Recursively insert a new point into the tree
    int current_dim = depth % dimension; // Get current dimension based on depth; should go from 0 to dimension, then loop

    if (root == nullptr) { // Root is null -> Create new node and return it (exit condition)
        auto *tmp = new node(point, depth);

        return tmp;
    }

    // Insert the new node left or right of the current node, based on the value at the new dimension
    if (point[current_dim] > root->p[current_dim]) {
        std::cout << "Inserting right" << std::endl;
        root->right = insert_rec(root->right, point, ++depth);
    } else {
        std::cout << "Inserting left" << std::endl;
        root->left = insert_rec(root->left, point, ++depth);
    }

    return root;
}

void kdtree::insert(int *point) {
    root = insert_rec(root, point, 0);
}

kdtree::kdtree() {
    root = nullptr;
}
