#include "V2FactoryInventionLoader.h"
#include "V2FactoryInvention.h"
#include "ParserHelpers.h"
#include "Log.h"
#include <set>
#include <fstream>
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"


mappers::V2FactoryInventionLoader::V2FactoryInventionLoader()
{
	LOG(LogLevel::Info) << "\tLoading V2 Factory Inventions";

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getVic2Path() + "/inventions/", filenames);
	for (auto filename : filenames)
	{
		ParseV2InventionFile(theConfiguration.getVic2Path() + "/inventions/" + filename);
	}
}


void mappers::V2FactoryInventionLoader::ParseV2InventionFile(const std::string& filename)
{
	std::ifstream incFile(filename);	// the parsed regiment costs file
	if (incFile.fail())
	{
		std::runtime_error exception("Could not open: " + filename);
		throw exception;
	}

	V2FactoryInvention factoryInvention(incFile);
	std::map<std::string, std::string> incInventions = factoryInvention.getFactoryInventionMap();
	factoryInventionMap.insert(incInventions.begin(), incInventions.end());
}
