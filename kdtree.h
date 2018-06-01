//
// Created by karl on 31.05.18.
//

#include <cstdlib>
#include <queue>

using namespace std;

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

class kdtree {
private:
    const static int dimension = 2; // Used for vectors, eg vector[dimension]

    // NODE CLASS
    class node {
    public:
        int *p;
        int dim;
        node* left;
        node* right;

        node (int *np, int ndim);
    };

    // RECT CLASS
    class rect {
    public:
        int *origin, *end;

        rect ();
        void print ();
    };

    // CIRCLE CLASS
    class circ {
    public:
        int *origin, radius;

        circ (int* orig, int rad);
    };

    // HEAP ENTRY CLASS


    // HEAP CLASS
    class point_heap {
    private:
        class heap_point {
        public:
            int *point;
            int dist;

            heap_point(int* p, int d);
        };

        // Lambda expression for comparing in priority queue
        struct compare
        {
            bool operator()(const heap_point p1, const heap_point p2)
            {
                return p1.dist < p2.dist;
            }
        };


        priority_queue<heap_point, vector<heap_point>, compare> heap;
        int amount;
    public:
        explicit point_heap(int a);
        bool add(int* p, int dist);
        int get_worst_dist();
        int** get_points();
    };

    node* root; // The first inserted value
    rect* bounds; // The bounding box of the whole tree - Encompasses all points
    node* insert_rec(node *root, int point[dimension], int depth);
    point_heap* search_rec(node *root, rect *current_bounds, point_heap *best, circ *c);
    void print_rec(node *root, int depth);
    void del_rec(node *root);


public:
    kdtree();
    ~kdtree();

    void insert(int point[dimension]);
    int **search(int *point, int amount);
    void print();
};


#endif //KDTREE_KDTREE_H
