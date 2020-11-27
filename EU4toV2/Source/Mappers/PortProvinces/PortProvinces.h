#ifndef PORT_PROVINCES_H
#define PORT_PROVINCES_H
#include "Parser.h"
#include <set>

namespace mappers
{
// hack for naval bases.  not ALL naval bases are in port provinces, and if a navy spawns at a naval base in
// a non-port province, Vicky crashes....
class PortProvinces: commonItems::parser
{
  public:
	PortProvinces();
	explicit PortProvinces(std::istream& theStream);

	[[nodiscard]] auto isProvinceIDBlacklisted(const int& provinceID) const { return blacklist.contains(provinceID); }
	[[nodiscard]] auto isProvinceIDWhitelisted(const int& provinceID) const { return whitelist.contains(provinceID); }

  private:
	void registerKeys();

	std::set<int> whitelist;
	std::set<int> blacklist;
};
} // namespace mappers

#endif // PORT_PROVINCES_H
