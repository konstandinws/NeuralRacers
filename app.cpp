#include "app.hpp"
#include "mainMenuState.hpp"
#include "raylib.h"
#include "pauseState.hpp"

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
        ClearBackground(BLACK);
        if (!stateStack.empty())
            if(stateStack.back()-> isPausable() && IsKeyPressed(KEY_ESCAPE)) {
                pushState(new PauseState());
            }
            else{
                stateStack.back()->update(this);
                stateStack.back()->render(this);
            }

        BeginDrawing();
        
        // bug probable car je change d'etat en etant dans la boucle quand j'appuie sur ESC
        // car je supprime tous les elements de boucle mais continue a appeler render, parfois ca marche surement une UB
        // un moyen de fixe et d'utiliser des lambdas pour faire les changements d'etats apres la boucle de rendu 

        // for (State* state : stateStack)
        //     state->render(this);

        EndDrawing();

    }
}
