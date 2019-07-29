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



#ifndef EU4_COUNTRIES_H_
#define EU4_COUNTRIES_H_



#include "newParser.h"
#include "EU4Version.h"
#include <map>
#include <memory>
#include <string>



namespace mappers
{
class IdeaEffectMapper;
}


namespace EU4
{
class Country;


class countries: public commonItems::parser
{
	public:
		countries(
			const EU4::Version& theVersion,
			std::istream& theStream,
			const mappers::IdeaEffectMapper& ideaEffectMapper
		);

		std::map<std::string, std::shared_ptr<EU4::Country>> getTheCountries() const { return theCountries; }

	private:
		std::map<std::string, std::shared_ptr<EU4::Country>> theCountries;
};

}



#endif // EU4_COUNTRIES_H_