//
// Created by karl on 31.05.18.
//

#include "kdtree.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <limits>

// CONSTRUCTORS AND FUNCTIONS FOR CHILD CLASSES

kdtree::node::node(int *np, int ndim) { // Constructor for node without values left or right
    p = new int[dimension];

    for (int i = 0; i < dimension; i++) {
        p[i] = np[i]; // There it is
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

kdtree::circ::circ(int *orig, int rad) {
    origin = orig;
    radius = rad;
}

kdtree::point_heap::heap_point::heap_point(int *p, int d) {
    point = new int[dimension];

    for (int i = 0; i < dimension; i++) {
        point[i] = p[i];
    }

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

int kdtree::point_heap::get_worst_dist() {
    // Return infinity if heap is not full, worst distance if full
    if (heap.size() < amount)
        return std::numeric_limits<int>::max();

    return heap.top().dist;
}

// KD-TREE FUNCTIONS

// PRIVATE FUNCTIONS

kdtree::node* kdtree::insert_rec(kdtree::node *root, int *point, int depth) { // Recursively insert a new point
    cout << "Inserting " << point[0] << ", " << point[1] << endl;

    // Get current dimension based on depth; should go from 0 to dimension, then loop back to 0
    int current_dim = depth % dimension;

    if (root == nullptr) { // Current node is null -> Create new node and return it (exit condition)
        cout << "Creating the node" << endl;

        auto *tmp = new node(point, depth);

        return tmp;
    }

    // Insert the new node left or right of the current node, based on the value at the current check dimension
    if (point[current_dim] > root->p[current_dim]) {
        std::cout << "Inserting right" << std::endl;

        root->right = insert_rec(root->right, point, ++depth);
    } else {
        std::cout << "Inserting left" << std::endl;

        root->left = insert_rec(root->left, point, ++depth);
    }

    return root;
}

kdtree::point_heap *
kdtree::search_rec(kdtree::node *root, kdtree::rect *current_bounds, kdtree::point_heap *best, kdtree::circ *c) {
    cout << "Searching at node: " << root->p[0] << ", " << root->p[1] << endl;

    int dist;

    // Calculate distance (Pythagoras)
    int squared_sum = 0;

    for (int i = 0; i < dimension; i++) {
        squared_sum += pow(root->p[i] - c->origin[i], 2);
    }

    dist = (int)sqrt((double)squared_sum);

    // Try inserting point into heap (works if heap is not full or point is better than worst point in heap)
    if (best->add(root->p, dist)) {
        c->radius = best->get_worst_dist(); // If insert was successful, get new worst distance
    }

    // Check on which side point is and continue search at better path
    bool is_left = c->origin[root->dim] < root->p[root->dim];

    // TODO: Set intersect based on whether bound rect and circle overlap

    // Search the better side first and get updated circle
    if (is_left && root->left) {
        search_rec(root->left, current_bounds, best, c);
    } else if (!is_left && root->right){
        search_rec(root->right, current_bounds, best, c);
    }

    // Check for intersection on the worse side and search there if there was an intersection
    bool intersect = true;

    if (intersect) {
        if (is_left && root->right) {
            search_rec(root->right, current_bounds, best, c);
        } else if (!is_left && root->left){
            search_rec(root->left, current_bounds, best, c);
        }
    }

    return best; // Exit condition: No node was either on the good side or intersected
}

void kdtree::print_rec(kdtree::node *root, int depth) {
    cout << "Depth: " << depth << endl;
    cout << "Node with point: ";
    for (int i = 0; i < dimension; i++) {
        cout << root->p[i] << ", ";
    }
    cout << endl;

    if (root->left) {
        cout << "Left: " << endl;
        print_rec(root->left, depth + 1);
    }

    if (root->right) {
        cout << "Right: " << endl;
        print_rec(root->right, depth + 1);
    }
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

int **kdtree::search(int *point, int amount) {
    // Priority queue which holds the found points
    auto heap = new point_heap(amount);

    // Radius of circle is infinite as long as heap is not full
    auto circle = new circ(point, std::numeric_limits<int>::max());

    point_heap *result = search_rec(root, bounds, heap, circle);

    return result->get_points();
}

void kdtree::print() {
    cout << "---------- Printing Tree ----------" << endl;

    print_rec(root, 0);

    cout << "-----------------------------------" << endl;
}