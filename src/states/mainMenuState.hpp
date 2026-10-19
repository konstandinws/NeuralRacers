#pragma once
#include "core/state.hpp"
#include "ui/component.hpp"

class MainMenuState : public State
{
public:
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;

private:
    ui::ComponentPtr root;
};
