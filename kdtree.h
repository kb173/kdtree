//
// Created by karl on 31.05.18.
//

#include <cstdlib>
#include <queue>

using namespace std;

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

class kdtree
{
private:
    const static int dimension = 2; // Used for vectors, eg vector[dimension]

    // NODE CLASS
    class node
    {
    public:
        double *p;
        int dim;
        node *left;
        node *right;

        node(double *np, int ndim);
    };

    // RECT CLASS
    class rect
    {
    public:
        double *origin, *end;

        rect();

        void print();
    };

    // CIRCLE CLASS
    class circ
    {
    public:
        double *origin, radius;

        circ(double *orig, double rad);
    };

    // HEAP ENTRY CLASS


    // HEAP CLASS
    class point_heap
    {
    private:
        class heap_point
        {
        public:
            double *point;
            double dist;

            heap_point(double *p, double d);
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

        bool add(double *p, double dist);

        double get_worst_dist();

        double **get_points();
    };

    node *root; // The first inserted value
    rect *bounds; // The bounding box of the whole tree - Encompasses all points
    node *insert_rec(node *root, double point[dimension], int depth);

    point_heap *search_rec(node *root, rect *current_bounds, point_heap *best, circ *c);

    double get_distance(double *p1, double *p2);

    void print_rec(node *root, int depth);

    void del_rec(node *root);

    bool intersect(rect *r, circ *c);


public:
    kdtree();

    ~kdtree();

    void insert(double point[dimension]);

    double **search(double *point, int amount);

    void print();
};


#endif //KDTREE_KDTREE_H
