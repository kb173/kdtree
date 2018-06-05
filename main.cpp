#include <iostream>
#include "kdtree.h"
#include <algorithm>
#include <fstream>
#include <SFML/Graphics.hpp>


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
    file >> length;

    auto **points = new double*[length];

    for (int i = 0; i < length; i++)
    {
        points[i] = new double[2];
    }


    long count = 0;
    bool isx = true;
    while (file >> number) {
        if(isx){
            points[count][0] = number;
            isx = !isx;
        } else{
            points[count][1] = number;
            isx = !isx;
            count++;
        }
    }

    tree->insert(points, length, 0);

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
        if (result[i] != nullptr)
        {
            cout << "Point found: ";

            for (int d = 0; d < 2; d++)
            {
                cout << result[i][d] << ", ";
            }
            cout << endl;
        }
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(1440, 1440), "k-d-tree");

    sf::CircleShape normal_p(10);
    normal_p.setFillColor(sf::Color(250, 250, 250));

    sf::CircleShape search_p(10);
    search_p.setFillColor(sf::Color(250, 100, 50));

    sf::CircleShape found_p(10);
    found_p.setFillColor(sf::Color(100, 250, 50));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (int i = 0; i < length; i++)
        {
            normal_p.setPosition(720 + points[i][0], 720 + points[i][1]);
            window.draw(normal_p);
        }

        search_p.setPosition(720 + search_point[0], 720 + search_point[1]);
        window.draw(search_p);

        for (int i = 0; i < amount; i++)
        {
            if (result[i] != nullptr)
            {
                found_p.setPosition(720 + result[i][0], 720 + result[i][1]);
                window.draw(found_p);
            }
        }

        // end the current frame
        window.display();
    }

    return 0;
}