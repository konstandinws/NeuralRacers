#include "trackEditorState.hpp"
#include "app.hpp"
#include "raygui.h"

TrackEditorState::TrackEditorState() {
    camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = { 0, 0 }; // L'origine de la caméra
}

void TrackEditorState::update(App* app) {

}

void TrackEditorState::render(App* app) {
    
}