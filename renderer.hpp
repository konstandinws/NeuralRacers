#pragma once
#include "car.hpp"
#include "track.hpp"

class Renderer {
public:

    void drawTrack(Track* track);
    
    void drawGrid();

    void drawCar(Car* car);
    void showCarBindings();
    void showHUD(const Car& car, float raceTime, float endRaceTime);


};