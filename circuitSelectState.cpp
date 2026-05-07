#include "circuitSelectState.hpp"
#include "app.hpp"
#include "raylib.h"
#include "raygui.h"
#include "pauseState.hpp"
#include "fileLoader.hpp"
#include "mainMenuState.hpp"
#include <iostream>

CircuitSelectState::CircuitSelectState() {
    scrollOffset = { 0, 0 }; 
    
    std::string folderPath = "Tracks";
    
    loadFilesNameFromFolder(folderPath, trackFiles);
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
            
            std::cout << "Lancement du circuit : " << trackFiles[i] << std::endl;
        }
    }

    EndScissorMode();

    if (GuiButton(Rectangle{400, 680, 200, 50}, "RETOUR")) {
        app->changeState(new MainMenuState());
    }
}   