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



#include "PopRatio.h"



void EU4::PopRatio::decay(float diffInYears, const EU4::PopRatio& currentPop)
{
	double upperNonCurrentRatio = (1.0 - currentPop.upperRatio);
	double middleNonCurrentRatio = (1.0 - currentPop.middleRatio);
	double lowerNonCurrentRatio = (1.0 - currentPop.lowerRatio);

	upperRatio -= .0025 * diffInYears * upperRatio / upperNonCurrentRatio;
	middleRatio -= .0025 * diffInYears * middleRatio / middleNonCurrentRatio;
	lowerRatio -= .0025 * diffInYears * lowerRatio / lowerNonCurrentRatio;
}



void EU4::PopRatio::increase(float diffInYears)
{
	upperRatio += .0025 * diffInYears;
	middleRatio += .0025 * diffInYears;
	lowerRatio += .0025 * diffInYears;
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