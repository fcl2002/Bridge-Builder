#include <SFML/Graphics.hpp>
#include "include/Scene.h"
#include "include/HUD.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Bridge Builder - Game");

    sf::Font font;
    if (!font.openFromFile("assets/Press_Start_2P/PressStart2P-Regular.ttf"))
        return -1;

    Scene scene;

    // Game state
    bool simRunning   = false;
    int  currentLevel = 1;
    int  budget       = 1000;
    int  score        = 0;

    HUD hud(font, currentLevel, score, budget);

    // Sky gradient
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
                    std::string action = hud.handleClick(mouseF);
                    if (action == "play") {
                        simRunning = !simRunning;
                    } else if (action == "reset") {
                        simRunning = false;
                        score  = 0;
                        budget = 1000;
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

        window.clear();
        window.draw(sky);
        scene.draw(window);
        hud.draw(window, simRunning);
        window.display();
    }

    return 0;
}
