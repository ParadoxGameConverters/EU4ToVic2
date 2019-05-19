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



#ifndef SLAVE_CULTURE_MAPPER_H
#define SLAVE_CULTURE_MAPPER_H



#include "newParser.h"
#include "CultureMapping.h"
#include "../EU4World/Regions/Regions.h"
#include <istream>
#include <optional>
#include <string>
#include <vector>



namespace mappers
{

class SlaveCultureMapper: commonItems::parser
{
	public:
		SlaveCultureMapper(std::istream& theStream);

		std::optional<std::string> cultureMatch(
			const EU4::Regions& EU4Regions,
			const std::string& culture,
			const std::string& religion,
			int EU4Province = -1,
			const std::string& ownerTag = ""
		) const;

	private:
		std::vector<cultureMapping> cultureMap;
};

}



#endif // SLAVE_CULTURE_MAPPER_H