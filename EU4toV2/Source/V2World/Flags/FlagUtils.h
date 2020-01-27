#ifndef FLAG_UTILS_H
#define FLAG_UTILS_H

#include <string>
#include "Color.h"

namespace V2
{
	bool createColonialFlag(
		const std::string& colonialOverlordPath, 
		const std::string& colonialBasePath, 
		const std::string& targetPath);
	bool createCustomFlag(
		const commonItems::Color& c1, 
		const commonItems::Color& c2, 
		const commonItems::Color& c3, 
		const std::string& emblemPath, 
		const std::string& basePath, 
		const std::string& targetPath);
}

#endif // FLAG_UTILS_H