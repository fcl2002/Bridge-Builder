#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color defaultColor;
    sf::Color hoverColor;
    sf::Color outlineColor;
    bool isHovered;
    std::function<void()> onClick;

    // Padding constants
    static constexpr float PADDING_X = 15.0f;
    static constexpr float PADDING_Y = 10.0f;

public:
    Button(const sf::Font& font, const std::string& label, sf::Vector2f position,
           sf::Color defaultColor = sf::Color(70, 130, 180),
           sf::Color hoverColor = sf::Color(23, 61, 99));

    void setCallback(std::function<void()> callback);
    void setText(const std::string& newText);
    void update(const sf::Vector2i& mousePos);
    void handleClick(const sf::Vector2i& mousePos);
    void draw(sf::RenderWindow& window);
    
    sf::FloatRect getBounds() const;
    float getWidth() const;
};
