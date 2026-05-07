#pragma once
#include <vector>
#include "state.hpp"
#include "car.hpp"
#include "track.hpp"
#include "trackEditor.hpp"

class App {
private:
    std::vector<State*> stateStack;
    bool running;

public:
    const int W = 1200;
    const int H = 800;
    
    Car car;
    Track track;
    TrackEditor editor;
    Camera2D camera;

    App();
    ~App();

    void run();
    void quit() { running = false; }
    bool isRunning();

    void pushState(State* state);
    void popState();
    void changeState(State* state);
};