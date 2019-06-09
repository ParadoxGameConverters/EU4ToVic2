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



#include "Provinces.h"
#include "Log.h"
#include <stdexcept>
#include <string>



EU4::Provinces::Provinces(std::istream& theStream)
{
	registerKeyword(std::regex("-[0-9]+"), [this](const std::string& numberString, std::istream& theStream) {
		Province newProvince(numberString, theStream);
		provinces.insert(std::make_pair(newProvince.getNum(), std::move(newProvince)));
	});
	parseStream(theStream);
}


EU4::Province& EU4::Provinces::getProvince(int provinceNumber)
{
	std::map<int, Province>::iterator province = provinces.find(provinceNumber);
	if (province == provinces.end())
	{
		std::range_error exception(std::string("Old province ") + std::to_string(provinceNumber) + std::string(" does not exist (bad mapping?)"));
		throw exception;
	}
	else
	{
		return province->second;
	}
}


void EU4::Provinces::checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const
{
	for (auto& province: provinces)
	{
		auto Vic2Provinces = provinceMapper.getVic2ProvinceNumbers(province.first);
		if (Vic2Provinces.size() == 0)
		{
			LOG(LogLevel::Warning) << "No mapping for province " << province.first;
		}
	}
}