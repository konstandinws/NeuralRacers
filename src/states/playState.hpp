#pragma once

#include "core/state.hpp"
#include <string>
#include "game/track.hpp"
#include "game/car.hpp"
#include <limits>

class PlayState : public State {
public:
    PlayState(const std::string& trackName);
    ~PlayState() = default;

    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;

private:
    Car car;    
    Track track;
    Camera2D camera;
    float raceTime;
    float endRaceTime = std::numeric_limits<float>::max();
};