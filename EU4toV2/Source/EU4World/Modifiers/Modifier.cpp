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



#include "Modifier.h"
#include "ParserHelpers.h"
#include "Log.h"



EU4::Modifier::Modifier(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& effect, std::istream& theStream) {
		commonItems::singleDouble amount(theStream);

		if (effects.count(effect) == 0)
		{
			effects.insert(std::make_pair(effect, amount.getDouble()));
		}
		else
		{
			LOG(LogLevel::Warning) << "Multiple instances of effect " << effect << " in modifier.";
		}
	});

	parseStream(theStream);
}


double EU4::Modifier::getEffectAmount(const std::string& theEffect) const
{
	if (effects.count(theEffect) > 0)
	{
		return effects.at(theEffect);
	}
	else
	{
		return 0;
	}
}