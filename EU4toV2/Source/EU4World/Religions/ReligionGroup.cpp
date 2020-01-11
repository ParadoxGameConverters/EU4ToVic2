#include "ReligionGroup.h"
#include "ParserHelpers.h"

EU4::ReligionGroup::ReligionGroup(const std::string& groupName, std::istream& theStream)
{
	registerKeyword("defender_of_faith", commonItems::ignoreItem);
	registerKeyword("can_form_personal_unions", commonItems::ignoreItem);
	registerKeyword("center_of_religion", commonItems::ignoreItem);
	registerKeyword("flags_with_emblem_percentage", commonItems::ignoreItem);
	registerKeyword("flag_emblem_index_range", commonItems::ignoreItem);
	registerKeyword("ai_will_propagate_through_trade", commonItems::ignoreItem);
	registerKeyword("religious_schools", commonItems::ignoreItem);
	registerKeyword("harmonized_modifier", commonItems::ignoreItem);
	registerKeyword("crusade_name", commonItems::ignoreItem);

	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this, groupName](const std::string& religionName, std::istream& theStream)
	{
		Religion newReligion(religionName, groupName);
		religions.insert(std::make_pair(religionName, newReligion));
		commonItems::ignoreItem(religionName, theStream);
	});

	parseStream(theStream);
	clearRegisteredKeywords();
}