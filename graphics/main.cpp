#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    // Grass background
    sf::RectangleShape grass(sf::Vector2f(1200, 350));
    grass.setPosition(sf::Vector2f(0, 450));
    grass.setFillColor(sf::Color(110, 130, 70)); // dark green

    // Inclined ground (triangle)
    sf::ConvexShape slope;
    slope.setPointCount(7);
    slope.setPoint(0, sf::Vector2f(0, 400));     // top left
    slope.setPoint(1, sf::Vector2f(150, 400));   // top right
    slope.setPoint(2, sf::Vector2f(270, 560));   // bottom right
    slope.setPoint(3, sf::Vector2f(248, 564));   // bottom right
    slope.setPoint(4, sf::Vector2f(224, 568));   // bottom right
    slope.setPoint(5, sf::Vector2f(200, 572));   // bottom right
    slope.setPoint(6, sf::Vector2f(0, 575));     // bottom left
    slope.setFillColor(sf::Color(143, 163, 90)); // green

    // Dark gray bridge
    sf::ConvexShape darkGrayBridge;
    darkGrayBridge.setPointCount(7);
    darkGrayBridge.setPoint(0, sf::Vector2f(150, 400));
    darkGrayBridge.setPoint(1, sf::Vector2f(303, 400));
    darkGrayBridge.setPoint(2, sf::Vector2f(303, 420));
    darkGrayBridge.setPoint(3, sf::Vector2f(343, 420));
    darkGrayBridge.setPoint(4, sf::Vector2f(343, 550));
    darkGrayBridge.setPoint(5, sf::Vector2f(330, 560));
    darkGrayBridge.setPoint(6, sf::Vector2f(270, 560));
    darkGrayBridge.setFillColor(sf::Color(50, 50, 50)); // dark gray

    // River
    sf::ConvexShape river;
    river.setPointCount(13);
    river.setPoint(0, sf::Vector2f(260, 450));
    river.setPoint(1, sf::Vector2f(376, 455));
    river.setPoint(2, sf::Vector2f(604, 473));
    river.setPoint(3, sf::Vector2f(739, 486));
    river.setPoint(4, sf::Vector2f(802, 506));
    river.setPoint(5, sf::Vector2f(1160, 800));
    river.setPoint(6, sf::Vector2f(1160, 800));
    river.setPoint(7, sf::Vector2f(417, 800));
    river.setPoint(8, sf::Vector2f(424, 597));
    river.setPoint(9, sf::Vector2f(421, 542));
    river.setPoint(10, sf::Vector2f(397, 475));
    river.setPoint(11, sf::Vector2f(303, 461));
    river.setPoint(12, sf::Vector2f(260, 455));
    river.setFillColor(sf::Color(100, 149, 237)); // river blue

    // Main game loop
    while (window.isOpen()) {
        // Process events
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Clear with sky blue color
        window.clear(sf::Color(173, 216, 230));

        window.draw(grass);
        window.draw(slope);
        window.draw(river);
        window.draw(darkGrayBridge);

        // Display everything
        window.display();
    }

    return 0;
}
