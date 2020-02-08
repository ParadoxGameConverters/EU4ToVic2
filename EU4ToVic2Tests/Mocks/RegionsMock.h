#ifndef EU4_REGIONS_MOCK_H
#define EU4_REGIONS_MOCK_H

#include "gmock/gmock.h"
#include "../../EU4toV2/Source/EU4World/Regions/Regions.h"

class mockRegions: public EU4::Regions
{
	public:
		MOCK_CONST_METHOD1(regionIsValid, bool(const std::string& regionName));
		MOCK_CONST_METHOD2(provinceInRegion, bool(int province, const std::string& regionName));
};

#endif // EU4_REGIONS_MOCK_H
