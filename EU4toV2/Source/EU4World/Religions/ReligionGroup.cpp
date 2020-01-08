#include "ReligionGroup.h"
#include "ParserHelpers.h"

EU4::ReligionGroup::ReligionGroup(const std::string& groupName, std::istream& theStream)
{
	registerKeyword(std::regex("defender_of_faith"), commonItems::ignoreItem);
	registerKeyword(std::regex("can_form_personal_unions"), commonItems::ignoreItem);
	registerKeyword(std::regex("center_of_religion"), commonItems::ignoreItem);
	registerKeyword(std::regex("flags_with_emblem_percentage"), commonItems::ignoreItem);
	registerKeyword(std::regex("flag_emblem_index_range"), commonItems::ignoreItem);
	registerKeyword(std::regex("ai_will_propagate_through_trade"), commonItems::ignoreItem);
	registerKeyword(std::regex("religious_schools"), commonItems::ignoreItem);
	registerKeyword(std::regex("harmonized_modifier"), commonItems::ignoreItem);
	registerKeyword(std::regex("crusade_name"), commonItems::ignoreItem);

	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this, groupName](const std::string& religionName, std::istream& theStream)
	{
		Religion newReligion(religionName, groupName);
		religions.insert(std::make_pair(religionName, newReligion));
		commonItems::ignoreItem(religionName, theStream);
	});

	parseStream(theStream);
}