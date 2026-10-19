#pragma once
#include "core/state.hpp"
#include <vector>
#include <string>
#include "ui/component.hpp"

class CircuitSelectState : public State
{

private:
    std::vector<std::string> trackFiles;
    ui::ComponentPtr root;

public:
    bool isPausable() const override { return true; }
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;
    CircuitSelectState();
};
