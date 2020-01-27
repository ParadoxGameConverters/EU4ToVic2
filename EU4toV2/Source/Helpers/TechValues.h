#ifndef TECH_CONVERSION_HELPERS_H
#define TECH_CONVERSION_HELPERS_H

#include <map>
#include <string>
#include <memory>

namespace EU4
{
	class Country;
}
namespace mappers
{
	class IdeaEffectMapper;
	class TechgroupsMapper;
}
namespace V2
{
	class Country;
}

namespace helpers
{
	class TechValues
	{
	public:
		explicit TechValues(const std::map<std::string, std::shared_ptr<V2::Country>>& countries);

		[[nodiscard]] static bool isValidCountryForTechConversion(const V2::Country& country);
		[[nodiscard]] double getNormalizedArmyTech(const EU4::Country& country) const;
		[[nodiscard]] double getNormalizedNavyTech(const EU4::Country& country) const;
		[[nodiscard]] double getNormalizedCommerceTech(const EU4::Country& country) const;
		[[nodiscard]] double getNormalizedCultureTech(const EU4::Country& country) const;
		[[nodiscard]] double getNormalizedIndustryTech(const EU4::Country& country) const;

	private:
		[[nodiscard]] static double getCountryArmyTech(const EU4::Country& country);
		[[nodiscard]] static double getCountryNavyTech(const EU4::Country& country);
		[[nodiscard]] static double getCountryCommerceTech(const EU4::Country& country);
		[[nodiscard]] static double getCountryCultureTech(const EU4::Country& country);
		[[nodiscard]] static double getCountryIndustryTech(const EU4::Country& country);
		[[nodiscard]] static double getNormalizedScore(double score, double max, double mean);

		double armyMax = 0;
		double navyMax = 0;
		double commerceMax = 0;
		double cultureMax = 0;
		double industryMax = 0;

		double armyMean = 0;
		double navyMean = 0;
		double commerceMean = 0;
		double cultureMean = 0;
		double industryMean = 0;
	};
}

#endif // TECH_CONVERSION_HELPERS_H