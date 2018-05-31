//
// Created by karl on 31.05.18.
//

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

class kdtree {
private:
    const static int dimension = 2; // Used for vectors, eg vector[dimension]

    // NODE
    class node {
    public:
        int *p;
        int dim;
        node* left;
        node* right;

        node (int *np, int ndim) { // Constructor for node without values left or right
            p = new int[dimension];

            for (int i = 0; i < dimension; i++) {
                p[i] = np[i];
            }

            dim = ndim;
            left = nullptr;
            right = nullptr;
        }
    };

    // RECT
    class rect {
    public:
        int *origin, *end;

        rect (){ // Initialize origin and size vector to 0
            origin = new int[dimension];
            end = new int[dimension];

            for (int i = 0; i < dimension; i++) {
                origin[i] = 0;
                end[i] = 0;
            }
        }
    };

    node* root;
    rect* bounds;
    node* insert_rec(node *root, int point[dimension], int depth);

public:
    kdtree();

    void insert(int point[dimension]);
};


#endif //KDTREE_KDTREE_H
