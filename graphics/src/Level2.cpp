#include "../include/Level.h"
#include <iostream>
#include <memory>

void Level2::run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) {
    constexpr float MAX_WOOD_SEGMENT_LENGTH = 80.0f;
    // Do not draw the car in Level 2
    Scene scene(false);

    sf::VertexArray sky(sf::PrimitiveType::TriangleStrip, 4);
    sky[0].position = sf::Vector2f(0, 0);
    sky[1].position = sf::Vector2f(1200, 0);
    sky[2].position = sf::Vector2f(0, 800);
    sky[3].position = sf::Vector2f(1200, 800);
    sky[0].color = sky[1].color = sf::Color(80, 193, 198);
    sky[2].color = sky[3].color = sf::Color(200, 235, 240);

    sf::Texture truckTexture;
    std::unique_ptr<sf::Sprite> truckSprite;
    bool truckLoaded = false;
    if (truckTexture.loadFromFile("assets/icons/truck.png")) {
        std::cout << "[OK] Loaded truck sprite: assets/icons/truck.png\n";
        truckLoaded = true;
    } else if (truckTexture.loadFromFile("graphics/assets/icons/truck.png")) {
        std::cout << "[OK] Loaded truck sprite: graphics/assets/icons/truck.png\n";
        truckLoaded = true;
    } else {
        std::cout << "[ERROR] Failed to load truck sprite: assets/icons/truck.png\n";
    }

    if (truckLoaded) {
        truckSprite = std::make_unique<sf::Sprite>(truckTexture);
        // Set truck size and position directly (pixel-perfect)
        const float targetWidth = 144.0f;
        const float targetHeight = 72.0f;
        const auto size = truckTexture.getSize();
        float scaleX = targetWidth / static_cast<float>(size.x);
        float scaleY = targetHeight / static_cast<float>(size.y);
        truckSprite->setScale(sf::Vector2f(scaleX, scaleY));
        // Place the truck at a fixed pixel position
        truckSprite->setPosition({30.0f, 368.0f});
    }

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
                    } else if (action == "reset") {
                        simRunning = false;
                        score = 0;
                        budget = 1000;
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

        window.clear();
        window.draw(sky);
        scene.draw(window); // no car in Level 2, only the truck
        if (truckSprite) {
            window.draw(*truckSprite);
        }
        // Extra red dot (anchor node) below the left bridge
        sf::CircleShape extraRedDot(4.0f); // radius 4
        extraRedDot.setOrigin(sf::Vector2f(4.0f, 4.0f)); // center origin
        extraRedDot.setPosition(sf::Vector2f(343.0f, 480.0f)); // slightly below the default (343, 440)
        extraRedDot.setFillColor(sf::Color(210, 30, 30));
        window.draw(extraRedDot);
        // Extra red dot (anchor node) below the right bridge
        sf::CircleShape extraRedDotRight(4.0f); // radius 4
        extraRedDotRight.setOrigin(sf::Vector2f(4.0f, 4.0f)); // center origin
        extraRedDotRight.setPosition(sf::Vector2f(937.0f, 480.0f)); // slightly below the default (937, 440)
        extraRedDotRight.setFillColor(sf::Color(210, 30, 30));
        window.draw(extraRedDotRight);
        hud.draw(window, simRunning);
        window.display();
    }
}
