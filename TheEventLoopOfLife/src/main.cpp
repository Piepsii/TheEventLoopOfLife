//main.cpp

#include <SFML/Graphics.hpp>

int main() {
    sf::VideoMode videoMode;
    const char* title = "The Event Loop of Life";

    sf::RenderWindow window(sf::VideoMode(1080, 720), title);
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

        window.clear();
        window.display();
        sf::Time delay(sf::seconds(frameTime - delta.asSeconds()));
        sf::sleep(delay);
    }

    return 0;
}
