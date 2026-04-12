#include "../include/Level.h"

void Level2::run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) {
    constexpr float MAX_WOOD_SEGMENT_LENGTH = 80.0f;
    Scene scene(false, true); // Car disabled, truck enabled
    scene.addFixedSupportNode(sf::Vector2f(420.0f, 480.0f));
    scene.addFixedSupportNode(sf::Vector2f(850.0f, 480.0f));

    sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
    sky[0].position = sf::Vector2f(0, 0);
    sky[1].position = sf::Vector2f(1200, 0);
    sky[2].position = sf::Vector2f(0, 800);
    sky[3].position = sf::Vector2f(1200, 800);
    sky[0].color = sky[1].color = sf::Color(80, 193, 198);
    sky[2].color = sky[3].color = sf::Color(200, 235, 240);

    while (window.isOpen()) {
        sf::Vector2f mouseF(sf::Mouse::getPosition(window));
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

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
                        score = 0;
                        budget = getInitialBudgetForLevel(currentLevel);
                        scene.clearWoodSegments();
                        hud.update(score, budget);
                    } else if (action == "level_1") {
                        simRunning = false;
                        currentLevel = 1;
                        return;
                    } else if (action == "level_2") {
                        simRunning = false;
                        currentLevel = 2;
                        return;
                    } else if (action == "level_3") {
                        simRunning = false;
                        currentLevel = 3;
                        return;
                    } else if (action == "level_4") {
                        simRunning = false;
                        currentLevel = 4;
                        return;
                    }

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
                    int cost = scene.commitWoodSegment(budget, 0.5f);
                    if (cost > 0) {
                        hud.update(score, budget);
                    }
                }
            }

            if (event->is<sf::Event::KeyPressed>()) {
                auto code = event->getIf<sf::Event::KeyPressed>()->code;
                if (code == sf::Keyboard::Key::Escape) {
                    hud.handleKeyEscape();
                }
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
}
