#include "UnitTypeMapper.h"
#include "ParserHelpers.h"
#include "Log.h"
#include <set>
#include <fstream>
#include "OSCompatibilityLayer.h"
#include "../Configuration.h"


mappers::UnitTypeMapper::UnitTypeMapper()
{
	LOG(LogLevel::Info) << "\tReading unit strengths from EU4 installation folder";

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getEU4Path() + "/common/units/", filenames);
	for (auto filename : filenames)
	{
		AddUnitFileToRegimentTypeMap((theConfiguration.getEU4Path() + "/common/units"), filename);
	}

	for (auto modName : theConfiguration.getEU4Mods())
	{
		std::set<std::string> filenames;
		Utils::GetAllFilesInFolder(modName + "/common/units/", filenames);
		for (auto filename : filenames)
		{
			AddUnitFileToRegimentTypeMap((modName + "/common/units"), filename);
		}
	}

}


void mappers::UnitTypeMapper::AddUnitFileToRegimentTypeMap(const std::string directory, const std::string filename)
{
	std::string filePath = directory + "/" + filename;
	std::ifstream incFile(filePath);	// the parsed regiment costs file
	if (incFile.fail())
	{
		std::runtime_error exception("Could not open: " + directory + "/" + filename);
		throw exception;
	}

	int period = filename.find_last_of('.');
	std::string name = filename.substr(0, period);

	UnitType unitType(incFile);
	if (unitType.getCategory() == EU4::REGIMENTCATEGORY::num_reg_categories)
	{
		LOG(LogLevel::Warning) << "Unit file for " << name << " at: " << filePath << " has no type!";
		return;
	}

	if (unitType.getCategory() == EU4::REGIMENTCATEGORY::transport) unitType.setStrength(24);

	unitTypeMap.insert(std::pair(name, unitType));
}
