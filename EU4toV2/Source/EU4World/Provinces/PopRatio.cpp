#include "PopRatio.h"
#include <algorithm>

EU4::PopRatio::PopRatio(const std::string& _culture, const std::string& _religion):
culture(_culture), religion(_religion){}

void EU4::PopRatio::decay(float diffInYears, const EU4::PopRatio& currentPop)
{
	double upperNonCurrentRatio = (1.0 - currentPop.upperRatio);
	double middleNonCurrentRatio = (1.0 - currentPop.middleRatio);
	double lowerNonCurrentRatio = (1.0 - currentPop.lowerRatio);

	upperRatio -= std::min(.0025 * diffInYears * upperRatio / upperNonCurrentRatio, upperRatio);
	middleRatio -= std::min(.0025 * diffInYears * middleRatio / middleNonCurrentRatio, middleRatio);
	lowerRatio -= std::min(.0025 * diffInYears * lowerRatio / lowerNonCurrentRatio, lowerRatio);
}

void EU4::PopRatio::increase(float diffInYears)
{
	upperRatio += std::min(.0025 * diffInYears, 1.0 - upperRatio);
	middleRatio += std::min(.0025 * diffInYears, 1.0 - middleRatio);
	lowerRatio += std::min(.0025 * diffInYears, 1.0 - lowerRatio);
}

void EU4::PopRatio::convertFrom()
{
	upperRatio /= 2.0;
	middleRatio /= 2.0;
}

void EU4::PopRatio::convertToCulture(const std::string& _culture)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0.0;
	culture = _culture;
}

void EU4::PopRatio::convertToReligion(const std::string& _religion)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0.0;
	religion = _religion;
}

void EU4::PopRatio::convertTo(const std::string& _culture, const std::string& _religion)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0.0;
	culture = _culture;
	religion = _religion;
}