#ifndef CULTURE_H
#define CULTURE_H

#include "newParser.h"

namespace mappers
{
	class Culture: commonItems::parser
	{
	public:
		Culture() = default;
		explicit Culture(std::istream& theStream);

		[[nodiscard]] const auto& getMaleNames() const { return maleNames; }
		[[nodiscard]] const auto& getFemaleNames() const { return femaleNames; }
		[[nodiscard]] const auto& getDynastyNames() const { return dynastyNames; }

		void addNameNames(const std::vector<std::string>& names) { maleNames.insert(maleNames.end(), names.begin(), names.end()); }
		void addFemaleNames(const std::vector<std::string>& names) { femaleNames.insert(femaleNames.end(), names.begin(), names.end()); }
		void addDynastyNames(const std::vector<std::string>& names) { dynastyNames.insert(dynastyNames.end(), names.begin(), names.end()); }		

	private:
		std::string primaryTag;
		std::string graphicalCulture;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;
		std::vector<std::string> dynastyNames;
	};
}

#endif // CULTURE_H
