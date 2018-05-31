//
// Created by karl on 31.05.18.
//

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
    };

    node* root; // The first inserted value
    rect* bounds; // The bounding box of the whole tree - Encompasses all points
    node* insert_rec(node *root, int point[dimension], int depth);

public:
    kdtree();

    void insert(int point[dimension]);
};


#endif //KDTREE_KDTREE_H
