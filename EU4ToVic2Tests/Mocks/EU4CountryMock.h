#ifndef EU4_COUNTRY_MOCK_H
#define EU4_COUNTRY_MOCK_H

#include "gmock/gmock.h"
#include "../../EU4toV2/Source/EU4World/Country/EU4Country.h"

class mockEU4Country: public EU4::Country
{
	public:
		MOCK_CONST_METHOD0(getAdmTech, double());
		MOCK_CONST_METHOD0(getDipTech, double());
		MOCK_CONST_METHOD0(getMilTech, double());
		MOCK_CONST_METHOD0(getArmy, double());
		MOCK_CONST_METHOD0(getNavy, double());
		MOCK_CONST_METHOD0(getCommerce, double());
		MOCK_CONST_METHOD0(getIndustry, double());
		MOCK_CONST_METHOD0(getCulture, double());
		MOCK_CONST_METHOD0(getNationalIdeas, const std::set<std::string>&());
};

#endif // EU4_COUNTRY_MOCK_H