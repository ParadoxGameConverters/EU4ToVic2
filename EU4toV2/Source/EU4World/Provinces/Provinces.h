#ifndef PROVINCES_H
#define PROVINCES_H

#include "EU4Province.h"
#include "newParser.h"
#include "../../Mappers/ProvinceMappings/ProvinceMapper.h"
#include "../../Configuration.h"
#include <map>

namespace EU4
{
	class Provinces: commonItems::parser
	{
		public:
			Provinces(std::istream& theStream, const Buildings& buildingTypes, const Modifiers& modifierTypes);
			Province& getProvince(int provinceNumber);

			std::map<int, Province>& getAllProvinces() { return provinces; }
			double geTotalProvinceWeights() const { return totalProvinceWeights; };
			void checkAllProvincesMapped(const mappers::ProvinceMapper& provinceMapper) const;
			void determineTotalProvinceWeights(const Configuration& configuration);

		private:
			void logTotalProvinceWeights() const;

			std::map<int, Province> provinces;
			double totalProvinceWeights = 0.0;
	};
}

#endif // PROVINCES_H