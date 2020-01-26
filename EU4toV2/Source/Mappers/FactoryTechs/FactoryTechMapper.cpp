#include "FactoryTechMapper.h"
#include "FactoryTechDetails.h"
#include "ParserHelpers.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "../../Configuration.h"

mappers::FactoryTechMapper::FactoryTechMapper()
{
	registerKeys();
	LOG(LogLevel::Info) << "Parsing V2 Factory Techs";

	std::set<std::string> filenames;
	Utils::GetAllFilesInFolder(theConfiguration.getVic2Path() + "/technologies/", filenames);
	for (const auto& filename : filenames)
	{
		parseFile(theConfiguration.getVic2Path() + "/technologies/" + filename);
	}
	clearRegisteredKeywords();
}

mappers::FactoryTechMapper::FactoryTechMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

void mappers::FactoryTechMapper::registerKeys()
{
	registerRegex("[a-z_]+", [this](const std::string& tech, std::istream& theStream)
		{
			const FactoryTechDetails techDetails(theStream);
			// Every V2 technology activates but a single building, no worry of missing something.
			if (!techDetails.getFactoryName().empty())
			{
				factoryTechMap.insert(std::make_pair(techDetails.getFactoryName(), tech));
			}
		});
	registerRegex("[a-zA-Z0-9\\_.:]+", commonItems::ignoreItem);
}
