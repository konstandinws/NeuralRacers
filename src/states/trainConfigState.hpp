#pragma once
#include <string>
#include <vector>
#include "core/state.hpp"
#include "ui/component.hpp"
#include "ui/root.hpp"

// ecran avant l'entrainement : choix du circuit et du nombre de voitures
class TrainConfigState : public State
{
public:
    TrainConfigState();
    bool isPausable() const override { return true; }
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;

private:
    ui::ComponentPtr buildUI(App* app);
    void selectTrack(int index);

    std::vector<std::string> trackFiles;
    int selectedTrack = -1;
    int selectedCheckpoints = 0;
    int carCount = 50;
    float listScroll = 0.0f;  // garde la position de la liste quand l'interface est reconstruite

    ui::Root root;

    static constexpr int MIN_CARS = 10;
    static constexpr int MAX_CARS = 300;
    static constexpr int CAR_STEP = 10;
};
