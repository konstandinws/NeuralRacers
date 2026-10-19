#pragma once
#include "game/car.hpp"
#include "game/track.hpp"
#include "ai/aiCar.hpp"

class Renderer {
public:

    void drawTrack(Track* track);

    void drawCar(Car* car);
    void showCarBindings();
    void showHUD(const Car& car, float raceTime, float endRaceTime);

    void drawAICars(const std::vector<AICar>& aiCars);

};