#include <iostream>
#include "kdtree.h"

/*int main()
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
}*/

#include <SFML/Graphics.hpp>


int main()
{
    auto *tree = new kdtree();

    double points[6][2] = {{250, 200}, {300, 100}, {300, 300}, {200, 50}, {100, 50}, {100, 50}};


    for (int i = 0; i < 6; i++)
    {
        tree->insert(points[i]);
    }

    double search_point[2] = {30, 400};

    double **result = tree->search(search_point, 3);

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
        for (int i = 0; i < 6; i++)
        {
            sf::CircleShape normal_p(10);
            normal_p.setFillColor(sf::Color(250, 250, 250));
            normal_p.setOrigin(-500 + points[i][0], -500 + points[i][1]);
            window.draw(normal_p);
        }

        search_p.setOrigin(-500 + search_point[0], -500 + search_point[1]);
        window.draw(search_p);

        for (int i = 0; i < 3; i++)
        {
            found_p.setOrigin(-500 + result[i][0], -500 + result[i][1]);
            window.draw(found_p);
        }

        // end the current frame
        window.display();
    }

    return 0;
}