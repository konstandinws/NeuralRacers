#pragma once

#include "state.hpp"
#include <string>
#include "track.hpp"
#include "car.hpp"

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
};