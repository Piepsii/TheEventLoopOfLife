//main.cpp

#include <SFML\Graphics.hpp>
#include "Simulation.h"

int main() {
    sf::VideoMode videoMode;
    const char* title = "The Event Loop of Life";

    // 10 tiles * 32 pixels + 9 borders
    const int width = 649;
    const int height = 649;

    Simulation* sim = new Simulation(width, height, 10);
    sf::RenderWindow window(sf::VideoMode(width, height), title);

    sf::Clock clock;
    sf::Time delta;
    float frameTime = 1.0f / 5.0f;
    bool running = true;
    
    while (running)
    {
        sf::Event event;
        delta = clock.restart();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.setActive();

        if (sim->Update(delta.asSeconds())) {
            sim->Draw(window);
        }
        else {
            running = false;
        }

        window.display();
        sf::Time delay(sf::seconds(frameTime - delta.asSeconds()));
        sf::sleep(delay);
    }

    delete sim;
    sim = nullptr;

    return 0;
}
