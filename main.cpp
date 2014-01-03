#include <cstdio>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "game.hpp"

int main(int argc, char** argv)
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Sumo Ball");
    window.setFramerateLimit(60);
    Game * game = new Game();

    int score[] = {0, 0};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (int result = game->update())
        {
			score[result - 1]++;

			delete game;
			game = new Game();
			std::cout << "Player one: " << score[0] << "\tPlayer two: "<< score[1] << std::endl;
        }

        window.clear(sf::Color::White);
        game->draw(&window);
        window.display();
    }

    return 0;
}
