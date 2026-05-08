#pragma once
#include "state.hpp"
#include "track.hpp"
#include "raylib.h"
#include "trackEditor.hpp"

class TrackEditorState : public State
{
public:
    TrackEditorState();
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;

private:  
    Track track;
    Camera2D camera;
    TrackEditor editor;
};