#pragma once
#include "state.hpp"
#include "track.hpp"
#include "raylib.h"

enum class EditorTool {
    DRAW_WALLS,
    PLACE_START,
    PLACE_FINISH
};

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
    EditorTool currentTool = EditorTool::DRAW_WALLS;
};