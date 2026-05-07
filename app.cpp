#include "app.hpp"
#include "menuState.hpp"
#include "raylib.h"

App::App() : running(true) {
    InitWindow(1200, 800, "Neuro-Trackmania");
    SetTargetFPS(60);

    // On démarre le jeu en mettant le Menu tout en haut de la pile !
    pushState(new MenuState());
}

bool App::isRunning() {
    return running;
}

App::~App() {
    while (!stateStack.empty()) popState();
    CloseWindow();
}

void App::pushState(State* state) {
    stateStack.push_back(state);
}

void App::popState() {
    if (!stateStack.empty()) {
        delete stateStack.back(); 
        stateStack.pop_back();    
    }
}

void App::changeState(State* state) {
    popState();
    pushState(state);
}

void App::run() {
    while (running && !WindowShouldClose()) {
        if (!stateStack.empty()) {
            stateStack.back()->update(this);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        
        for (State* state : stateStack) {
            state->render(this);
        }

        EndDrawing();
    }
}