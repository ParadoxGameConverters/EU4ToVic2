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



#include "SlaveCultureMapper.h"
#include "CultureMappingRule.h"
#include "Log.h"



mappers::SlaveCultureMapper::SlaveCultureMapper(std::istream& theStream)
{
	LOG(LogLevel::Info) << "Parsing slave culture mappings";

	registerKeyword(std::regex("link"), [this](const std::string& unused, std::istream& theStream)
		{
			CultureMappingRule rule(theStream);
			auto newRules = rule.getMappings();
			for (auto newRule: newRules)
			{
				cultureMap.push_back(newRule);
			}
		}
	);

	parseStream(theStream);
}


std::optional<std::string> mappers::SlaveCultureMapper::cultureMatch(
	const EU4::Regions& EU4Regions,
	const std::string& culture,
	const std::string& religion,
	int EU4Province,
	const std::string& ownerTag
) const
{
	for (auto cultureMapping: cultureMap)
	{
		auto possibleMatch = cultureMapping.cultureMatch(EU4Regions, culture, religion, EU4Province, ownerTag);
		if (possibleMatch)
		{
			return possibleMatch;
		}
	}

	return {};
}