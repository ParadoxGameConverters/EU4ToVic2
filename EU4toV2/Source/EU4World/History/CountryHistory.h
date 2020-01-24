#ifndef EU4_COUNTRY_HISTORY_H
#define EU4_COUNTRY_HISTORY_H

#include "newParser.h"
#include "../Leader/EU4Leader.h"

namespace EU4
{
	class CountryHistory : commonItems::parser
	{
	public:
		explicit CountryHistory(std::istream& theStream);
		[[nodiscard]] const std::vector<Leader>& getLeaders() const { return leaders; }
		[[nodiscard]] const std::string& getDynasty() const { return dynasty; }

	private:
		std::vector<Leader> leaders;
		std::string dynasty;
	};
}

#endif // EU4_COUNTRY_HISTORY_H