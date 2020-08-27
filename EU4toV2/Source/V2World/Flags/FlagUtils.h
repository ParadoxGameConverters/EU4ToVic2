#ifndef FLAG_UTILS_H
#define FLAG_UTILS_H

#include "newColor.h"
#include <string>

namespace V2
{
bool createColonialFlag(const std::string& colonialOverlordPath, const std::string& colonialBasePath, const std::string& targetPath);
bool createCustomFlag(const commonItems::newColor& c1,
	 const commonItems::newColor& c2,
	 const commonItems::newColor& c3,
	 const std::string& emblemPath,
	 const std::string& basePath,
	 const std::string& targetPath);
} // namespace V2

#endif // FLAG_UTILS_H