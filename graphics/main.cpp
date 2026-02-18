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
    sf::ConvexShape slopeLeft;
    slopeLeft.setPointCount(7);
    slopeLeft.setPoint(0, sf::Vector2f(0, 400));     // top left
    slopeLeft.setPoint(1, sf::Vector2f(150, 400));   // top right
    slopeLeft.setPoint(2, sf::Vector2f(270, 560));   // bottom right
    slopeLeft.setPoint(3, sf::Vector2f(248, 564));   // bottom right
    slopeLeft.setPoint(4, sf::Vector2f(224, 568));   // bottom right
    slopeLeft.setPoint(5, sf::Vector2f(200, 572));   // bottom right
    slopeLeft.setPoint(6, sf::Vector2f(0, 575));     // bottom left
    slopeLeft.setFillColor(sf::Color(143, 163, 90)); // green

    // Dark gray bridge - left side
    sf::ConvexShape darkGrayBridgeLeft;
    darkGrayBridgeLeft.setPointCount(7);
    darkGrayBridgeLeft.setPoint(0, sf::Vector2f(150, 400));
    darkGrayBridgeLeft.setPoint(1, sf::Vector2f(303, 400));
    darkGrayBridgeLeft.setPoint(2, sf::Vector2f(303, 420));
    darkGrayBridgeLeft.setPoint(3, sf::Vector2f(343, 420));
    darkGrayBridgeLeft.setPoint(4, sf::Vector2f(343, 550));
    darkGrayBridgeLeft.setPoint(5, sf::Vector2f(330, 560));
    darkGrayBridgeLeft.setPoint(6, sf::Vector2f(270, 560));
    darkGrayBridgeLeft.setFillColor(sf::Color(50, 50, 50)); // dark gray

    // Small square to say where the bridge starts
    sf::RectangleShape squareLeft(sf::Vector2f(674, 20));
    squareLeft.setPosition(sf::Vector2f(303, 400));
    squareLeft.setFillColor(sf::Color(80, 80, 80)); // dark gray (shadow)

    // sf::RectangleShape squareRight(sf::Vector2f(40, 20));
    // squareRight.setPosition(sf::Vector2f(1200-303+40, 400));
    // squareRight.setFillColor(sf::Color(80, 80, 80)); // dark gray (shadow)

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

    // Right side - mirrored dark gray bridge
    sf::ConvexShape darkGrayBridgeRight;
    darkGrayBridgeRight.setPointCount(7);
    darkGrayBridgeRight.setPoint(0, sf::Vector2f(1200-150+80, 400));
    darkGrayBridgeRight.setPoint(1, sf::Vector2f(1200-303+80, 400));
    darkGrayBridgeRight.setPoint(2, sf::Vector2f(1200-303+80, 420));
    darkGrayBridgeRight.setPoint(3, sf::Vector2f(1200-343+80, 420));
    darkGrayBridgeRight.setPoint(4, sf::Vector2f(1200-343+80, 550));
    darkGrayBridgeRight.setPoint(5, sf::Vector2f(1200-330+80, 560));
    darkGrayBridgeRight.setPoint(6, sf::Vector2f(1200-270+80, 560));
    darkGrayBridgeRight.setFillColor(sf::Color(50, 50, 50)); // dark gray

    // Right side - mirrored slope
    sf::ConvexShape slopeRight;
    slopeRight.setPointCount(7);
    slopeRight.setPoint(0, sf::Vector2f(1200 + 80, 400));         // top right
    slopeRight.setPoint(1, sf::Vector2f(1200 - 150 + 80, 400));   // top left
    slopeRight.setPoint(2, sf::Vector2f(1200 - 270 + 80, 560));   // bottom left
    slopeRight.setPoint(3, sf::Vector2f(1200 - 248 + 80, 564));   // bottom left
    slopeRight.setPoint(4, sf::Vector2f(1200 - 224 + 80, 568));   // bottom left
    slopeRight.setPoint(5, sf::Vector2f(1200 - 200 + 80, 572));   // bottom left
    slopeRight.setPoint(6, sf::Vector2f(1200 + 80, 575));         // bottom right
    slopeRight.setFillColor(sf::Color(143, 163, 90)); // green

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
        window.draw(slopeLeft);
        window.draw(river);
        window.draw(darkGrayBridgeLeft);
        window.draw(slopeRight);
        window.draw(darkGrayBridgeRight);
        window.draw(squareLeft);
        // window.draw(squareRight);

        // Display everything
        window.display();
    }

    return 0;
}
