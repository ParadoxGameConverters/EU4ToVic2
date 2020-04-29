#ifndef UNIT_TYPE_MAPPER_H
#define UNIT_TYPE_MAPPER_H

#include "Parser.h"
#include "UnitType.h"

namespace mappers
{
	typedef std::map<std::string, UnitType> RegimentTypeMap;

	class UnitTypeMapper : commonItems::parser
	{
	public:
		UnitTypeMapper() = default;
		void initUnitTypeMapper();
		
		[[nodiscard]] const auto& getUnitTypeMap() const { return unitTypeMap; }

	private:
		RegimentTypeMap unitTypeMap;
		void addUnitFileToRegimentTypeMap(const std::string& directory, const std::string& filename);
	};
};

#endif // UNIT_TYPE_MAPPER_H
