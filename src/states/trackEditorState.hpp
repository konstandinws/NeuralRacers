#pragma once
#include <string>
#include "core/state.hpp"
#include "raylib.h"
#include "editor/trackEditor.hpp"
#include "ui/component.hpp"

class TrackEditorState : public State
{
public:
    TrackEditorState();
    // while the popup is open, Esc closes it instead of opening the pause menu
    bool isPausable() const override { return !savePopupOpen; }
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;

private:
    void updateEditor(bool mouseAllowed);

    // saving: G or the SAVE button -> popup with the name -> CONFIRM
    void requestSave();
    void openSavePopup();
    void closeSavePopup();
    void confirmSave();
    void buildSavePopup();
    void showMessage(const std::string& text, Color color);

    Camera2D camera;
    TrackEditor editor;

    ui::ComponentPtr toolbar;
    ui::ComponentPtr savePopup;
    bool savePopupOpen = false;
    // the popup's callbacks cannot destroy it while it is running:
    // they set this flag and the popup is rebuilt at the start of the next frame (like a React re-render)
    bool savePopupDirty = false;
    std::string trackName;
    std::string saveError;

    // temporary message at the bottom of the screen
    std::string message;
    Color messageColor = WHITE;
    float messageTimer = 0.0f;
};
