#include "V2FactoryTechLoader.h"
#include "V2FactoryTech.h"
#include "ParserHelpers.h"
#include "Log.h"
#include <set>
#include <fstream>
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"


mappers::V2FactoryTechLoader::V2FactoryTechLoader()
{
	LOG(LogLevel::Info) << "\tLoading V2 Factory Techs";

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getVic2Path() + "/technologies/", filenames);
	for (auto filename : filenames)
	{
		ParseV2TechFile(theConfiguration.getVic2Path() + "/technologies/" + filename);
	}
}


void mappers::V2FactoryTechLoader::ParseV2TechFile(const std::string& filename)
{
	std::ifstream incFile(filename);	// the parsed regiment costs file
	if (incFile.fail())
	{
		std::runtime_error exception("Could not open: " + filename);
		throw exception;
	}

	V2FactoryTech factoryTech(incFile);
	std::map<std::string, std::string> incTechs = factoryTech.getFactoryTechMap();
	factoryTechMap.insert(incTechs.begin(), incTechs.end());
}
