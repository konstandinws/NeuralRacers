# Neuro-Trackmania

Jeu de course 2D en C++ avec [raylib](https://www.raylib.com/), où des voitures pilotées par un réseau de neurones apprennent à finir un circuit par neuroévolution (algorithme génétique).

- **Jouer** : piloter soi-même sur un circuit et battre son meilleur temps.
- **Éditeur** : dessiner un circuit, placer le départ, l'arrivée et les checkpoints.
- **Entraîner** : une population de voitures IA apprend le circuit génération après génération.

## Compilation

Prérequis : un compilateur C++17 et raylib.

```bash
# macOS
brew install raylib

make        # compile dans build/
make run    # compile puis lance le jeu
make clean  # supprime build/
```

Sous Windows (MinGW), il faut indiquer où se trouve raylib :

1. Télécharger raylib (par ex. `raylib-5.5_win64_mingw-w64.zip`) depuis les [releases](https://github.com/raysan5/raylib/releases) et le décompresser.
2. Copier `config.mk.example` en `config.mk` et y mettre le chemin du dossier décompressé :
   ```make
   RAYLIB_PATH = C:/chemin/vers/raylib-5.5_win64_mingw-w64
   ```
3. Lancer `make run`.

`config.mk` est ignoré par git : chacun garde son propre chemin. On peut aussi passer le chemin directement : `make run RAYLIB_PATH=C:/chemin/vers/raylib`.

Le jeu doit être lancé depuis la racine du projet pour trouver `assets/`.

## Contrôles

| Mode | Touches |
|---|---|
| Jeu | Flèches : conduire · R : recommencer · Échap : pause |
| Éditeur | Clic : placer · WASD : caméra · Q/E : zoom · Espace : effacer · Retour arrière : annuler un checkpoint · G : sauvegarder |
| Entraînement | WASD : caméra · Q/E : zoom · Échap : pause |

## Structure

```
src/
  main.cpp
  core/      boucle du jeu, pile d'états, rendu, aléatoire
  states/    écrans : menu, pause, jeu, sélection, éditeur, entraînement
  game/      voiture, circuit, géométrie
  ai/        voiture IA (capteurs, fitness), réseau de neurones
  editor/    éditeur de circuit
  io/        lecture/écriture des circuits, listing des fichiers
external/    bibliothèques tierces (raygui)
assets/      circuits (.txt)
docs/        notes et TODO
```

## Format des circuits

Fichiers texte dans `assets/tracks/` :

```
S x,y                 départ
A angle               direction de départ (radians)
F x1,y1;x2,y2         ligne d'arrivée
C x1,y1;x2,y2         checkpoint (dans l'ordre de passage, optionnel)
x1,y1;x2,y2           segment de mur
<EOF>
```
