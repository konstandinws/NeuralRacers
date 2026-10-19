#include "states/circuitSelectState.hpp"
#include "core/app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "states/pauseState.hpp"
#include "io/fileLoader.hpp"
#include "states/mainMenuState.hpp"
#include <iostream>
#include "states/playState.hpp"
#include "io/trackSerializer.hpp"

CircuitSelectState::CircuitSelectState() {
    scrollOffset = { 0, 0 }; 
    
    loadFilesNameFromFolder(TRACKS_DIR, trackFiles);
}


void CircuitSelectState::update(App* app)
{

}

void CircuitSelectState::render(App* app)
{
    DrawText("SELECT YOUR CIRCUIT", 400, 50, 40, RAYWHITE);

    Rectangle panelBounds = { 400, 150, 400, 500 }; 
    
    float buttonHeight = 50.0f;
    float spacing = 10.0f;
    
    float totalContentHeight = trackFiles.size() * (buttonHeight + spacing);
    Rectangle contentBounds = { 0, 0, panelBounds.width - 20, totalContentHeight };
    
    Rectangle view; 

    GuiScrollPanel(panelBounds, NULL, contentBounds, &scrollOffset, &view);

    BeginScissorMode(view.x, view.y, view.width, view.height);

    for (size_t i = 0; i < trackFiles.size(); ++i) {
        float btnY = panelBounds.y + (i * (buttonHeight + spacing)) + scrollOffset.y;
        
        Rectangle btnBounds = { panelBounds.x + 10, btnY, view.width - 20, buttonHeight };

        if (GuiButton(btnBounds, trackFiles[i].c_str())) {
            app->changeState(new PlayState(TRACKS_DIR + trackFiles[i]));
        }
    }

    EndScissorMode();

    if (GuiButton(Rectangle{400, 680, 200, 50}, "RETOUR")) {
        app->changeState(new MainMenuState());
    }
}   