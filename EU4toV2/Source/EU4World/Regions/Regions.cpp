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



#include "Regions.h"
#include "Areas.h"



EU4::Regions::Regions(const EU4::areas& areas, std::istream& regionsFile)
{
	registerKeyword(std::regex("\\w+_region"), [this, areas](const std::string & areaName, std::istream & areasFile)
	{
		EU4::region newRegion(areasFile);
		newRegion.addProvinces(areas);
		regions.insert(make_pair(areaName, newRegion));
	});

	parseStream(regionsFile);

	auto theAreas = areas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>> & theArea)
	{
		regions.insert(make_pair(theArea.first, EU4::region(theArea.second)));
	});
}


EU4::Regions::Regions(const EU4::areas& areas)
{
	auto theAreas = areas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>> & theArea)
	{
		regions.insert(make_pair(theArea.first, EU4::region(theArea.second)));
	});
}


bool EU4::Regions::provinceInRegion(int province, const std::string& regionName) const
{
	auto region = regions.find(regionName);
	if (region != regions.end())
	{
		return region->second.containsProvince(province);
	}
	else
	{
		return false;
	}
}