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



#ifndef CULTURE_MAPPING_H_
#define CULTURE_MAPPING_H_



#include "../EU4World/Regions/Regions.h"
#include <map>
#include <optional>
#include <string>



namespace mappers
{

enum class distinguisherTypes
{
	owner,
	religion,
	province,
	region
};

class cultureMapping
{
	public:
		cultureMapping(
			const std::string& sourceCulture,
			const std::string& destinationCulture,
			const std::map<distinguisherTypes, std::string>& distinguishers
		);

		std::optional<std::string> cultureMatch(
			const EU4::Regions& EU4Regions,
			const std::string& culture,
			const std::string& religion,
			int EU4Province,
			const std::string& ownerTag
		);

	private:
		bool distinguishersMatch(
			const EU4::Regions& EU4Regions,
			const std::map<distinguisherTypes, std::string>& distinguishers,
			const std::string& religion,
			int EU4Province,
			const std::string& ownerTag
		);

		std::string sourceCulture;
		std::string destinationCulture;
		std::map<distinguisherTypes, std::string> distinguishers;
};

}



#endif // CULTURE_MAPPING_H_