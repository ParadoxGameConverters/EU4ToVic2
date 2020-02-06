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
		CultureGroup() = default;
		CultureGroup(std::string _name, std::istream& theStream);

		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getCultures() const { return cultures; }

		void addNeoCulture(const std::string& name, const Culture& culture, const std::string& oldCulture);
		void mergeCulture(const std::string& name, const Culture& culture);
		[[nodiscard]] auto containsCulture(const std::string& cultureName) const { return cultures.count(cultureName) > 0; }

		friend std::ostream& operator<<(std::ostream& output, const CultureGroup& cultureGroup);

	private:
		std::string name;
		std::string graphicalCulture;
		std::string unit;
		std::string leader;
		std::string isOverseas;
		std::string culturalUnionTag;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;
		std::vector<std::string> dynastyNames;
		std::map<std::string, Culture> cultures;
	};
}

#endif // CULTURE_GROUP_H
