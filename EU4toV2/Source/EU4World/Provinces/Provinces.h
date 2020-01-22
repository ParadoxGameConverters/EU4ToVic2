#ifndef PROVINCES_H
#define PROVINCES_H

#include "EU4Province.h"
#include "newParser.h"
#include "../../Mappers/Buildings/Buildings.h"
#include "../../Configuration.h"
#include <map>

namespace EU4
{
	class Provinces: commonItems::parser
	{
	public:
		Provinces(std::istream& theStream, const mappers::Buildings& buildingTypes, const Modifiers& modifierTypes);
		std::shared_ptr<Province> getProvince(int provinceNumber);

		const std::map<int, std::shared_ptr<Province>>& getAllProvinces() const { return provinces; }
		double geTotalProvinceWeights() const { return totalProvinceWeights; };
		void determineTotalProvinceWeights(const Configuration& configuration);

	private:
		void logTotalProvinceWeights() const;

		std::map<int, std::shared_ptr<Province>> provinces;
		double totalProvinceWeights = 0.0;
	};
}

#endif // PROVINCES_H