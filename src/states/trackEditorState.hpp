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
    // pendant le popup, Echap le ferme au lieu d'ouvrir la pause
    bool isPausable() const override { return !savePopupOpen; }
    void onEnter(App* app) override;
    void update(App* app) override;
    void render(App* app) override;

private:
    void updateEditor(bool mouseAllowed);

    // sauvegarde : G ou bouton ENREGISTRER -> popup avec le nom -> VALIDER
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
    // les callbacks du popup ne peuvent pas le detruire pendant qu'il s'execute :
    // ils levent ce drapeau et le popup est reconstruit au debut de la frame suivante (comme un re-render React)
    bool savePopupDirty = false;
    std::string trackName;
    std::string saveError;

    // message temporaire en bas de l'ecran
    std::string message;
    Color messageColor = WHITE;
    float messageTimer = 0.0f;
};
