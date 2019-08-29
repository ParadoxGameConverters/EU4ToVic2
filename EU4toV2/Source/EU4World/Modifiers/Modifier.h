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



#ifndef MODIFIER_H
#define MODIFIER_H



#include "newParser.h"
#include <map>
#include <string>



namespace EU4
{

class Modifier: commonItems::parser
{
	public:
		Modifier() = default;
		~Modifier() = default;
		Modifier(const Modifier&) = default;
		Modifier(Modifier&&) = default;
		Modifier& operator=(const Modifier&) = default;
		Modifier& operator=(Modifier&&) = default;

		Modifier(std::istream& theStream);

		double getEffectAmount(const std::string& modifier) const;

		const std::map<std::string, double>& getAllEffects() const { return effects; }

	private:
		std::map<std::string, double> effects;
};

}



#endif // MODIFIER_H