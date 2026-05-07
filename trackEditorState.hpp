#pragma once
#include "state.hpp"

class TrackEditorState : public State
{
public:
    bool isPausable() const override { return true; }
    void update(App* app) override;
    void render(App* app) override;
};