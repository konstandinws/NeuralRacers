#pragma once
#include <vector>
#include <string>


void loadFilesNameFromFolder(const std::string& folderPath, std::vector<std::string>& filesNames);

// premier nom libre de la forme <prefix>X dans le dossier (fichiers .txt), X = plus grand numero existant + 1
std::string nextFreeName(const std::string& folderPath, const std::string& prefix);

// pour un TextInput de nom de fichier : lettres, chiffres, _ et - acceptes, espace -> _, le reste refuse
int fileNameCharFilter(int c);
