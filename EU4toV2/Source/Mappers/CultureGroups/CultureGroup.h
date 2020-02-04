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
		CultureGroup(std::string _name, std::istream& theStream);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getCultures() const { return cultures; }

		void addCulture(const std::string& name, const Culture& culture) { cultures.insert(std::make_pair(name, culture)); }
		void mergeCulture(const std::string& name, const Culture& culture);

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
