#ifndef PROVINCES_H
#define PROVINCES_H
#include "Configuration.h"
#include "EU4Province.h"
#include "Parser.h"
#include <map>

namespace mappers
{
class SuperGroupMapper;
}

namespace EU4
{
class Regions;
class Provinces: commonItems::parser
{
  public:
	explicit Provinces(std::istream& theStream);

	[[nodiscard]] auto geTotalProvinceWeights() const { return totalProvinceWeights; }
	[[nodiscard]] const auto& getAllProvinces() const { return provinces; }
	[[nodiscard]] const std::shared_ptr<Province>& getProvince(int provinceNumber) const;

	void determineTotalProvinceWeights(const Configuration& configuration);

  private:
	void registerKeys();
	void logTotalProvinceWeights() const;

	std::map<int, std::shared_ptr<Province>> provinces;
	double totalProvinceWeights = 0.0;
};
} // namespace EU4

#endif // PROVINCES_H