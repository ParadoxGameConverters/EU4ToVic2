#ifndef EU4_COUNTRY_HISTORY_DATE_H
#define EU4_COUNTRY_HISTORY_DATE_H

#include "newParser.h"
#include "../Leader/EU4Leader.h"

namespace EU4
{
	class CountryHistoryDate: commonItems::parser
	{
		public:
			CountryHistoryDate(std::istream& theStream);
			const std::vector<EU4::Leader>& getLeaders() const { return leaders; }

		private:
			std::vector<EU4::Leader> leaders;
	};
}

#endif // EU4_COUNTRY_HISTORY_DATE_H