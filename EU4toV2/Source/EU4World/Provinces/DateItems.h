#ifndef DATE_ITEMS_H
#define DATE_ITEMS_H

#include "newParser.h"
#include <vector>
#include <map>

namespace EU4
{
	enum class DateItemType
	{
		OWNER_CHANGE,
		CULTURE_CHANGE,
		RELIGION_CHANGE
	};

	static std::map<std::string, DateItemType> DateItemNames
	{
		{ "owner", DateItemType::OWNER_CHANGE },
		{ "culture", DateItemType::CULTURE_CHANGE },
		{ "religion", DateItemType::RELIGION_CHANGE }
	};

	class DateItems : commonItems::parser
	{
	public:
		DateItems(std::istream& theStream);
		const std::vector<std::pair<DateItemType, std::string>>& getDateChanges() const { return dateChanges; }

	private:
		std::vector<std::pair<DateItemType, std::string>> dateChanges;
	};
}

#endif // DATE_ITEMS_H