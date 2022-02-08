#include <SFML\Graphics.hpp>
#include "Simulation.h"
#include "InputManager.h"
#include "Time.h"

int main() {
    sf::VideoMode videoMode;
    const char* title = "The Event Loop of Life";
    InputManager input;

    // 10 tiles * 32 pixels + 9 borders
    const int width = 649;
    const int height = 649;

    const unsigned int rows = 10;
    const unsigned int columns = 10;

    Simulation* sim = new Simulation(width, height);
    sim->createGrid(rows, columns);
    sf::RenderWindow window(sf::VideoMode(width, height), title);

    sf::Clock clock;
    sf::Time delta;
    float hertz = 60.0f;
    float frameTime = 1.0f / hertz;
    bool running = true;
    
    while (running)
    {
        sf::Event event;
        delta = clock.restart();
        while (window.pollEvent(event))
        {
            switch (event.type) {
            case sf::Event::KeyPressed:
                input.setKey(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                input.setKey(event.key.code, false);
                break;
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        if (input.isKeyPressed(sf::Keyboard::Key::Down)) {
            hertz -= 10.0f;
        }
        if (input.isKeyPressed(sf::Keyboard::Key::Up)) {
            hertz += 10.0f;
        }
        frameTime = 1.0f / hertz;

        window.clear(sf::Color::Black);
        window.setActive();

        if (Time::Update() && sim->Update(delta.asSeconds())) {
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
