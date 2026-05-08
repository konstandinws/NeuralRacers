#pragma once
#include "car.hpp"
#include "track.hpp"

class Renderer {
public:
    void drawCar(Car* car);
    void showCarInfos(Car* car);
    void drawTrack(Track* track);
    void drawGrid();

    void showCarBindings();

    void showHUD(const Car& car, float raceTime);
};