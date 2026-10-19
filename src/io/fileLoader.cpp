#include "io/fileLoader.hpp"
#include <filesystem>



void loadFilesNameFromFolder(const std::string& folderPath, std::vector<std::string>& filesNames) {
    if (std::filesystem::exists(folderPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            if (entry.path().extension() == ".txt") { 
                filesNames.push_back(entry.path().filename().string());
            }
        }
    }
}

