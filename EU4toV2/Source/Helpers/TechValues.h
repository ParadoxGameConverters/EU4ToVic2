/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef TECH_COVNERSION_HELPERS_H
#define TECH_COVNERSION_HELPERS_H



#include <map>
#include <memory>
#include <string>



namespace EU4
{
class Country;
}
namespace mappers
{
class IdeaEffectMapper;
}

class V2Country;



namespace helpers
{

class TechValues
{
	public:
		TechValues(const std::map<std::string, V2Country*>& countries);

		bool isValidCountryForTechConversion(const V2Country* country) const;
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