#include <SFML/Graphics.hpp>
#include "include/Button.h"
#include "include/Scene.h"
#include "include/Grid.h"

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    // Load font
    sf::Font font;
    if (!font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf")) {
        return -1;
    }

    // Create scene
    Scene scene;

    // Game state
    bool editMode = false;

    // Create buttons
    Button buildButton(font, "BUILD", {10.0f, 10.0f});
    buildButton.setCallback([&editMode, &buildButton]() {
        editMode = !editMode;
        buildButton.setText(editMode ? "PLAY" : "BUILD");
    });

    Button levelButton(font, "LEVEL", {10.0f + buildButton.getWidth() + 20.0f, 10.0f});
    // levelButton.setCallback([](){ /* TODO: Implementar seleção de levels */ });

    // Create grid
    std::vector<sf::Vector2f> gridBoundary = {
        {0.0f, 0.0f},
        {1200.0f, 0.0f},
        {1200.0f, 440.0f},
        {960.0f, 440.0f},
        {960.0f, 560.0f},
        {320.0f, 560.0f},
        {320.0f, 440.0f},
        {0.0f, 440.0f}
    };
    Grid grid(gridBoundary);

    // Main game loop
    while (window.isOpen()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Update buttons
        buildButton.update(mousePos);
        levelButton.update(mousePos);

        // Process events
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    buildButton.handleClick(mousePos);
                    levelButton.handleClick(mousePos);
                }
            }

            // Keyboard shortcut: 'B' to toggle build/play mode
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::B) {
                    editMode = !editMode;
                    buildButton.setText(editMode ? "PLAY" : "BUILD");
                }
            }
        }

        // Render
        window.clear(sf::Color(173, 216, 230)); // Sky blue

        scene.draw(window);

        if (editMode) {
            grid.draw(window);
        }

        buildButton.draw(window);
        levelButton.draw(window);

        window.display();
    }

    return 0;
}
