#include "Religions.h"
#include "ReligionGroup.h"
#include "ParserHelpers.h"

void EU4::Religions::addReligions(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z_]+"), [this](const std::string& groupName, std::istream& theStream) {
		ReligionGroup newGroup(groupName, theStream);
		for (auto religion: newGroup.takeReligions())
		{
			theReligions.insert(religion);
		}
	});
	registerKeyword(std::regex("[a-zA-Z0-9_\\.:]+"), commonItems::ignoreItem);

	parseStream(theStream);
}


std::optional<EU4::Religion> EU4::Religions::getReligion(std::string name) const
{
	auto religionsItr = theReligions.find(name);
	if (religionsItr != theReligions.end())
	{
		return religionsItr->second;
	}
	else
	{
		return {};
	}
}