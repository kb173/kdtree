//
// Created by karl on 31.05.18.
//

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

class kdtree {
private:
    const static int dimension = 2;

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

    node* root;
    node* insert_rec(node *root, int point[dimension], int depth);

public:
    kdtree();

    void insert(int point[dimension]);
};


#endif //KDTREE_KDTREE_H
