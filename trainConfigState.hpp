#pragma once
#include "state.hpp"

class TrainConfigState : public State
{
public:
    void update(App* app) override;
    void render(App* app) override;
};