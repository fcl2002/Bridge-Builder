#pragma once
#include <memory>
#include "Scene.h"
#include "HUD.h"

inline int getInitialBudgetForLevel(int level) {
    switch (level) {
        case 1: return 1000;
        case 2: return 1200;
        case 3: return 1500;
        default: return 1000;
    }
}

class Level {
public:
    virtual ~Level() = default;
    virtual void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) = 0;
};

class Level1 : public Level {
public:
    void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) override;
};

class Level2 : public Level {
public:
    void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) override;
};

class Level3 : public Level {
public:
    void run(sf::RenderWindow& window, HUD& hud, int& score, int& budget, bool& simRunning, int& currentLevel) override;
};

