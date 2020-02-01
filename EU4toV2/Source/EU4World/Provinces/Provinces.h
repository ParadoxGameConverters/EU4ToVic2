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
		explicit Provinces(std::istream& theStream);
		
		[[nodiscard]] auto geTotalProvinceWeights() const { return totalProvinceWeights; }
		[[nodiscard]] const auto& getAllProvinces() const { return provinces; }
		std::shared_ptr<Province> getProvince(int provinceNumber);
		
		void determineTotalProvinceWeights(const Configuration& configuration);

	private:
		void logTotalProvinceWeights() const;

		std::map<int, std::shared_ptr<Province>> provinces;
		double totalProvinceWeights = 0.0;
	};
}

#endif // PROVINCES_H