#include "io/fileLoader.hpp"
#include <filesystem>
#include <algorithm>



void loadFilesNameFromFolder(const std::string& folderPath, std::vector<std::string>& filesNames) {
    if (std::filesystem::exists(folderPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.path().extension() == ".txt") {
                filesNames.push_back(entry.path().filename().string());
            }
        }
    }
    // l'ordre du dossier depend du systeme : on trie pour avoir toujours le meme affichage
    std::sort(filesNames.begin(), filesNames.end());
}

