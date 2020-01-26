#include "PopRatio.h"
#include <algorithm>

EU4::PopRatio::PopRatio(std::string _culture, std::string _religion):
culture(std::move(_culture)), religion(std::move(_religion)){}

void EU4::PopRatio::decay(float diffInYears, const PopRatio& currentPop)
{
	const auto upperNonCurrentRatio = 1 - currentPop.upperRatio;
	const auto middleNonCurrentRatio = 1 - currentPop.middleRatio;
	const auto lowerNonCurrentRatio = 1 - currentPop.lowerRatio;

	upperRatio -= std::min(.0025 * diffInYears * upperRatio / upperNonCurrentRatio, upperRatio);
	middleRatio -= std::min(.0025 * diffInYears * middleRatio / middleNonCurrentRatio, middleRatio);
	lowerRatio -= std::min(.0025 * diffInYears * lowerRatio / lowerNonCurrentRatio, lowerRatio);
}

void EU4::PopRatio::increase(float diffInYears)
{
	upperRatio += std::min(.0025 * diffInYears, 1 - upperRatio);
	middleRatio += std::min(.0025 * diffInYears, 1 - middleRatio);
	lowerRatio += std::min(.0025 * diffInYears, 1 - lowerRatio);
}

void EU4::PopRatio::convertFrom()
{
	upperRatio /= 2;
	middleRatio /= 2;
}

void EU4::PopRatio::convertToCulture(const std::string& _culture)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0;
	culture = _culture;
}

void EU4::PopRatio::convertToReligion(const std::string& _religion)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0;
	religion = _religion;
}

void EU4::PopRatio::convertTo(const std::string& _culture, const std::string& _religion)
{
	upperRatio = 0.5;
	middleRatio = 0.5;
	lowerRatio = 0;
	culture = _culture;
	religion = _religion;
}