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



#ifndef V2_TECH_SCHOOL
#define V2_TECH_SCHOOL



#include "newParser.h"
#include <string>
#include <vector>



namespace Vic2
{

class Vic2TechSchool: commonItems::parser
{
	public:
		Vic2TechSchool(const std::string& name, std::istream& theStream);

		double calculateComparisonScore(double armyInvestment, double commerceInvestment, double cultureInvestment, double industryInvestment, double navyInvestment) const;

		std::string getName() const { return name; }

	private:
		std::string name;

		double armyBonus;
		double commerceBonus;
		double cultureBonus;
		double industryBonus;
		double navyBonus;
};

}



#endif // V2_TECH_SCHOOL