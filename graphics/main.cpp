#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    // Grass background
    sf::RectangleShape grass(sf::Vector2f(1200, 400));
    grass.setPosition(sf::Vector2f(0, 490));
    grass.setFillColor(sf::Color(110, 130, 70)); // dark green

    // Inclined ground (triangle)
    sf::ConvexShape slopeLeft;
    slopeLeft.setPointCount(7);
    slopeLeft.setPoint(0, sf::Vector2f(  0, 400+40));     // top left
    slopeLeft.setPoint(1, sf::Vector2f(150, 400+40));   // top right
    slopeLeft.setPoint(2, sf::Vector2f(270, 560+40));   // bottom right
    slopeLeft.setPoint(3, sf::Vector2f(248, 564+40));   // bottom right
    slopeLeft.setPoint(4, sf::Vector2f(224, 568+40));   // bottom right
    slopeLeft.setPoint(5, sf::Vector2f(200, 572+40));   // bottom right
    slopeLeft.setPoint(6, sf::Vector2f(  0, 575+40));     // bottom left
    slopeLeft.setFillColor(sf::Color(143, 163, 90)); // green

    // Dark gray bridge - left side
    sf::ConvexShape darkGrayBridgeLeft;
    darkGrayBridgeLeft.setPointCount(7);
    darkGrayBridgeLeft.setPoint(0, sf::Vector2f(150, 400+40));
    darkGrayBridgeLeft.setPoint(1, sf::Vector2f(303, 400+40));
    darkGrayBridgeLeft.setPoint(2, sf::Vector2f(303, 420+40));
    darkGrayBridgeLeft.setPoint(3, sf::Vector2f(343, 420+40));
    darkGrayBridgeLeft.setPoint(4, sf::Vector2f(343, 550+40));
    darkGrayBridgeLeft.setPoint(5, sf::Vector2f(330, 560+40));
    darkGrayBridgeLeft.setPoint(6, sf::Vector2f(270, 560+40));
    darkGrayBridgeLeft.setFillColor(sf::Color(50, 50, 50)); // dark gray

    // Bridge simulation
    sf::RectangleShape rect(sf::Vector2f(674, 20));
    rect.setPosition(sf::Vector2f(303, 440));
    rect.setFillColor(sf::Color(80, 80, 80)); // dark gray

    // sf::RectangleShape squareRight(sf::Vector2f(40, 20));
    // squareRight.setPosition(sf::Vector2f(1200-303+40, 400));
    // squareRight.setFillColor(sf::Color(80, 80, 80)); // dark gray (shadow)

    // River
    sf::ConvexShape river;
    river.setPointCount(13);
    river.setPoint(0, sf::Vector2f( 260, 450+40));
    river.setPoint(1, sf::Vector2f( 376, 455+40));
    river.setPoint(2, sf::Vector2f( 604, 473+40));
    river.setPoint(3, sf::Vector2f( 739, 486+40));
    river.setPoint(4, sf::Vector2f( 802, 506+40));
    river.setPoint(5, sf::Vector2f(1160, 800+40));
    river.setPoint(6, sf::Vector2f(1160, 800+40));
    river.setPoint(7, sf::Vector2f( 417, 800+40));
    river.setPoint(8, sf::Vector2f( 424, 597+40));
    river.setPoint(9, sf::Vector2f( 421, 542+40));
    river.setPoint(10, sf::Vector2f(397, 475+40));
    river.setPoint(11, sf::Vector2f(303, 461+40));
    river.setPoint(12, sf::Vector2f(260, 455+40));
    river.setFillColor(sf::Color(100, 149, 237)); // river blue

    // Right side - mirrored dark gray bridge
    sf::ConvexShape darkGrayBridgeRight;
    darkGrayBridgeRight.setPointCount(7);
    darkGrayBridgeRight.setPoint(0, sf::Vector2f(1200-150+80, 400+40));
    darkGrayBridgeRight.setPoint(1, sf::Vector2f(1200-303+80, 400+40));
    darkGrayBridgeRight.setPoint(2, sf::Vector2f(1200-303+80, 420+40));
    darkGrayBridgeRight.setPoint(3, sf::Vector2f(1200-343+80, 420+40));
    darkGrayBridgeRight.setPoint(4, sf::Vector2f(1200-343+80, 550+40));
    darkGrayBridgeRight.setPoint(5, sf::Vector2f(1200-330+80, 560+40));
    darkGrayBridgeRight.setPoint(6, sf::Vector2f(1200-270+80, 560+40));
    darkGrayBridgeRight.setFillColor(sf::Color(50, 50, 50)); // dark gray

    // Right side - mirrored slope
    sf::ConvexShape slopeRight;
    slopeRight.setPointCount(7);
    slopeRight.setPoint(0, sf::Vector2f(1200 + 80, 400+40));         // top right
    slopeRight.setPoint(1, sf::Vector2f(1200 - 150 + 80, 400+40));   // top left
    slopeRight.setPoint(2, sf::Vector2f(1200 - 270 + 80, 560+40));   // bottom left
    slopeRight.setPoint(3, sf::Vector2f(1200 - 248 + 80, 564+40));   // bottom left
    slopeRight.setPoint(4, sf::Vector2f(1200 - 224 + 80, 568+40));   // bottom left
    slopeRight.setPoint(5, sf::Vector2f(1200 - 200 + 80, 572+40));   // bottom left
    slopeRight.setPoint(6, sf::Vector2f(1200 + 80, 575+40));         // bottom right
    slopeRight.setFillColor(sf::Color(143, 163, 90)); // green

    // Variable to control the mode
    bool editMode = false;

    // Load font for button text
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // If Arial not found, try another common font
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf")) {
            return -1; // Cannot load font
        }
    }

    // Button text
    sf::Text buttonText(font);
    buttonText.setString("BUILD");
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);

    // Button padding
    const float paddingX = 20.0f; // Left and right padding
    const float paddingY = 10.0f; // Top and bottom padding

    // Calculate button size based on text bounds + padding
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    float buttonWidth = textBounds.size.x + 2 * paddingX;
    float buttonHeight = textBounds.size.y + 2 * paddingY + 10.0f; // Extra for descenders

    // Build mode button
    sf::RectangleShape buildButton({buttonWidth, buttonHeight});
    buildButton.setPosition({20.0f, 20.0f});
    buildButton.setFillColor(sf::Color(70, 130, 180)); // Steel blue
    buildButton.setOutlineThickness(2.0f);
    buildButton.setOutlineColor(sf::Color(30, 80, 130));

    // Position text with padding
    buttonText.setPosition({
        buildButton.getPosition().x + paddingX,
        buildButton.getPosition().y + paddingY
    });

    // Mouse hover state
    bool isButtonHovered = false;

    // Main game loop
    while (window.isOpen()) {
        // Get mouse position
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect buttonBounds = buildButton.getGlobalBounds();
        
        // Check if mouse is hovering over button
        isButtonHovered = buttonBounds.contains(sf::Vector2f(mousePos));
        
        // Update button appearance based on hover state
        if (isButtonHovered) {
            buildButton.setFillColor(sf::Color(100, 160, 210)); // Lighter blue on hover
        } else {
            if (editMode) {
                buildButton.setFillColor(sf::Color(50, 200, 50)); // Green when active
            } else {
                buildButton.setFillColor(sf::Color(70, 130, 180)); // Default blue
            }
        }
        
        // Update button text and size based on mode
        if (editMode) {
            buttonText.setString("Building...");
        } else {
            buttonText.setString("BUILD");
        }
        
        // Recalculate button size and position text with padding
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        float buttonWidth = textBounds.size.x + 2 * paddingX;
        float buttonHeight = textBounds.size.y + 2 * paddingY + 10.0f;
        buildButton.setSize({buttonWidth, buttonHeight});
        
        buttonText.setPosition({
            buildButton.getPosition().x + paddingX,
            buildButton.getPosition().y + paddingY
        });

        // Process events
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Handle mouse clicks
            if (event->is<sf::Event::MouseButtonPressed>()) {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                    if (isButtonHovered) {
                        editMode = !editMode; // Toggle build mode
                    }
                }
            }

            // Keep keyboard shortcut
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::E) {
                    editMode = !editMode;
                }
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
        window.draw(rect);

        // Draw UI button and text on top
        window.draw(buildButton);
        window.draw(buttonText);

        if (editMode) {
            // Define editable area (example: from x=150 to x=1050, y=400 to y=800)
            int gridLeft = 0, gridTop = 0, gridWidth = 1200, gridHeight = 560;
            int cellSize = 40;
            sf::Color gridColor(0, 0, 0, 60); // semi-transparent black

            // Vertical lines
            for (int x = gridLeft; x <= gridLeft + gridWidth; x += cellSize) {
                sf::Vertex line[2];
                line[0].position = sf::Vector2f((float)x, (float)gridTop);
                line[0].color = gridColor;
                line[1].position = sf::Vector2f((float)x, (float)(gridTop + gridHeight));
                line[1].color = gridColor;
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
            // Horizontal lines
            for (int y = gridTop; y <= gridTop + gridHeight; y += cellSize) {
                sf::Vertex line[2];
                line[0].position = sf::Vector2f((float)gridLeft, (float)y);
                line[0].color = gridColor;
                line[1].position = sf::Vector2f((float)(gridLeft + gridWidth), (float)y);
                line[1].color = gridColor;
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        // Display everything
        window.display();
    }

    return 0;
}
