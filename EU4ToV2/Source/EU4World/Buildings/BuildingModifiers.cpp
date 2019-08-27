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



#include "BuildingModifiers.h"
#include "ParserHelpers.h"
#include "Log.h"



EU4::BuildingModifiers::BuildingModifiers(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& modifier, std::istream& theStream) {
		commonItems::singleDouble amount(theStream);

		if (modifiers.count(modifier) == 0)
		{
			modifiers.insert(std::make_pair(modifier, amount.getDouble()));
		}
		else
		{
			LOG(LogLevel::Warning) << "Multiple instances of modifier " << modifier << " in building.";
		}
	});

	parseStream(theStream);
}


double EU4::BuildingModifiers::getModifierAmount(const std::string& theModifier)
{
	if (modifiers.count(theModifier) > 0)
	{
		return modifiers.at(theModifier);
	}
	else
	{
		return 0;
	}
}