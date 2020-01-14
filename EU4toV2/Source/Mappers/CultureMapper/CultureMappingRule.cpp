#include "CultureMappingRule.h"
#include "ParserHelpers.h"

mappers::CultureMappingRule::CultureMappingRule(std::istream& theStream)
{
	registerKeyword("vic2", [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString v2Str(theStream);
			destinationCulture = v2Str.getString();
		});
	registerKeyword("eu4", [this](const std::string& unused, std::istream& theStream) 
		{
			commonItems::singleString eu4Str(theStream);
			sourceCultures.push_back(eu4Str.getString());
		});
	registerRegex("region|religion|owner|provinceid", [this](const std::string& typeString, std::istream& theStream) 
		{
			commonItems::singleString distinguisherStr(theStream);
		
			mappers::DISTINGUISHER_TYPES type;
			if (typeString == "region") { type = DISTINGUISHER_TYPES::REGION; }
			else if (typeString == "religion") { type = DISTINGUISHER_TYPES::RELIGION; }
			else if (typeString == "owner") { type = DISTINGUISHER_TYPES::OWNER; }
			else if (typeString == "provinceid") { type = DISTINGUISHER_TYPES::PROVINCE; }

			distinguishers.insert(std::make_pair(type, distinguisherStr.getString()));
		});

	parseStream(theStream);
	clearRegisteredKeywords();

	for (auto sourceCulture: sourceCultures)
	{
		CultureMapping newRule(sourceCulture, destinationCulture, distinguishers);
		mappings.push_back(newRule);
	}
}