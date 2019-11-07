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



#include "Modifiers.h"
#include "ParserHelpers.h"



EU4::Modifiers::Modifiers(std::istream& theStream)
{
	addModifiers(theStream);
}


void EU4::Modifiers::addModifiers(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9\\_]+"), [this](const std::string& modifierName, std::istream& theStream) {
		Modifier modifier(theStream);
		modifiers.insert(std::make_pair(modifierName, modifier));
	});
	parseStream(theStream);
}


std::optional<EU4::Modifier> EU4::Modifiers::getModifier(const std::string& modifierName) const
{
	if (modifiers.count(modifierName) > 0)
	{
		return std::make_optional(modifiers.at(modifierName));
	}
	else
	{
		return std::nullopt;
	}
}