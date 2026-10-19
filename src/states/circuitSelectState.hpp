#pragma once
#include "core/state.hpp"
#include <vector>
#include <string>
#include "raylib.h"

class CircuitSelectState : public State
{

private: 
    std::vector<std::string> trackFiles; 
    Vector2 scrollOffset;

public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
    CircuitSelectState();
};