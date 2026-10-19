#pragma once
#include "raylib.h"

// couleurs communes a toute l'interface, a modifier ici pour changer le style partout
namespace ui::theme {

inline constexpr Color surface      = {35, 35, 45, 255};
inline constexpr Color surfaceHover = {55, 55, 70, 255};
inline constexpr Color surfaceDark  = {20, 20, 26, 255};
inline constexpr Color border       = {90, 90, 110, 255};
inline constexpr Color accent       = {230, 41, 55, 255};  // meme rouge que les voitures
inline constexpr Color text         = {245, 245, 245, 255};

}
