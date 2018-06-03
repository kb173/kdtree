//
// Created by karl on 31.05.18.
//

#include "kdtree.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <limits>

// CONSTRUCTORS AND FUNCTIONS FOR CHILD CLASSES

kdtree::node::node(double *np, int ndim)
{ // Constructor for node without values left or right
    p = new double[dimension]; // Deleted in destructor of tree

    for (int i = 0; i < dimension; i++)
    {
        p[i] = np[i]; // There it is
    }

    dim = ndim;
    left = nullptr;
    right = nullptr;
}

kdtree::rect::rect()
{ // Initialize origin and size vector to 0
    origin = new double[dimension]; // Deleted in destructor
    end = new double[dimension]; // Deleted in destructor

    for (int i = 0; i < dimension; i++)
    {
        origin[i] = 0;
        end[i] = 0;
    }
}

void kdtree::rect::print()
{
    cout << "Origin: (";

    for (int i = 0; i < dimension; i++)
    {
        cout << origin[i] << ", ";
    }

    cout << ");  " << "End: (";

    for (int i = 0; i < dimension; i++)
    {
        cout << end[i] << ", ";
    }

    cout << ")" << endl;
}

kdtree::rect::~rect()
{
    delete origin;
    delete end;
    delete this;
}

kdtree::circ::circ(double *orig, double rad)
{
    origin = orig;
    radius = rad;
}

kdtree::circ::~circ()
{
    delete origin;
    delete this;
}

kdtree::point_heap::heap_point::heap_point(double *p, double d)
{
    point = new double[dimension]; // Deleted in destructor

    for (int i = 0; i < dimension; i++)
    {
        point[i] = p[i];
    }

    dist = d;
}

kdtree::point_heap::heap_point::~heap_point()
{
}

kdtree::point_heap::point_heap(int a)
{
    amount = a;
}

bool kdtree::point_heap::add(double *p, double dist)
{
    if (heap.size() < amount)
    { // Heap is not full yet, so insert regardless of points in heap
        heap.push(heap_point(p, dist));
        return true;
    }

    if (heap.top().dist > dist)
    { // Distance of new point is smaller than distance of furthest point in heap
        heap.pop();
        heap.push(heap_point(p, dist));
        return true;
    }

    return false; // Heap is full and dist of new point is greater than dist of furthest point in heap
}

double **kdtree::point_heap::get_points()
{
    auto **pts = new double *[amount]; // Needs to be deleted manually!
    int current = 0;

    while (!heap.empty())
    {
        pts[current] = new double[dimension];
        pts[current] = heap.top().point;

        heap.pop();

        current++;
    }

    return pts;
}

double kdtree::point_heap::get_worst_dist()
{
    // Return infinity if heap is not full, worst distance if full
    if (heap.size() < amount)
        return numeric_limits<double>::max();

    return heap.top().dist;
}

// KD-TREE FUNCTIONS

// PRIVATE FUNCTIONS

kdtree::node *kdtree::insert_rec(kdtree::node *root, double *point, int depth)
{ // Recursively insert a new point
    // cout << "Inserting " << point[0] << ", " << point[1] << endl;

    // Get current dimension based on depth; should go from 0 to dimension, then loop back to 0
    int current_dim = depth % dimension;

    if (root == nullptr)
    { // Current node is null -> Create new node and return it (exit condition)
        // cout << "Creating the node" << endl;

        auto *tmp = new node(point, depth); // Deleted in destructor of tree

        return tmp;
    }

    // Check if identical point is already in the tree, don't insert if so
    bool identical = true;

    for (int i = 0; i < dimension; i++)
    {
        if (root->p[i] != point[i])
        {
            identical = false;
            break;
        }
    }

    if (identical)
    {
        // cout << "Identical point already exists, aborting" << endl;
        print();
        return root;
    }

    // Insert the new node left or right of the current node, based on the value at the current check dimension
    if (point[current_dim] > root->p[current_dim])
    {
        // std::cout << "Inserting right" << std::endl;

        root->right = insert_rec(root->right, point, ++depth);
    } else
    {
        // std::cout << "Inserting left" << std::endl;

        root->left = insert_rec(root->left, point, ++depth);
    }

    return root;
}

bool kdtree::search_rec(kdtree::node *root, kdtree::rect *current_bounds, kdtree::point_heap *best, kdtree::circ *c)
{
    // cout << "Searching at node: " << root->p[0] << ", " << root->p[1] << endl;

    double dist = get_distance(root->p, c->origin); // Distance between current point and point we're searching for

    // Try inserting point into heap (true if heap is not full or point is better than worst point in heap)
    if (best->add(root->p, dist))
    {
        c->radius = best->get_worst_dist(); // If insert was successful, get new worst distance
    }

    // Check on which side point is
    bool is_left = c->origin[root->dim] < root->p[root->dim];

    // Get bounds for both sides
    auto *left_bounds = new rect(); // Both deleted by destructor
    auto *right_bounds = new rect();

    for (int i = 0; i < dimension; i++) // Copy old bounds into new ones first
    {
        left_bounds->origin[i] = current_bounds->origin[i];
        left_bounds->end[i] = current_bounds->end[i];

        right_bounds->origin[i] = current_bounds->origin[i];
        right_bounds->end[i] = current_bounds->end[i];
    }

    // For the bounds of the section on the left, we need to move the end, since the right side drops out
    left_bounds->end[root->dim] = root->p[root->dim];
    right_bounds->origin[root->dim] = root->p[root->dim];

    /*cout << "Overall bounds: " << endl;
    current_bounds->print();

    cout << "Left bounds: " << endl;
    left_bounds->print();

    cout << "Right bounds: " << endl;
    right_bounds->print();

    cout << "Radius: " << c->radius << endl;*/


    // We need to go deeper
    if (is_left) // Searchpoint is on the left of current axis
    {
        // If the good path encloses all possible points, this becomes true and the worse path is not checked
        bool done = false;

        // Search at the obvious route first
        if (root->left)
        {
            done = search_rec(root->left, left_bounds, best, c);
        }

/*        if (done)
        {
            // cout << "Overlap - we're done!" << endl;
        }*/

        // Also search the other side if the bounds overlap with the search circle
        if (!done && root->right && intersect(right_bounds, c))
        {
            search_rec(root->right, right_bounds, best, c);
        }
    } else
    {
        // Same thing for the right side
        bool done = false;

        if (root->right)
        {
            done = search_rec(root->right, right_bounds, best, c);
        }

/*        if (done)
        {
            cout << "Overlap - we're done!" << endl;
        }*/

        if (!done && root->left && intersect(left_bounds, c))
        {
            search_rec(root->left, left_bounds, best, c);
        }
    }

    // Exit condition: No node was either on the good side or intersected
    return within(current_bounds, c);
}

void kdtree::print_rec(kdtree::node *root, int depth)
{
    cout << "Depth: " << depth << endl;
    cout << "Node with point: ";
    for (int i = 0; i < dimension; i++)
    {
        cout << root->p[i] << ", ";
    }
    cout << endl;

    if (root->left)
    {
        cout << "Left: " << endl;
        print_rec(root->left, depth + 1);
    }

    if (root->right)
    {
        cout << "Right: " << endl;
        print_rec(root->right, depth + 1);
    }
}

void kdtree::del_rec(kdtree::node *root)
{
    if (root->left)
    {
        del_rec(root->left);
    }

    if (root->right)
    {
        del_rec(root->right);
    }

    delete root->p;
    delete root;
}

bool kdtree::intersect(kdtree::rect *r, kdtree::circ *c)
{
    // Get closest point within rectangle to circle
    auto *clamped = new double[dimension]; // Deleted below

    for (int i = 0; i < dimension; i++)
    {
        // Make clamped[i] be between r->origin[i] and r->end[i]
        if (c->origin[i] < r->origin[i])
        {
            clamped[i] = r->origin[i];
        } else if (c->origin[i] > r->end[i])
        {
            clamped[i] = r->end[i];
        } else
        {
            clamped[i] = c->origin[i];
        }
    }

    double dist = get_distance(clamped, c->origin);

    delete clamped;

    // If distance is smaller than the circle radius, that means they're intersecting
    return dist < c->radius;
}

bool kdtree::within(kdtree::rect *r, kdtree::circ *c)
{
    // Check if closest point within circle to rectangle is within the rectangle
    for (int i = 0; i < dimension; i++) {
        if (r->origin[i] > c->origin[i] - c->radius) // ex: |-radius   |r->origin   |c->origin
        {
            return false;
        }

        if (r->end[i] < c->origin[i] + c->radius) // ex: |c->origin   |r->end    |+radius
        {
            return false;
        }
    }

    return true;
}

double kdtree::get_distance(double *p1, double *p2)
{
    // Pythagoras
    double squared_sum = 0;

    for (int i = 0; i < dimension; i++)
    {
        squared_sum += pow(p1[i] - p2[i], 2);
    }

    return sqrt(squared_sum);
}

// PUBLIC FUNCTIONS

kdtree::kdtree()
{
    root = nullptr;
    bounds = new rect(); // Deleted in destructor of tree
}

void kdtree::insert(double *point)
{
    // Handle how the overall bounds change due to this new point
    if (root == nullptr)
    { // Set bounds to just this point if it is the first inserted point
        bounds->origin = new double[dimension](); // Both deleted in destructor of tree
        bounds->end = new double[dimension]();

        for (int i = 0; i < dimension; i++)
        {
            bounds->origin[i] = point[i];
            bounds->end[i] = point[i];
        }
    } else
    { // If it is not the first point, update bounds to match new point
        for (int i = 0; i < dimension; i++)
        {
            if (point[i] < bounds->origin[i])
            {
                bounds->origin[i] = point[i];
            } else if (point[i] > bounds->end[i])
            {
                bounds->end[i] = point[i];
            }
        }
    }

    root = insert_rec(root, point, 0);

    /*// Debug info
    std::cout << "Insert successful!" << std::endl;
    std::cout << "New bounds: (" << bounds->origin[0] << ", " << bounds->origin[1] << ") ";
    std::cout << "(" << bounds->end[0] << ", " << bounds->end[1] << ") " << std::endl;*/
}

double **kdtree::search(double *point, int amount)
{
    // Priority queue which holds the found points
    auto heap = new point_heap(amount); // Deleted below

    // Radius of circle is infinite as long as heap is not full
    auto circle = new circ(point, std::numeric_limits<double>::max()); // Suicide in destructor

    search_rec(root, bounds, heap, circle);

    double **pts = heap->get_points();

    delete heap;

    return pts;
}

void kdtree::print()
{
    cout << "---------- Printing Tree ----------" << endl;

    print_rec(root, 0);

    cout << "-----------------------------------" << endl;
}

kdtree::~kdtree()
{
    // Delete nodes
    del_rec(root);

    // Delete bounds
    delete bounds->origin;
    delete bounds->end;
    delete bounds;
}
