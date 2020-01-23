#ifndef EU4_REGION_H
#define EU4_REGION_H

#include "newParser.h"
#include "Areas.h"
#include <set>

namespace EU4
{
	class Region: commonItems::parser
	{
	public:
		Region(std::istream& theStream);
		Region(const std::set<int>& _provinces);
		bool regionContainsProvince(int province) const;
		bool areaContainsProvince(const std::string& areaName, int province) const;
		void addProvinces(const Areas& areas);
		const std::map<std::string, std::set<int>>& getAreaProvinces() const{ return areaProvinces; }
		const std::set<std::string>& getAreaNames() const { return areaNames; }
		const std::string& getName() const { return name; }

	private:
		std::map<std::string, std::set<int>> areaProvinces;
		std::set<std::string> areaNames;
		std::string name;
	};
}

#endif // EU4_REGION_H