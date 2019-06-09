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



#ifndef PROVINCE_MAPPER_H
#define PROVINCE_MAPPER_H



#include "ProvinceMappingsVersion.h"
#include "../../Configuration.h"
#include "newParser.h"
#include <map>
#include <memory>
#include <unordered_set>
#include <vector>



namespace EU4
{

class Version;

}
class Object;



namespace mappers
{

class ProvinceMapper: commonItems::parser
{
	public:
		ProvinceMapper(std::istream& theStream, const Configuration& configuration);

		std::vector<int> getVic2ProvinceNumbers(int EU4ProvinceNumber) const;
		std::vector<int> getEU4ProvinceNumbers(int Vic2ProvinceNumber) const;
		bool isProvinceResettable(int Vic2ProvinceNumber, const std::string& region) const;

	private:
		ProvinceMappingsVersion getMappingsVersion(
			const std::map<EU4::Version, ProvinceMappingsVersion>& mappingsVersions,
			const Configuration& configuration
		);
		void createMappings(const ProvinceMappingsVersion& provinceMappingsVersion);
		void addProvincesToResettableRegion(const std::string& regionName, const std::vector<int>& provinces);

		std::map<int, std::vector<int>> Vic2ToEU4ProvinceMap;
		std::map<int, std::vector<int>> EU4ToVic2ProvinceMap;
		std::map<std::string, std::set<int>> resettableProvinces;
};

}



#endif // PROVINCE_MAPPER_H