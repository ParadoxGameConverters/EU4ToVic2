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



#include "StateMapper.h"
#include "../Configuration.h"
#include "Log.h"
#include "OSCompatibilityLayer.h"
#include "ParserHelpers.h"
#include <fstream>



Vic2::stateMapper::stateMapper(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& unused, std::istream& theStream){
		commonItems::intList provinces(theStream);

		std::vector<int> neighbors;
		for (auto province: provinces.getInts())
		{
			neighbors.push_back(province);
		}

		for (auto neighbor: neighbors)
		{
			stateProvincesMap.insert(std::make_pair(neighbor, neighbors));
		}
	});

	parseStream(theStream);
}


std::vector<int> Vic2::stateMapper::getAllProvincesInState(int province)
{
	auto mapping = stateProvincesMap.find(province);
	if (mapping != stateProvincesMap.end())
	{
		return mapping->second;
	}
	else
	{
		std::vector<int> empty;
		return empty;
	}
}


Vic2::stateMapperFile::stateMapperFile()
{
	LOG(LogLevel::Info) << "Parsing region structure";

	std::string filename;
	if (Utils::DoesFileExist("./blankMod/output/map/region.txt"))
	{
		filename = "./blankMod/output/map/region.txt";
	}
	else
	{
		filename = theConfiguration.getVic2Path() + "/map/region.txt";
	}

	std::ifstream theFile(filename);
	if (!theFile.is_open())
	{
		LOG(LogLevel::Error) << "Could not open " << filename << " for parsing.";
		return;
	}

	const char firstChar = theFile.peek();
	if (firstChar == '\xEF')
	{
		char bitBucket[3];
		theFile.read(bitBucket, sizeof(bitBucket));
	}

	theStateMapper = std::make_unique<stateMapper>(theFile);

	theFile.close();
}