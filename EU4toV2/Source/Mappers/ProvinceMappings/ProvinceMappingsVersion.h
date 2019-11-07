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



#ifndef PROVINCE_MAPPING_VERSION_H
#define PROVINCE_MAPPING_VERSION_H



#include "newParser.h"
#include "ProvinceMapping.h"
#include "../../EU4World/EU4Version.h"
#include <vector>



namespace mappers
{

class ProvinceMappingsVersion: commonItems::parser
{
	public:
		ProvinceMappingsVersion(const std::string& versionString, std::istream& theStream);

		const EU4::Version& getVersion() const { return version; }
		std::vector<ProvinceMapping> getMappings() const { return mappings; }

	private:
		EU4::Version version;
		std::vector<ProvinceMapping> mappings;
};

}



#endif // PROVINCE_MAPPING_VERSION_H