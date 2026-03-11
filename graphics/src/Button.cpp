#include "../include/Button.h"

Button::Button(const sf::Font& font, const std::string& label, sf::Vector2f position,
               sf::Color defaultColor, sf::Color hoverColor)
    : text(font), defaultColor(defaultColor), hoverColor(hoverColor), isHovered(false) {
    
    // Setup text
    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    // Calculate button size based on text
    sf::FloatRect textBounds = text.getLocalBounds();
    float buttonWidth = textBounds.size.x + 2 * PADDING_X;
    float buttonHeight = textBounds.size.y + 2 * PADDING_Y + 10.0f;

    // Setup shape
    shape.setSize({buttonWidth, buttonHeight});
    shape.setPosition(position);
    shape.setFillColor(defaultColor);
    shape.setOutlineThickness(2.0f);
    outlineColor = sf::Color(defaultColor.r - 40, defaultColor.g - 50, defaultColor.b - 50);
    shape.setOutlineColor(outlineColor);

    // Position text with padding
    text.setPosition({
        position.x + PADDING_X,
        position.y + PADDING_Y
    });
}

void Button::setCallback(std::function<void()> callback) {
    onClick = callback;
}

void Button::setText(const std::string& newText) {
    text.setString(newText);
    
    // Recalculate size
    sf::FloatRect textBounds = text.getLocalBounds();
    float buttonWidth = textBounds.size.x + 2 * PADDING_X;
    float buttonHeight = textBounds.size.y + 2 * PADDING_Y + 10.0f;
    shape.setSize({buttonWidth, buttonHeight});
    
    // Reposition text
    text.setPosition({
        shape.getPosition().x + PADDING_X,
        shape.getPosition().y + PADDING_Y
    });
}

void Button::update(const sf::Vector2i& mousePos) {
    sf::FloatRect bounds = shape.getGlobalBounds();
    isHovered = bounds.contains(sf::Vector2f(mousePos));
    
    if (isHovered) {
        shape.setFillColor(hoverColor);
    } else {
        shape.setFillColor(defaultColor);
    }
}

void Button::handleClick(const sf::Vector2i& mousePos) {
    if (isHovered && onClick) {
        onClick();
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

sf::FloatRect Button::getBounds() const {
    return shape.getGlobalBounds();
}

float Button::getWidth() const {
    return shape.getSize().x;
}
