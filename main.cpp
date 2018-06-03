#include <iostream>
#include <fstream>
#include <sstream>
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

    int number;
    bool isx;

    int xvalue;
    int yvalue;

    file >> number;

    while (file >> number) {
        if(isx){
            xvalue = number;
            isx = !isx;
        } else{
            yvalue = number;
            isx = !isx;

            int point[2] = {xvalue, yvalue};
            tree->insert(point);
        }
    }

    int searchx;
    int searchy;
    int amount;

    cout << "Enter the point you want to search for: " << endl;

    cin >> searchx;
    cin >> searchy;

    cout << "How many points do you want to search for in its surroundings" << endl;

    cin >> amount;

    int searchpoint[2] = {searchx, searchy};
    int **result = tree->search(searchpoint, amount);


    for (int i = 0; i < amount; i++) {
        cout << "Point found: ";

        for (int d = 0; d < amount; d++) {
            cout << result[i][d] << ", ";
        }

        cout << endl;
    }

    tree->print();

    return 0;
}