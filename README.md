# Neural Racers

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus)
![raylib](https://img.shields.io/badge/raylib-5.5-black)
![Platforms](https://img.shields.io/badge/platforms-macOS%20%7C%20Linux%20%7C%20Windows-lightgrey)

A 2D racing game where AI-driven cars teach themselves to complete a track.

<p align="center">
  <img src="docs/media/training.gif" alt="AI training: generation 1, then generation 28" width="720">
</p>

At first, the cars drive randomly and crash into the first turn. Generation after generation, the best ones pass on what they learned, until the whole population can finish the track.

Everything is written from scratch in C++ with [raylib](https://www.raylib.com/): the neural network, the genetic algorithm, the physics and the UI. No AI library.

## Features

- **Train an AI** on any track, with up to 300 cars per generation, and watch what they "see" in real time.
- **Resume a trained model**, on the same track or a new one.
- **Draw your own tracks** in the built-in editor.
- **Take the wheel** and try to beat your best time.

## Getting started

You need a C++20 compiler, `make` and raylib 5.5.

**macOS**
```bash
brew install raylib
make run
```

**Linux** (raylib installed in `/usr/local`)
```bash
make run
```

**Windows (MinGW)**
1. Download `raylib-5.5_win64_mingw-w64.zip` from the [raylib releases](https://github.com/raysan5/raylib/releases) and extract it.
2. Copy `config.mk.example` to `config.mk` and set the folder path:
   ```make
   RAYLIB_PATH = C:/path/to/raylib-5.5_win64_mingw-w64
   ```
3. Run `make run`.

## Controls

| Screen | Controls |
|---|---|
| Game | `↑` `↓` `←` `→` drive · `R` restart · `Esc` pause |
| Editor | click to place · `W` `A` `S` `D` camera · `Q` / `E` zoom · `G` save |
| Training | `W` `A` `S` `D` camera · `Q` / `E` zoom · `Esc` pause |

## Coming next

- Faster training, without waiting for rendering.
- Race against the best AI, shown as a ghost.
- Edit an existing track in the editor.
