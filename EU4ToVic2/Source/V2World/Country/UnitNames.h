#ifndef UNIT_NAMES_H
#define UNIT_NAMES_H

#include "Parser.h"
#include <set>

namespace V2
{
class UnitNames: commonItems::parser
{
  public:
	UnitNames() = default;
	UnitNames(std::istream& theStream);

	std::string outputType(const std::vector<std::string>& shipType) const;
	bool isNameUsed(const std::string& name) { return usedNames.count(name); }
	void addUsedName(const std::string& name) { usedNames.insert(name); }

	[[nodiscard]] const auto& getManowars() const { return manowars; }
	[[nodiscard]] const auto& getFrigates() const { return frigates; }
	[[nodiscard]] const auto& getClipperTransports() const { return clipperTransports; }

	friend std::ostream& operator<<(std::ostream& output, const UnitNames& unitNames);

  private:
	std::vector<std::string> dreadnoughts;
	std::vector<std::string> battleships;
	std::vector<std::string> ironclads;
	std::vector<std::string> manowars;
	std::vector<std::string> cruisers;
	std::vector<std::string> frigates;
	std::vector<std::string> monitors;
	std::vector<std::string> clipperTransports;
	std::vector<std::string> steamTransports;
	std::vector<std::string> commerceRaiders;
	std::set<std::string> usedNames;
};
} // namespace V2


#endif // UNIT_NAMES_H