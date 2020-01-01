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
		RegimentTypeMap getUnitTypeMap() const { return unitTypeMap; }

	private:
		RegimentTypeMap unitTypeMap;

		void AddUnitFileToRegimentTypeMap(std::string directory, std::string filename);
	};
};

#endif // UNIT_TYPE_MAPPER_H_
