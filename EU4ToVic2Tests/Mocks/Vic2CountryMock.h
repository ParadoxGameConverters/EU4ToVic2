#ifndef VIC2_COUNTRY_MOCK_H
#define VIC2_COUNTRY_MOCK_H

#include "gmock/gmock.h"
#include "../../EU4toV2/Source/EU4World/Country/EU4Country.h"
#include "../../EU4toV2/Source/V2World/Country/Country.h"

class mockVic2Country: public V2::Country
{
	public:
		MOCK_CONST_METHOD0(isCivilized, bool());
		MOCK_CONST_METHOD0(getProvinces, const std::map<int, std::shared_ptr<V2::Province>>&());
		MOCK_CONST_METHOD0(getSourceCountry, std::shared_ptr<EU4::Country>());
};

#endif // VIC2_COUNTRY_MOCK_H