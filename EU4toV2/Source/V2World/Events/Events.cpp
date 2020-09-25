#include "Events.h"
#include "CommonRegexes.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>
#include <functional>
#include <regex>

V2::Events::Events()
{
	registerKeys();
}

V2::Events::Events(const std::string filename)
{
	std::ifstream theFile(filename);
	std::stringstream buffer;
	buffer << theFile.rdbuf();

	events.push_back(buffer.str());
}

V2::Events::Events(const std::set<std::string> evtFiles)
{
	registerKeys();
	for (const auto& evtFile: evtFiles)
	{
		parseFile(evtFile);
	}
	clearRegisteredKeywords();
}

void V2::Events::registerKeys()
{
	registerRegex(commonItems::catchallRegex, [this](const std::string& unused, std::istream& theStream) {
		commonItems::singleString theEvent(theStream);
		events.push_back(theEvent.getString());
	});
}

void V2::Events::updateEvents(const std::map<int, int>& stateMap, const std::map<int, int>& provinceMap)
{
	for (auto& event: events)
	{
		std::regex state("([A-Z]{3}_)([0-9]+)");
		std::regex prov("(province_id.*)([0-9]+)");
		std::regex provScope("(\\s)([0-9]+)(\\s=\\s\\{)");

		updateIDs(event, state, 2, stateMap);
		updateIDs(event, prov, 2, provinceMap);
		updateIDs(event, provScope, 2, provinceMap);
	}
}

void V2::Events::updateIDs(std::string& theEvent, std::regex expression, int captureGroup, std::map<int, int> map)
{
	std::sregex_iterator end;

	std::sregex_iterator matchItr(theEvent.begin(), theEvent.end(), expression);
	while (matchItr != end)
	{
		if (matchItr->size() > 0)
		{
			std::string newStr;
			for (int i = 1; i < matchItr->size(); ++i)
			{
				if (i != captureGroup)
				{
					newStr += (*matchItr)[i].str();
				}
				else
				{
					newStr += std::to_string(map.find(std::stoi((*matchItr)[i].str()))->second);
				}
			}
			const auto& matchedStr = (*matchItr)[0].str();
			theEvent.replace(theEvent.find(matchedStr), matchedStr.length(), newStr);
		}
		++matchItr;
	}
}
