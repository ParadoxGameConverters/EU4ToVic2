#ifndef CULTURE_GROUP_H
#define CULTURE_GROUP_H

#include "newParser.h"
#include "Culture.h"
#include <map>

namespace mappers
{
	class CultureGroup: commonItems::parser
	{
	public:
		CultureGroup(const std::string& name_, std::istream& theStream);

		std::string getName() const { return name; }
		std::map<std::string, Culture> getCultures() const { return cultures; }

	private:
		std::string name;
		std::string graphicalCulture;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;
		std::vector<std::string> dynastyNames;
		std::map<std::string, Culture> cultures;
	};
}

#endif // CULTURE_GROUP_H
