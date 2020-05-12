#ifndef EU4_COUNTRY_HISTORY_H
#define EU4_COUNTRY_HISTORY_H

#include "Parser.h"
#include "../Leader/EU4Leader.h"

namespace EU4
{
	class CountryHistory : commonItems::parser
	{
	public:
		explicit CountryHistory(std::istream& theStream);
		
		[[nodiscard]] const auto& getLeaders() const { return leaders; }
		[[nodiscard]] const auto& getDynasty() const { return dynasty; }
		[[nodiscard]] const auto& getPrimaryCulture() const { return primaryCulture; }
		[[nodiscard]] const auto& getReligion() const { return religion; }

	private:
		std::vector<Leader> leaders;
		std::string dynasty;
		std::string religion;
		std::string primaryCulture;
	};
}

#endif // EU4_COUNTRY_HISTORY_H