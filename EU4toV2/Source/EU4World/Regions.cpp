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
#include "../Configuration.h"
#include "Regions/Areas.h"
#include "Regions/Region.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include <algorithm>
#include <fstream>




EU4::Regions::Regions():
	EU4RegionsMap(),
	regions()
{
	LOG(LogLevel::Info) << "Parsing EU4 regions";

	if (theConfiguration.getEU4Version() >= EU4::Version("1.14"))
	{
		initEU4RegionsNewVersion();
	}
	else
	{
		initEU4RegionsOldVersion();
	}
}


void EU4::Regions::initEU4RegionsOldVersion()
{
	regions.clear();

	std::ifstream theStream(theConfiguration.getEU4Path() + "/map/region.txt");
	EU4::areas installedAreas(theStream);
	theStream.close();

	auto theAreas = installedAreas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>>& theArea)
	{
		regions.insert(make_pair(theArea.first, EU4::region(theArea.second)));
	});

	for (auto itr: theConfiguration.getEU4Mods())
	{
		if (!Utils::DoesFileExist(itr + "/map/region.txt"))
		{
			continue;
		}

		regions.clear();

		std::ifstream modStream(itr + "/map/region.txt");
		EU4::areas modAreas(modStream);
		modStream.close();

		auto theAreas = modAreas.getAreas();
		std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>>& theArea)
		{
			regions.insert(make_pair(theArea.first, EU4::region(theArea.second)));
		});
	}
}


void EU4::Regions::initEU4RegionsNewVersion()
{
	std::ifstream theStream(theConfiguration.getEU4Path() + "/map/area.txt");
	EU4::areas installedAreas(theStream);
	theStream.close();

	initEU4RegionsFile(installedAreas, (theConfiguration.getEU4Path() + "/map/region.txt"));

	for (auto itr: theConfiguration.getEU4Mods())
	{
		if (!Utils::DoesFileExist(itr + "/map/area.txt") || !Utils::DoesFileExist(itr + "/map/region.txt"))
		{
			continue;
		}

		std::ifstream modStream(itr + "/map/area.txt");
		EU4::areas modAreas(modStream);
		modStream.close();

		initEU4RegionsFile(modAreas, (itr + "/map/region.txt"));
	}
}


void EU4::Regions::initEU4RegionsFile(const EU4::areas& areas, const std::string& regionsFilename)
{
	regions.clear();

	registerKeyword(std::regex("\\w+_region"), [this, areas](const std::string& areaName, std::istream& areasFile)
	{
		EU4::region newRegion(areasFile);
		newRegion.addProvinces(areas);
		regions.insert(make_pair(areaName, newRegion));
	});

	parseFile(regionsFilename);

	auto theAreas = areas.getAreas();
	std::for_each(theAreas.begin(), theAreas.end(), [this](const std::pair<std::string, std::set<int>>& theArea)
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