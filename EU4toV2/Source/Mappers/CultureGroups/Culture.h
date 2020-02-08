#ifndef CULTURE_H
#define CULTURE_H

#include "newParser.h"
#include "Color.h"

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
		[[nodiscard]] const auto& getOriginalCulture() const { return originalCulture; }
		[[nodiscard]] auto getNeoCulture() const { return neoCulture; }

		void setFirstNames(const std::vector<std::string>& fnames) { firstNames = fnames; }
		void setLastNames(const std::vector<std::string>& lnames) { lastNames = lnames; }
		void setColor(const commonItems::Color& theColor) { color = theColor; }
		
		void addNameNames(const std::vector<std::string>& names) { maleNames.insert(maleNames.end(), names.begin(), names.end()); }
		void addFemaleNames(const std::vector<std::string>& names) { femaleNames.insert(femaleNames.end(), names.begin(), names.end()); }
		void addDynastyNames(const std::vector<std::string>& names) { dynastyNames.insert(dynastyNames.end(), names.begin(), names.end()); }
		void setNeoCulture(const bool nc) { neoCulture = nc; radicalism = 10; }
		void setOriginalCulture(const std::string& origName) { originalCulture = origName; }

		void transmogrify();

		friend std::ostream& operator<<(std::ostream& output, const Culture& culture);

	private:
		std::string primaryTag;
		std::string graphicalCulture;
		std::vector<std::string> maleNames;
		std::vector<std::string> femaleNames;
		std::vector<std::string> firstNames;
		std::vector<std::string> lastNames;
		std::vector<std::string> dynastyNames;
		commonItems::Color color;
		int radicalism = 0;
		bool neoCulture = false; // culture generated on-the-fly by the converter
		std::string originalCulture; // relevant only for neocultures so we know where they originated from.

	};
}

#endif // CULTURE_H
