/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */



#ifndef POP_RATIO_H_
#define POP_RATIO_H_



#include "Date.h"
#include <string>



namespace EU4
{

class PopRatio
{
	public:
	PopRatio(const std::string& _culture, const std::string& _religion):
		culture(_culture),
		religion(_religion)
	{}

		void decay(float diffInYears, const EU4::PopRatio& currentPop);
		void increase(float diffInYears);
		void convertFrom();
		void convertToCulture(const std::string& culture);
		void convertToReligion(const std::string& religion);
		void convertTo(const std::string& culture, const std::string& religion);

		std::string getCulture() const { return culture; }
		std::string getReligion() const { return religion; }
		double getUpperRatio() const { return upperRatio; }
		double getMiddleRatio() const { return middleRatio; }
		double getLowerRatio() const { return lowerRatio; }

	private:
		std::string culture;
		std::string religion;
		double upperRatio = 1.0;
		double middleRatio = 1.0;
		double lowerRatio = 1.0;
};

}



#endif // POP_RATIO_H_