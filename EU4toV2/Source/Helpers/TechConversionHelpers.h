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



#include <memory>



namespace EU4
{

class Country;

}



namespace helpers
{

double getCountryArmyTech(const std::shared_ptr<EU4::Country>& country);
double getCountryNavyTech(const std::shared_ptr<EU4::Country>& country);
double getCountryCommerceTech(const std::shared_ptr<EU4::Country>& country);
double getCountryCultureTech(const std::shared_ptr<EU4::Country>& country);
double getCountryIndustryTech(const std::shared_ptr<EU4::Country>& country);

void updateMaxAndTotal(double& max, double& total, double techLevel);

double getNormalizedScore(double score, double max, double mean);

}


#endif // TECH_COVNERSION_HELPERS_H