#pragma once
#include "core/state.hpp"
#include <vector>
#include <string>
#include "ui/component.hpp"
#include "ui/root.hpp"

class CircuitSelectState : public State
{

private:
    std::vector<std::string> trackFiles;
    std::string loadError;   // shown under the list when the chosen track cannot be loaded
    float listScroll = 0.0f; // kept across rebuilds

    ui::Root root;
    ui::ComponentPtr buildUI(App* app);
    void play(App* app, const std::string& file);

public:
    bool isPausable() const override { return true; }
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;
    CircuitSelectState();
};
