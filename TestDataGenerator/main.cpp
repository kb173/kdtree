#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int main()
{
    static const int point_num = 20;

    srand (time(NULL));

    ofstream outfile ("points.txt");

    outfile << point_num << endl;

    for (int i = 0; i < point_num; i++)
    {
        outfile << (double)(rand() % 14400 - 7200) / 10
                << " " << (double)(rand() % 14400 - 7200) / 10 << endl;
    }

    outfile.close();

    return 0;
}