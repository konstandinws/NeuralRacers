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

// pre-training screen: choose the track, the model (new or existing) and the number of cars
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
    // scrolling list of buttons with a single selected one
    ui::ComponentPtr selectList(const std::vector<std::string>& labels, int selected, std::function<void(int)> onSelect, float* scroll);
    void selectTrack(int index);
    void selectModel(int index);
    void launch(App* app);

    std::vector<std::string> trackFiles;
    int selectedTrack = -1;
    std::optional<Track> selectedTrackData;  // empty if the file is invalid
    std::string trackError;
    float trackScroll = 0.0f;  // keeps the lists' scroll position when the UI is rebuilt

    std::vector<std::string> modelNames;
    int selectedModel = -1;    // -1 = new model
    std::optional<SavedModel> selectedModelData;
    bool selectedModelAdapted = false;  // old model that had the speed inputs added
    std::string newModelName;
    std::string modelError;
    float modelScroll = 0.0f;

    int carCount = 50;

    ui::Root root;

    static constexpr int MIN_CARS = 2;   // the genetic algorithm needs 2 parents
    static constexpr int MAX_CARS = 300;
};
