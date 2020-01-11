#ifndef EU4_MODS_H
#define EU4_MODS_H

#include "../../Configuration.h"
#include <map>
#include <optional>

namespace EU4
{
	class Mods
	{
	public:
		Mods(const std::vector<std::string>& usedMods, Configuration& theConfiguration);
		std::optional<std::string> getModPath(const std::string& modName) const;

	private:
		void loadEU4ModDirectory(const Configuration& theConfiguration);
		void loadSteamWorkshopDirectory(const Configuration& theConfiguration);
		void loadCK2ExportDirectory(const Configuration& theConfiguration);
		void loadModDirectory(const std::string& searchDirectory);

		std::map<std::string, std::string> possibleMods;
		std::map<std::string, std::string> possibleCompressedMods;
	};
}

#endif // EU4_MODS_H