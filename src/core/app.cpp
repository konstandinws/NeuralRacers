#include "core/app.hpp"
#include "states/mainMenuState.hpp"
#include "raylib.h"
#include "states/pauseState.hpp"

App::App() : running(true) {
    InitWindow(1200, 800, "Neuro-Trackmania");
    SetExitKey(0); // eviter que le jeu se ferme avec la touche ESC
    SetTargetFPS(60);
    Renderer renderer;


    doPush(std::make_unique<MainMenuState>());
}

App::~App() {
    // the states are destroyed automatically, before the window (they may still use raylib resources)
    pendingActions.clear();
    stateStack.clear();
    CloseWindow();
}

// the public functions do not touch the stack directly, they queue the change

void App::pushState(std::unique_ptr<State> state) {
    pendingActions.push_back({PendingAction::Type::Push, std::move(state)});
}

void App::popState() {
    pendingActions.push_back({PendingAction::Type::Pop, nullptr});
}

void App::clearAndPush(std::unique_ptr<State> state) {
    pendingActions.push_back({PendingAction::Type::ClearAndPush, std::move(state)});
}

void App::changeState(std::unique_ptr<State> state) {
    pendingActions.push_back({PendingAction::Type::Change, std::move(state)});
}

void App::doPush(std::unique_ptr<State> state) {
    stateStack.push_back(std::move(state));
    stateStack.back()->onEnter(this);
}

void App::doPop() {
    if (!stateStack.empty()) {
        stateStack.pop_back();
    }
}

void App::applyPendingActions() {
    // move the queue out first, in case an action (e.g. onEnter) queues another one
    std::vector<PendingAction> actions = std::move(pendingActions);
    pendingActions.clear();

    for (PendingAction& action : actions) {
        switch (action.type) {
            case PendingAction::Type::Push:
                doPush(std::move(action.state));
                break;
            case PendingAction::Type::Pop:
                doPop();
                break;
            case PendingAction::Type::Change:
                doPop();
                doPush(std::move(action.state));
                break;
            case PendingAction::Type::ClearAndPush:
                stateStack.clear();
                doPush(std::move(action.state));
                break;
        }
    }
}

void App::run() {
    while (running && !WindowShouldClose()) {
        dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        if (!stateStack.empty()) {
            if (stateStack.back()->isPausable() && IsKeyPressed(KEY_ESCAPE)) {
                pushState(std::make_unique<PauseState>());
            } else {
                stateStack.back()->update(this);
                stateStack.back()->render(this);
            }
        }
        EndDrawing();

        // tous les etats ont fini de s'executer pour cette frame, on peut modifier la pile sans risque
        applyPendingActions();
    }
}
