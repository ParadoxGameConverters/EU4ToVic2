#include "UnitTypeMapper.h"
#include "CommonFunctions.h"
#include "Configuration.h"
#include "OSCompatibilityLayer.h"
#include <set>

void mappers::UnitTypeMapper::initUnitTypeMapper()
{
	Log(LogLevel::Info) << "Parsing unit strengths from EU4 installation.";

	for (const auto& filename: commonItems::GetAllFilesInFolder(theConfiguration.getEU4Path() / "common/units"))
		addUnitFileToRegimentTypeMap(theConfiguration.getEU4Path() / "common/units", filename);

	for (const auto& mod: theConfiguration.getMods())
		for (const auto& filename: commonItems::GetAllFilesInFolder(mod.path / "common/units"))
			addUnitFileToRegimentTypeMap(mod.path / "common/units", filename);
}

void mappers::UnitTypeMapper::loadUnitType(const std::string& unitName, std::istream& theStream)
{
	UnitType unitType(theStream);
	unitTypeMap.insert(std::pair(unitName, unitType));
}

void mappers::UnitTypeMapper::addUnitFileToRegimentTypeMap(const std::filesystem::path& directory, const std::filesystem::path& filename)
{
	auto name = filename.stem();

	UnitType unitType(directory / filename);
	if (unitType.getCategory().empty())
	{
		Log(LogLevel::Warning) << "Unit file for " << name << " at: " << directory.string() << "/" << filename.string() << " has no type!";
		return;
	}

	unitTypeMap.insert(std::pair(name.string(), unitType));
}

std::optional<mappers::UnitType> mappers::UnitTypeMapper::getUnitTypeForRegimentTypeName(const std::string& regimentTypeName) const
{
	if (const auto& typeItr = unitTypeMap.find(regimentTypeName); typeItr != unitTypeMap.end())
		return typeItr->second;
	else
		return std::nullopt;
}
