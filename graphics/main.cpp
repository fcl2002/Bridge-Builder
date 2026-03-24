#include <SFML/Graphics.hpp>
#include "include/Scene.h"
#include "include/HUD.h"

// Cost per wood pixel (can be fractional)
constexpr float WOOD_COST_PER_PIXEL = 0.5f;
#include <SFML/Graphics.hpp>
#include "include/Scene.h"
#include "include/HUD.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");
    constexpr float MAX_WOOD_SEGMENT_LENGTH = 80.0f;

    sf::Font font;
    if (!font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf"))
        return -1;

    Scene scene;

    // Game state variables
    bool simRunning = false;
    int currentLevel = 1;
    int budget = 1600;
    int score = 0;

    HUD hud(font, currentLevel, score, budget);

    // Sky gradient background
    sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
    sky[0].position = sf::Vector2f(   0,   0);  sky[1].position = sf::Vector2f(1200,   0);
    sky[2].position = sf::Vector2f(   0, 800);  sky[3].position = sf::Vector2f(1200, 800);
    sky[0].color = sky[1].color = sf::Color( 80, 193, 198);
    sky[2].color = sky[3].color = sf::Color(200, 235, 240);

    while (window.isOpen()) {
        sf::Vector2f mouseF(sf::Mouse::getPosition(window));

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto& me = *event->getIf<sf::Event::MouseButtonPressed>();
                if (me.button == sf::Mouse::Button::Left) {
                    const std::string action = hud.handleClick(mouseF);
                    if (action == "play") {
                        simRunning = !simRunning;
                        if (simRunning) {
                            scene.resetSimulation();
                        }
                    } else if (action == "reset") {
                        simRunning = false;
                        score  = 0;
                        budget = 1600;
                        scene.clearWoodSegments();
                        hud.update(score, budget);
                    }

                    // Start wood segment if tool is active and not clicking HUD
                    const bool hudCapturedClick = !action.empty() || hud.isPointInHUD(mouseF);
                    if (!hudCapturedClick && hud.isWoodModeActive()) {
                        scene.startWoodSegment(mouseF, MAX_WOOD_SEGMENT_LENGTH);
                    }
                }
            }
            

            if (event->is<sf::Event::MouseMoved>()) {
                if (scene.isBuildingWood()) {
                    scene.updateWoodSegmentPreview(mouseF);
                }
            }

            if (event->is<sf::Event::MouseButtonReleased>()) {
                auto& me = *event->getIf<sf::Event::MouseButtonReleased>();
                if (me.button == sf::Mouse::Button::Left && scene.isBuildingWood()) {
                    scene.updateWoodSegmentPreview(sf::Vector2f(static_cast<float>(me.position.x), static_cast<float>(me.position.y)));
                    int cost = scene.commitWoodSegment(budget, WOOD_COST_PER_PIXEL);
                    // Update HUD if segment was created and budget changed
                    if (cost > 0) {
                        hud.update(score, budget);
                    }
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                auto code = event->getIf<sf::Event::KeyPressed>()->code;
                if (code == sf::Keyboard::Key::Escape)
                    hud.handleKeyEscape();
            }

        }

        if (simRunning) {
            scene.simulateStep();
        }

        window.clear();
        window.draw(sky);
        scene.draw(window, simRunning);
        hud.draw(window, simRunning);
        window.display();
    }

    return 0;
}
