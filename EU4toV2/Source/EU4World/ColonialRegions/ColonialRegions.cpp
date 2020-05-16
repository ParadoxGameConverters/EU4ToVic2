#include "ColonialRegion.h"
#include "ColonialRegions.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

EU4::ColonialRegions::ColonialRegions()
{
	LOG(LogLevel::Info) << "Parsing EU4 colonial regions";

	registerRegex("colonial_\\w+", [this](const std::string& regionName, std::istream& theStream)
		{
			const ColonialRegion newRegion(theStream);
			for (const auto& provinceID : newRegion.getProvinces())
			{
				provinceToColonialRegions.insert(std::make_pair(provinceID, regionName));
			}
		});
	registerRegex("[a-zA-Z0-9_\\.:]+", commonItems::ignoreItem);

	if (!Utils::DoesFileExist(theConfiguration.getEU4Path() + "/common/colonial_regions/00_colonial_regions.txt"))
	{
		Log(LogLevel::Warning) << "Could not find " << theConfiguration.getEU4Path() + "/common/colonial_regions/00_colonial_regions.txt";
	}
	else
	{
		parseFile(theConfiguration.getEU4Path() + "/common/colonial_regions/00_colonial_regions.txt");
	}

	for (const auto& mod: theConfiguration.getEU4Mods())
	{		
		if (Utils::DoesFolderExist(mod + "/common/colonial_regions/"))
		{
			auto filenames = Utils::GetAllFilesInFolder(mod + "/common/colonial_regions/");
			for (const auto& filename : filenames)
			{
				parseFile(mod + "/common/colonial_regions/" + filename);
			}
		}
	}
	clearRegisteredKeywords();
}

bool EU4::ColonialRegions::provinceIsInRegion(const int province, const std::string& region) const
{
	const auto& mapping = provinceToColonialRegions.find(province);
	if (mapping != provinceToColonialRegions.end()) return mapping->second == region;
	return false;
}
