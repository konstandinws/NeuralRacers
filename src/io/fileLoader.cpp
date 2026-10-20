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
    // folder order depends on the OS: sort so the display is always the same
    std::sort(filesNames.begin(), filesNames.end());
}

std::string nextFreeName(const std::string& folderPath, const std::string& prefix) {
    int highest = 0;
    if (std::filesystem::exists(folderPath)) {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
            std::string stem = entry.path().stem().string();
            if (entry.path().extension() != ".txt" || stem.rfind(prefix, 0) != 0) {
                continue;
            }
            std::string number = stem.substr(prefix.size());
            if (!number.empty() && number.size() < 9 && std::all_of(number.begin(), number.end(), ::isdigit)) {
                highest = std::max(highest, std::stoi(number));
            }
        }
    }
    return prefix + std::to_string(highest + 1);
}

int fileNameCharFilter(int c) {
    bool letterOrDigit = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    if (letterOrDigit || c == '_' || c == '-') {
        return c;
    }
    if (c == ' ') {
        return '_';
    }
    return 0;
}

