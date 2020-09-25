#ifndef VANILLA_WORLD_H
#define VANILLA_WORLD_H

#include "../Province/Province.h"
#include "../../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../../Mappers/StateMapper/StateMapper.h"

namespace V2
{

	class VanillaWorld
	{
	public:
		VanillaWorld();

		void determineProvinceNumbers();

		[[nodiscard]] const auto& getProvinces() const { return provinces; }
		[[nodiscard]] const auto& getProvinceMapper() const { return provinceMapper; }
		[[nodiscard]] const auto& getStateMapper() const { return stateMapper; }

		void verifyMappings();

	private:
		std::vector<int> provinces;

		mappers::StateMapper stateMapper;
		mappers::ProvinceMapper provinceMapper;
	};
}


#endif // VANILLA_WORLD_H