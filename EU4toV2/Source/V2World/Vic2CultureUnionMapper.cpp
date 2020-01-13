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



#include "Vic2CultureUnionMapper.h"
#include "Vic2CultureUnion.h"
#include "Log.h"
#include "ParserHelpers.h"



Vic2::CultureUnionMapper::CultureUnionMapper(std::istream& theStream)
{
	registerKeyword(std::regex("link"), [this](const std::string& unused, std::istream& theStream){
		Vic2::CultureUnion newUnion(theStream);
		unionMap.insert(newUnion.takeTheUnion());
	});

	parseStream(theStream);
}


std::vector<std::string> Vic2::CultureUnionMapper::getCoreForCulture(const std::string& culture)
{
	std::vector<std::string> empty;

	auto mapping = unionMap.find(culture);
	if (mapping == unionMap.end())
	{
		return empty;
	}
	else
	{
		return mapping->second;
	}
}


Vic2::CultureUnionMapperFile::CultureUnionMapperFile()
{
	registerKeyword(std::regex("unions"), [this](const std::string& unused, std::istream& theStream){
		theCultureUnionMapper = std::make_unique<Vic2::CultureUnionMapper>(theStream);
	});
	registerKeyword(std::regex("nationals"), [this](const std::string& unused, std::istream& theStream) {
		theNationalsMapper = std::make_unique<Vic2::CultureUnionMapper>(theStream);
	});

	LOG(LogLevel::Info) << "Parsing union mappings";
	parseFile("configurables/unions.txt");
	LOG(LogLevel::Info) << "Parsing national mappings";
	parseFile("configurables/nationals.txt");
}
