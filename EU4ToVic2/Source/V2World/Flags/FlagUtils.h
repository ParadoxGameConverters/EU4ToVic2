#ifndef FLAG_UTILS_H
#define FLAG_UTILS_H

#include "Color.h"
#include <string>

namespace V2
{
bool createColonialFlag(const std::filesystem::path& colonialOverlordPath,
	 const std::filesystem::path& colonialBasePath,
	 const std::filesystem::path& targetPath);
bool createCustomFlag(const commonItems::Color& c1,
	 const commonItems::Color& c2,
	 const commonItems::Color& c3,
	 const std::filesystem::path& emblemPath,
	 const std::filesystem::path& basePath,
	 const std::filesystem::path& targetPath);
} // namespace V2

#endif // FLAG_UTILS_H