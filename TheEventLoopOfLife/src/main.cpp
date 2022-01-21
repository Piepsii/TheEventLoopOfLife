//main.cpp

#include <SFML\Graphics.hpp>
#include "Simulation.h"

int main() {
    sf::VideoMode videoMode;
    const char* title = "The Event Loop of Life";

    Simulation* sim = new Simulation(640, 640, 10);
    sf::RenderWindow window(sf::VideoMode(640, 640), title);

    sf::Clock clock;
    sf::Time delta;
    float frameTime = 1.0f / 60.0f;
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
