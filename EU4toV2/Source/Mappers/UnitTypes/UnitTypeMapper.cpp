#include "UnitTypeMapper.h"
#include "ParserHelpers.h"
#include "Log.h"
#include <set>
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"


mappers::UnitTypeMapper::UnitTypeMapper()
{
	LOG(LogLevel::Info) << "Parsing unit strengths from EU4 installation.";

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/units/", filenames);
	for (const auto& filename : filenames)
	{
		addUnitFileToRegimentTypeMap(theConfiguration.getEU4Path() + "/common/units", filename);
	}

	for (const auto& modName : theConfiguration.getEU4Mods())
	{
		std::set<std::string> moreFilenames;
		Utils::GetAllFilesInFolder(modName + "/common/units/", moreFilenames);
		for (const auto& filename : moreFilenames)
		{
			addUnitFileToRegimentTypeMap(modName + "/common/units", filename);
		}
	}

}

void mappers::UnitTypeMapper::addUnitFileToRegimentTypeMap(const std::string& directory, const std::string& filename)
{
	const int period = filename.find_last_of('.');
	auto name = filename.substr(0, period);

	UnitType unitType(directory + "/" + filename);
	if (unitType.getCategory() == EU4::REGIMENTCATEGORY::num_reg_categories)
	{
		LOG(LogLevel::Warning) << "Unit file for " << name << " at: " << directory << "/" << filename << " has no type!";
		return;
	}

	if (unitType.getCategory() == EU4::REGIMENTCATEGORY::transport) unitType.setStrength(24);

	unitTypeMap.insert(std::pair(name, unitType));
}
