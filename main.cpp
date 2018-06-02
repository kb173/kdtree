#include <iostream>
#include "kdtree.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    auto *tree = new kdtree();

    double p1[2] = {2.5, 2}; // x
    double p2[2] = {1, 1}; // y
    double p3[2] = {3, 3}; // x
    double p4[2] = {4, -1};// y

    tree->insert(p1);
    tree->insert(p2);
    tree->insert(p3);
    tree->insert(p4);

    double p5[2] = {3, 4};

    double **result = tree->search(p5, 2);

    for (int i = 0; i < 2; i++)
    {
        cout << "Point found: ";

        for (int d = 0; d < 2; d++)
        {
            cout << result[i][d] << ", ";
        }

        cout << endl;
    }

    tree->print();

    return 0;
}