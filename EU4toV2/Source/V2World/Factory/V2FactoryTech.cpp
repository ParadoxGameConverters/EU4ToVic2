#include "V2FactoryTech.h"
#include "V2FactoryTechDetails.h"
#include "ParserHelpers.h"
#include "Log.h"


mappers::V2FactoryTech::V2FactoryTech(std::istream& theStream)
{
	registerKeyword(std::regex("[a-z_]+"), [this](const std::string& tech, std::istream& theStream)
		{
			V2FactoryTechDetails techDetails(theStream);
			// Every V2 technology activates but a single building, no worry of missing something.
			if (!techDetails.getFactoryName().empty())
			{
				factoryTechMap.insert(std::make_pair(techDetails.getFactoryName(), tech));
			}
		});
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), commonItems::ignoreItem);

	parseStream(theStream);
}
