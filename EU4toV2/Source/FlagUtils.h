#ifndef FLAGUTILS_H_
#define FLAGUTILS_H_

#include <string>
#include "Color.h"

bool CreateColonialFlag(std::string colonialOverlordPath, std::string colonialBasePath, std::string targetPath);
bool CreateCustomFlag(commonItems::Color c1, commonItems::Color c2, commonItems::Color c3, std::string emblemPath, std::string basePath, std::string targetPath);

#endif