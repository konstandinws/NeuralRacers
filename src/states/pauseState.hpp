#pragma once
#include "core/state.hpp"

class PauseState : public State
{
public:
    void update(App* app) override;
    void render(App* app) override;
};