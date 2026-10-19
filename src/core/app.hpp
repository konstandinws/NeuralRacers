#pragma once
#include <memory>
#include <vector>
#include "core/state.hpp"
#include "core/renderer.hpp"

class App {
private:
    // the stack owns its states: popping one destroys it
    std::vector<std::unique_ptr<State>> stateStack;
    bool running;

    // State changes requested during update/render are applied after EndDrawing,
    // otherwise a state could destroy itself while its own code is still running.
    // A plain struct rather than lambdas: std::function cannot hold a move-only unique_ptr.
    struct PendingAction {
        enum class Type { Push, Pop, Change, ClearAndPush };
        Type type;
        std::unique_ptr<State> state;  // empty for Pop
    };
    std::vector<PendingAction> pendingActions;

    void applyPendingActions();
    void doPush(std::unique_ptr<State> state);
    void doPop();

public:
    const int W = 1200;
    const int H = 800;
    Renderer renderer;
    float dt;

    App();
    ~App();

    void run();
    void quit() { running = false; }

    // usage: app->changeState(std::make_unique<MainMenuState>());
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);
    void clearAndPush(std::unique_ptr<State> state);
};
