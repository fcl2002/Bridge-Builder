#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <algorithm>

// Check if a point is inside a polygon using ray casting algorithm
bool isPointInPolygon(const sf::Vector2f& point, const std::vector<sf::Vector2f>& polygon) {
    int intersections = 0;
    size_t n = polygon.size();
    
    for (size_t i = 0; i < n; i++) {
        sf::Vector2f p1 = polygon[i];
        sf::Vector2f p2 = polygon[(i + 1) % n];
        
        // Check if ray from point going right intersects edge
        if ((p1.y > point.y) != (p2.y > point.y)) {
            float xIntersection = (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x;
            if (point.x < xIntersection) {
                intersections++;
            }
        }
    }
    
    return (intersections % 2) == 1;
}

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
    slopeLeft.setPoint(0, sf::Vector2f(  0, 400+40));   // top left
    slopeLeft.setPoint(1, sf::Vector2f(150, 400+40));   // top right
    slopeLeft.setPoint(2, sf::Vector2f(270, 560+40));   // bottom right
    slopeLeft.setPoint(3, sf::Vector2f(248, 564+40));   // bottom right
    slopeLeft.setPoint(4, sf::Vector2f(224, 568+40));   // bottom right
    slopeLeft.setPoint(5, sf::Vector2f(200, 572+40));   // bottom right
    slopeLeft.setPoint(6, sf::Vector2f(  0, 575+40));   // bottom left
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

    // Game mode: false = BUILD mode (shows grid), true = PLAY mode (runs simulation)
    bool editMode = false;

    // Manual grid boundary definition - Define your polygon points here
    // You can define any polygon shape, just like slopeRight, river, etc.
    std::vector<sf::Vector2f> gridBoundaryPoints = {
        {0.0f, 0.0f},      // Point 1 - top left
        {1200.0f, 0.0f},   // Point 2
        {1200.0f, 440.0f}, // Point 3
        {960.0f, 440.0f},  // Point 4
        {960.0f, 560.0f},  // Point 5
        {320.0f, 560.0f},  // Point 6
        {320.0f, 440.0f},  // Point 7
        {0.0f, 440.0f},    // Point 8
    };
    
    // Calculate grid rectangular bounds from the 8 points
    float minX = gridBoundaryPoints[0].x, maxX = gridBoundaryPoints[0].x;
    float minY = gridBoundaryPoints[0].y, maxY = gridBoundaryPoints[0].y;
    for (const auto& point : gridBoundaryPoints) {
        if (point.x < minX) minX = point.x;
        if (point.x > maxX) maxX = point.x;
        if (point.y < minY) minY = point.y;
        if (point.y > maxY) maxY = point.y;
    }
    int gridLeft = (int)minX;
    int gridTop = (int)minY;
    int gridWidth = (int)(maxX - minX);
    int gridHeight = (int)(maxY - minY);

    // Load pixel font for retro game style
    sf::Font font;
    if (!font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf")) {
        return -1; // Cannot load font
    }

    // Button text with pixel font styling
    sf::Text buttonText(font);
    buttonText.setString("BUILD");
    buttonText.setCharacterSize(18); // Smaller size for pixel fonts
    buttonText.setFillColor(sf::Color::White);

    // Button padding
    const float paddingX = 15.0f; // Left and right padding
    const float paddingY = 10.0f; // Top and bottom padding

    // Calculate button size based on text bounds + padding
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    float buttonWidth = textBounds.size.x + 2 * paddingX;
    float buttonHeight = textBounds.size.y + 2 * paddingY + 10.0f; // Extra for descenders

    // Build mode button
    sf::RectangleShape buildButton({buttonWidth, buttonHeight});
    buildButton.setPosition({40.0f, 10.0f});
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
            buildButton.setFillColor(sf::Color(23, 61, 99)); // Lighter blue on hover
        } else {
            buildButton.setFillColor(sf::Color(70, 130, 180)); // Default blue
        }
        
        // Update button text and size based on mode
        if (editMode) {
            buttonText.setString("PLAY");
        } else {
            buttonText.setString("BUILD");
        }
        
        // Recalculate button size and position text with padding
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        float buttonWidth = textBounds.size.x + 2 * paddingX;
        float buttonHeight = textBounds.size.y + 2 * paddingY;
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
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (isButtonHovered) {
                        editMode = !editMode; // Toggle build mode
                    }
                }
            }

            // Keyboard shortcut: 'B' to toggle build/play mode
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::B) {
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

        if (editMode) {
            int cellSize = 40;
            sf::Color gridColor(0, 0, 0, 80); // semi-transparent black

            // Draw grid lines only within the polygon
            // Vertical lines
            for (int x = gridLeft; x <= gridLeft + gridWidth; x += cellSize) {
                // Collect all intersection points of this vertical line with polygon
                std::vector<float> intersectionY;
                
                for (size_t i = 0; i < gridBoundaryPoints.size(); i++) {
                    sf::Vector2f p1 = gridBoundaryPoints[i];
                    sf::Vector2f p2 = gridBoundaryPoints[(i + 1) % gridBoundaryPoints.size()];
                    
                    // Check if vertical line x intersects edge p1-p2
                    if ((p1.x <= x && p2.x >= x) || (p1.x >= x && p2.x <= x)) {
                        if (std::abs(p2.x - p1.x) > 0.001f) {
                            float y = p1.y + (p2.y - p1.y) * (x - p1.x) / (p2.x - p1.x);
                            intersectionY.push_back(y);
                        }
                    }
                }
                
                // Sort intersections and draw line segments
                std::sort(intersectionY.begin(), intersectionY.end());
                for (size_t i = 0; i + 1 < intersectionY.size(); i += 2) {
                    sf::Vertex line[2];
                    line[0].position = sf::Vector2f((float)x, intersectionY[i]);
                    line[0].color = gridColor;
                    line[1].position = sf::Vector2f((float)x, intersectionY[i + 1]);
                    line[1].color = gridColor;
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
            
            // Horizontal lines
            for (int y = gridTop; y <= gridTop + gridHeight; y += cellSize) {
                // Collect all intersection points of this horizontal line with polygon
                std::vector<float> intersectionX;
                
                for (size_t i = 0; i < gridBoundaryPoints.size(); i++) {
                    sf::Vector2f p1 = gridBoundaryPoints[i];
                    sf::Vector2f p2 = gridBoundaryPoints[(i + 1) % gridBoundaryPoints.size()];
                    
                    // Check if horizontal line y intersects edge p1-p2
                    if ((p1.y <= y && p2.y >= y) || (p1.y >= y && p2.y <= y)) {
                        if (std::abs(p2.y - p1.y) > 0.001f) {
                            float x = p1.x + (p2.x - p1.x) * (y - p1.y) / (p2.y - p1.y);
                            intersectionX.push_back(x);
                        }
                    }
                }
                
                // Sort intersections and draw line segments
                std::sort(intersectionX.begin(), intersectionX.end());
                for (size_t i = 0; i + 1 < intersectionX.size(); i += 2) {
                    sf::Vertex line[2];
                    line[0].position = sf::Vector2f(intersectionX[i], (float)y);
                    line[0].color = gridColor;
                    line[1].position = sf::Vector2f(intersectionX[i + 1], (float)y);
                    line[1].color = gridColor;
                    window.draw(line, 2, sf::PrimitiveType::Lines);
                }
            }
            
            // Draw the boundary outline
            if (gridBoundaryPoints.size() >= 3) {
                for (size_t i = 0; i < gridBoundaryPoints.size(); i++) {
                    size_t next = (i + 1) % gridBoundaryPoints.size();
                    sf::Vertex boundaryLine[2];
                    boundaryLine[0].position = gridBoundaryPoints[i];
                    boundaryLine[0].color = sf::Color(0, 200, 0, 255); // Bright green
                    boundaryLine[1].position = gridBoundaryPoints[next];
                    boundaryLine[1].color = sf::Color(0, 200, 0, 255);
                    window.draw(boundaryLine, 2, sf::PrimitiveType::Lines);
                }
            }
        }

        // Draw UI button and text on top
        window.draw(buildButton);
        window.draw(buttonText);

        // Display everything
        window.display();
    }

    return 0;
}
