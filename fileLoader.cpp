#include "fileLoader.hpp"

void loadCircuitsFromFolder(const std::string& folderPath) {
    for (const auto& element : std::filesystem::directory_iterator(folderPath)) {
        std::cout << element.path() << std::endl;
    }
}