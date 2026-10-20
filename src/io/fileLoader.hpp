#pragma once
#include <vector>
#include <string>


void loadFilesNameFromFolder(const std::string& folderPath, std::vector<std::string>& filesNames);

// first free name of the form <prefix>X in the folder (.txt files), X = highest existing number + 1
std::string nextFreeName(const std::string& folderPath, const std::string& prefix);

// for a file name TextInput: letters, digits, _ and - accepted, space -> _, everything else rejected
int fileNameCharFilter(int c);
