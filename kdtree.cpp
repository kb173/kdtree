//
// Created by karl on 31.05.18.
//

#include "kdtree.h"
#include <cstdlib>
#include <iostream>

// CONSTRUCTORS FOR CHILD CLASSES

kdtree::node::node(int *np, int ndim) { // Constructor for node without values left or right
    p = new int[dimension];

    for (int i = 0; i < dimension; i++) {
        p[i] = np[i];
    }

    dim = ndim;
    left = nullptr;
    right = nullptr;
}

kdtree::rect::rect() { // Initialize origin and size vector to 0
    origin = new int[dimension];
    end = new int[dimension];

    for (int i = 0; i < dimension; i++) {
        origin[i] = 0;
        end[i] = 0;
    }
}

kdtree::circ::circ() {
    origin = nullptr;
    radius = 0;
}


// PRIVATE FUNCTIONS

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

// PUBLIC FUNCTIONS

kdtree::kdtree() { // Initialize root and bounds
    root = nullptr;
    bounds = new rect();
}

void kdtree::insert(int *point) {
    if (root == nullptr) { // Set bounds to just this point if it is the first inserted point
        bounds->origin = new int[dimension]();
        bounds->end = new int[dimension]();

        for (int i = 0; i < dimension; i++) {
            bounds->origin[i] = point[i];
            bounds->end[i] = point[i];
        }
    } else { // If it is not the first point, update bounds to match new point
        for (int i = 0; i < dimension; i++) {
            if (point[i] < bounds->origin[i]) {
                bounds->origin[i] = point[i];
            } else if (point[i] > bounds->end[i]) {
                bounds->end[i] = point[i];
            }
        }
    }

    root = insert_rec(root, point, 0);

    // Debug info
    std::cout << "Insert successful!" << std::endl;
    std::cout << "New bounds: ("<< bounds->origin[0] << ", " << bounds->origin[1] << ") ";
    std::cout << "(" << bounds->end[0] << ", " << bounds->end[1] << ") " << std::endl;
}

kdtree::point_heap::heap_point::heap_point(int *p, int d) {
    *point = *p;
    dist = d;
}

kdtree::point_heap::point_heap(int a) {
    amount = a;
}

bool kdtree::point_heap::add(int *p, int dist) {
    if (heap.size() < amount) { // Heap is not full yet, so insert regardless of points in heap
        heap.push(heap_point(p, dist));
        return true;
    }

    if (heap.top().dist > dist) { // Distance of new point is smaller than distance of furthest point in heap
        heap.pop();
        heap.push(heap_point(p, dist));
        return true;
    }

    return false; // Heap is full and dist of new point is greater than dist of furthest point in heap
}

int **kdtree::point_heap::get_points() {
    auto **pts = new int*[amount];
    int current = 0;

    while (!heap.empty()) {
        pts[current] = new int[dimension];
        pts[current] = heap.top().point;
        heap.pop();

        current++;
    }

    return pts;
}
