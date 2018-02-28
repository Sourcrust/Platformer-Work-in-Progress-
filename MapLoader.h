#ifndef MAPLOADER_H
#define MAPLOADER_H
#include <vector>
#include <string>
#include "Platform.h"

int stringToInt(std::string s);
bool loadMap(int &numPlatforms, std::vector<Platform> &platforms,std::string f);

#endif
