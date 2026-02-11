#include <SFML/Graphics.hpp>

int main()
{
    // Create a window with 800x600 resolution
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Bridge Builder - Graphics Test");

    // Main game loop
    while (window.isOpen())
    {
        // Process events
        while (auto event = window.pollEvent())
        {
            // Close window when X is clicked
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Clear screen with a dark gray color
        window.clear(sf::Color(50, 50, 50));

        // Draw a simple shape to test rendering
        sf::CircleShape shape(50.f);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition({375.f, 275.f});
        window.draw(shape);

        // Display what was drawn
        window.display();
    }

    return 0;
}
