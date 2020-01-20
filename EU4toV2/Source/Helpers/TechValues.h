#ifndef TECH_COVNERSION_HELPERS_H
#define TECH_COVNERSION_HELPERS_H

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
		TechValues(const std::map<std::string, std::shared_ptr<V2::Country>>& countries);

		bool isValidCountryForTechConversion(std::shared_ptr<V2::Country> country) const;
		double getNormalizedArmyTech(const EU4::Country& country) const;
		double getNormalizedNavyTech(const EU4::Country& country) const;
		double getNormalizedCommerceTech(const EU4::Country& country) const;
		double getNormalizedCultureTech(const EU4::Country& country) const;
		double getNormalizedIndustryTech(const EU4::Country& country) const;

	private:
		double getCountryArmyTech(const EU4::Country& country) const;
		double getCountryNavyTech(const EU4::Country& country) const;
		double getCountryCommerceTech(const EU4::Country& country) const;
		double getCountryCultureTech(const EU4::Country& country) const;
		double getCountryIndustryTech(const EU4::Country& country) const;

		double getNormalizedScore(double score, double max, double mean) const;

		double armyMax = 0.0;
		double navyMax = 0.0;
		double commerceMax = 0.0;
		double cultureMax = 0.0;
		double industryMax = 0.0;

		double armyMean = 0.0;
		double navyMean = 0.0;
		double commerceMean = 0.0;
		double cultureMean = 0.0;
		double industryMean = 0.0;
	};
}

#endif // TECH_COVNERSION_HELPERS_H