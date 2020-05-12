#ifndef EU4_COUNTRY_HISTORY_DATE_H
#define EU4_COUNTRY_HISTORY_DATE_H

#include "Parser.h"
#include "../Leader/EU4Leader.h"

namespace EU4
{
	class CountryHistoryDate: commonItems::parser
	{
	public:
		explicit CountryHistoryDate(std::istream& theStream, const std::string& leaderClass);
		
		[[nodiscard]] const auto& getLeaders() const { return leaders; }
		[[nodiscard]] const auto& getDynasty() const { return dynasty; }

	private:
		std::vector<Leader> leaders;
		std::string dynasty;
	};
}

#endif // EU4_COUNTRY_HISTORY_DATE_H