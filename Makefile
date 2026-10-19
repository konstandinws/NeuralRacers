# -----------------------------------------------------
# Configuration
# -----------------------------------------------------

# Configuration locale propre a chaque machine (ignoree par git), voir config.mk.example
-include config.mk

SRC_DIR   = src
BUILD_DIR = build
OBJ_DIR   = $(BUILD_DIR)/obj
TARGET    = $(BUILD_DIR)/game

# Fichiers sources : src/*.cpp et src/<module>/*.cpp
SOURCES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS    = $(OBJECTS:.o=.d)

# Options de compilation
# -MMD -MP : genere les dependances vers les .hpp pour ne recompiler que ce qui a change
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -MMD -MP -I$(SRC_DIR) -isystem external/raygui

# Configuration par systeme (raylib, librairies, commandes)
ifeq ($(OS),Windows_NT)
    # pas d'emplacement standard sous Windows : RAYLIB_PATH doit etre donne dans config.mk
    TARGET     := $(TARGET).exe
    LIBS        = -lraylib -lopengl32 -lgdi32 -lwinmm
    MKDIR       = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    CLEAN       = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Darwin)
        # raylib installe avec Homebrew : brew install raylib
        RAYLIB_PATH ?= $(shell brew --prefix raylib 2>/dev/null)
        CXX          = clang++
        LIBS         = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
    else
        RAYLIB_PATH ?= /usr/local
        LIBS         = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    endif
    MKDIR = mkdir -p $(1)
    CLEAN = rm -rf $(BUILD_DIR)
endif

ifneq ($(MAKECMDGOALS),clean)
    ifeq ($(strip $(RAYLIB_PATH)),)
        $(error raylib introuvable. Copie config.mk.example en config.mk et renseigne RAYLIB_PATH, ou lance : make RAYLIB_PATH=/chemin/vers/raylib)
    endif
endif

CXXFLAGS += -isystem $(RAYLIB_PATH)/include
LDFLAGS   = -L$(RAYLIB_PATH)/lib

# -----------------------------------------------------
# Règles Make
# -----------------------------------------------------

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(call MKDIR,$(patsubst %/,%,$(dir $@)))
	$(CXX) $(CXXFLAGS) -c $< -o $@

# le jeu est lance depuis la racine pour trouver assets/
run: $(TARGET)
	./$(TARGET)

clean:
	$(CLEAN)

-include $(DEPS)
