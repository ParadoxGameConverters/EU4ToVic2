#include "StateMapper.h"
#include "../../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"

mappers::StateMapper::StateMapper()
{
	LOG(LogLevel::Info) << "Parsing state region structure.";
	registerKeys();

	std::string filename;
	if (commonItems::DoesFileExist("./blankMod/output/map/region.txt"))
	{
		filename = "./blankMod/output/map/region.txt";
	}
	else
	{
		filename = theConfiguration.getVic2Path() + "/map/region.txt";
	}

	parseFile(filename);
	clearRegisteredKeywords();
}


mappers::StateMapper::StateMapper(std::istream& theStream)
{
	registerKeys();
	parseStream(theStream);
	clearRegisteredKeywords();
}

mappers::StateMapper::StateMapper(std::string& filename)
{
	registerKeys();
	parseFile(filename);
	clearRegisteredKeywords();
}

void mappers::StateMapper::registerKeys()
{
	registerRegex("[a-zA-Z0-9_]+", [this](const std::string& id, std::istream& theStream) 
		{
			const commonItems::intList provinceList(theStream);

			std::regex stateNum("([A-Z]{3}_)([0-9]+)");
			std::smatch sm;
			std::regex_search(id, sm, stateNum);
			const auto& stateID = std::stoi(sm[2].str());

			std::set<int> provinces;
			for (auto province : provinceList.getInts()) provinces.insert(province);
			for (auto province : provinces)
			{
				stateProvincesMap.insert(std::make_pair(province, provinces));
				stateMap.insert(std::make_pair(stateID, provinces));
			}
		});
}

std::set<int> mappers::StateMapper::getAllProvincesInState(const int province) const
{
	const auto& mapping = stateProvincesMap.find(province);
	if (mapping != stateProvincesMap.end()) return mapping->second;
	std::set<int> empty;
	return empty;
}
