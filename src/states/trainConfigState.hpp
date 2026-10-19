#pragma once
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include "core/state.hpp"
#include "io/modelSerializer.hpp"
#include "game/track.hpp"
#include "ui/component.hpp"
#include "ui/root.hpp"

// ecran avant l'entrainement : choix du circuit, du modele (nouveau ou existant) et du nombre de voitures
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
    // liste defilante de boutons dont un seul est selectionne
    ui::ComponentPtr selectList(const std::vector<std::string>& labels, int selected, std::function<void(int)> onSelect, float* scroll);
    void selectTrack(int index);
    void selectModel(int index);
    void launch(App* app);

    std::vector<std::string> trackFiles;
    int selectedTrack = -1;
    std::optional<Track> selectedTrackData;  // empty if the file is invalid
    std::string trackError;
    float trackScroll = 0.0f;  // garde la position des listes quand l'interface est reconstruite

    std::vector<std::string> modelNames;
    int selectedModel = -1;    // -1 = nouveau modele
    std::optional<SavedModel> selectedModelData;
    bool selectedModelAdapted = false;  // ancien modele auquel on a ajoute les entrees de vitesse
    std::string newModelName;
    std::string modelError;
    float modelScroll = 0.0f;

    int carCount = 50;

    ui::Root root;

    static constexpr int MIN_CARS = 2;   // l'algorithme genetique a besoin de 2 parents
    static constexpr int MAX_CARS = 300;
};
