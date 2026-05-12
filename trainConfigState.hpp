#pragma once
#include "state.hpp"
#include "AICar.hpp"
#include <vector>

class TrainConfigState : public State
{
public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
    void createAICars(int numCars);
    TrainConfigState();

private:
    std::vector<AICar> aiCars;
    Track track;
    Camera2D camera;
};