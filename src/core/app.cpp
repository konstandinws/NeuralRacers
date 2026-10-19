#include "core/app.hpp"
#include "states/mainMenuState.hpp"
#include "raylib.h"
#include "states/pauseState.hpp"

App::App() : running(true) {
    InitWindow(1200, 800, "Neuro-Trackmania");
    SetExitKey(0); // eviter que le jeu se ferme avec la touche ESC
    SetTargetFPS(60);
    Renderer renderer;


    doPush(new MainMenuState());
}

App::~App() {
    // applique les actions restantes pour ne pas perdre (leak) les etats deja crees
    applyPendingActions();
    while (!stateStack.empty()) doPop();
    CloseWindow();
}

// les fonctions publiques ne modifient pas la pile directement, elles mettent l'action en attente

void App::pushState(State* state) {
    pendingActions.push_back([this, state]() { doPush(state); });
}

void App::popState() {
    pendingActions.push_back([this]() { doPop(); });
}

void App::clearAndPush(State* state) {
    pendingActions.push_back([this, state]() {
        while (!stateStack.empty()) doPop();
        doPush(state);
    });
}

void App::changeState(State* state) {
    pendingActions.push_back([this, state]() {
        doPop();
        doPush(state);
    });
}

void App::doPush(State* state) {
    stateStack.push_back(state);
    state->onEnter(this);
}

void App::doPop() {
    if (!stateStack.empty()) {
        delete stateStack.back();
        stateStack.pop_back();
    }
}

void App::applyPendingActions() {
    // on deplace la liste avant de l'executer au cas ou une action en ajoute une autre
    std::vector<std::function<void()>> actions = std::move(pendingActions);
    pendingActions.clear();
    for (auto& action : actions) {
        action();
    }
}

void App::run() {
    while (running && !WindowShouldClose()) {
        dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        if (!stateStack.empty()) {
            if (stateStack.back()->isPausable() && IsKeyPressed(KEY_ESCAPE)) {
                pushState(new PauseState());
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
