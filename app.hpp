#pragma once
#include <vector>
#include <functional>
#include "state.hpp"
#include "car.hpp"
#include "track.hpp"
#include "trackEditor.hpp"
#include "renderer.hpp"

class App {
private:
    std::vector<State*> stateStack;
    bool running;

    // les changements d'etat demandes pendant update/render sont appliques apres EndDrawing,
    // sinon un etat peut se supprimer lui-meme (delete this) pendant qu'il s'execute encore
    std::vector<std::function<void()>> pendingActions;

    void applyPendingActions();
    void doPush(State* state);
    void doPop();


public:
    const int W = 1200;
    const int H = 800;
    Renderer renderer;
    float dt;

    // Car car;
    // Track track;
    // TrackEditor editor;
    // Camera2D camera;

    App();
    ~App();



    void run();
    void quit() { running = false; }

    void pushState(State* state);
    void popState();
    void changeState(State* state);
    void clearAndPush(State* state);
};