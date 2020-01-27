#ifndef RELIGION_GROUP
#define RELIGION_GROUP

#include "newParser.h"
#include "Religion.h"
#include <map>

namespace EU4
{
	class ReligionGroup: commonItems::parser
	{
	public:
		ReligionGroup(const std::string& groupName, std::istream& theStream);
		auto takeReligions() { return std::move(religions); }

	private:
		std::map<std::string, Religion> religions;
	};
}

#endif // RELIGION_GROUP