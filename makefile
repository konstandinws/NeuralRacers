# -----------------------------------------------------
# Configuration
# -----------------------------------------------------

# Nom de ton projet
TARGET = game

# Fichiers sources
SOURCES = main.cpp

# Répertoire d'installation de raylib
RAYLIB_PATH = C:\Users\Konstantinos\Desktop\raylib-5.5_win64_mingw-w64

# Chemins include et lib
INCLUDE_PATHS = -I$(RAYLIB_PATH)/include
LIBRARY_PATHS = -L$(RAYLIB_PATH)/lib

# Librairies à linker
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Options de compilation
CFLAGS = -std=c++17

# -----------------------------------------------------
# Règles Make
# -----------------------------------------------------

$(TARGET): $(SOURCES)
	g++ $(SOURCES) $(CFLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBS) -o $(TARGET).exe

run: $(TARGET)
	./$(TARGET).exe

clean:
	del $(TARGET).exe
