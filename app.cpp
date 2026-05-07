#include "app.hpp"
#include "mainMenuState.hpp"
#include "raylib.h"

App::App() : running(true) {
    InitWindow(1200, 800, "Neuro-Trackmania");
    SetExitKey(0); // eviter que le jeu se ferme avec la touche ESC
    SetTargetFPS(60);

    
    pushState(new MainMenuState());
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

void App::clearAndPush(State* state) {
    while (!stateStack.empty()) popState();
    pushState(state);
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