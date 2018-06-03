#include <iostream>
#include <fstream>
#include "kdtree.h"


int main(int argc, char *argv[]) {

    auto *tree = new kdtree();

    if (argc > 1) {
        cout << "argv[1] = " << argv[1] << endl;
    } else {
        cout << "No file name entered. Exiting...";
        return -1;
    }

    ifstream file;
    file.open(argv[1]);


    int length;
    double number;
    file >> number;

    double points[length][2];

    long count = 0;
    bool isx = true;
    while (file >> number) {
        if(isx){
            points[count][0] = number;
            isx = !isx;
        } else{
            points[count][1] = number;
            isx = !isx;
        }
    }

    for (int i = 0; i < length; i++)
    {
        tree->insert(points[i]);
    }

    tree->print();


    double searchx;
    double searchy;
    int amount;

    cout << "Enter the point you want to search for: " << endl;

    cin >> searchx;
    cin >> searchy;

    cout << "How many points do you want to search for in its surroundings" << endl;
    
    cin >> amount;


    double search_point[2] = {searchx, searchy};
    double **result = new double*[amount];


    for (int i = 0; i < 2; i++)
    {
        result[i] = new double[2];
    }

    result = tree->search(search_point, amount);

    for (int i = 0; i < amount; i++)
    {
        cout << "Point found: ";

        for (int d = 0; d < 2; d++)
        {
            cout << result[i][d] << ", ";
        }

        cout << endl;
    }

    return 0;
}