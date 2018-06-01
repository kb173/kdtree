#include <iostream>
#include "kdtree.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto *tree = new kdtree();

    int p1[2] = {2, 2}; // x
    int p2[2] = {1, 1}; // y
    int p3[2] = {3, 1}; // x
    int p4[2] = {4, -1};// y

    tree->insert(p1);
    tree->insert(p2);
    tree->insert(p3);
    tree->insert(p4);

    int p5[2] = {1, 0};

    int **result = tree->search(p5, 2);

    for (int i = 0; i < 2; i++) {
        cout << "Point found: ";

        for (int d = 0; d < 2; d++) {
            cout << result[i][d] << ", ";
        }

        cout << endl;
    }

    tree->print();

    return 0;
}