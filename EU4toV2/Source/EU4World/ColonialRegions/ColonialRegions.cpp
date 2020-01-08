#include "ColonialRegion.h"
#include "ColonialRegions.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

EU4::ColonialRegions::ColonialRegions()
{
	LOG(LogLevel::Info) << "Parsing EU4 colonial regions";

	registerKeyword(std::regex("colonial_\\w+"), [this](const std::string& regionName, std::istream& theStream)
		{
			ColonialRegion newRegion(theStream);
			for (const auto& provinceID : newRegion.getProvinces())
			{
				provinceToColonialRegions.insert(std::make_pair(provinceID, regionName));
			}
		});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

		
	parseFile(theConfiguration.getEU4Path() + "/common/colonial_regions/00_colonial_regions.txt");

	for (auto mod: theConfiguration.getEU4Mods())
	{
		std::set<std::string> filenames;
		if (Utils::doesFolderExist(mod + "/common/colonial_regions/"))
		{
			Utils::GetAllFilesInFolder(mod + "/common/colonial_regions/", filenames);
			for (auto filename : filenames)
			{
				parseFile(mod + "/common/colonial_regions/" + filename);
			}
		}
	}
}

bool EU4::ColonialRegions::provinceIsInRegion(int province, const std::string& region) const
{
	auto mapping = provinceToColonialRegions.find(province);
	if (mapping != provinceToColonialRegions.end())
	{
		return (mapping->second == region);
	}
	else
	{
		return false;
	}
}
