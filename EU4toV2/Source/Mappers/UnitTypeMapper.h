#ifndef UNIT_TYPE_MAPPER_H_
#define UNIT_TYPE_MAPPER_H_

#include "newParser.h"
#include "UnitType.h"


namespace mappers
{
	typedef std::map<std::string, mappers::UnitType> RegimentTypeMap;

	class UnitTypeMapper : commonItems::parser
	{
	public:
		UnitTypeMapper();
		const RegimentTypeMap& getUnitTypeMap() const { return unitTypeMap; }

	private:
		RegimentTypeMap unitTypeMap;

		void AddUnitFileToRegimentTypeMap(const std::string& directory, const std::string& filename);
	};
};

#endif // UNIT_TYPE_MAPPER_H_
