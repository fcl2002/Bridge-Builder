#pragma once
#include <memory>
#include "Scene.h"
#include "HUD.h"

class Level {
public:
    virtual ~Level() = default;
    virtual void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning) = 0;
};

class Level1 : public Level {
public:
    void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning) override;
};