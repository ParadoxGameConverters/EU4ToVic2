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
#include "Log.h"
#include "ParserHelpers.h"
#include "Object.h"
#include "ParadoxParserUTF8.h"



Vic2CultureUnionMapper::Vic2CultureUnionMapper()
{
	shared_ptr<Object> unionsMapObj = parser_UTF8::doParseFile("unions.txt");
	if (unionsMapObj == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file unions.txt";
		exit(-1);
	}
	if (unionsMapObj->getLeaves().size() < 1)
	{
		LOG(LogLevel::Error) << "Failed to parse unions.txt";
		exit(-1);
	}

	initUnionMap(unionsMapObj->getLeaves()[0]);
}


void Vic2CultureUnionMapper::initUnionMap(shared_ptr<Object> obj)
{
	for (auto rule: obj->getLeaves())
	{
		vector<string> tags;
		string culture;
		for (auto ruleItems: rule->getLeaves())
		{
			if (ruleItems->getKey() == "tag")
			{
				tags.push_back(ruleItems->getLeaf());
			}
			if (ruleItems->getKey() == "culture")
			{
				culture = ruleItems->getLeaf();
			}
		}

		unionMap.insert(make_pair(culture, tags));
	}
}


vector<string> Vic2CultureUnionMapper::getCoreForCulture(const string& culture)
{
	vector<string> empty;

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


Vic2CultureUnionMapperFile::Vic2CultureUnionMapperFile()
{
	registerKeyword(std::regex("unions"), [this](const std::string& unused, std::istream& theStream){
		theCultureUnionMapper = std::make_unique<Vic2CultureUnionMapper>();
		commonItems::ignoreItem(unused, theStream);
	});

	LOG(LogLevel::Info) << "Parsing union mappings";
	parseFile("unions.txt");
}