#ifndef MOD_REFORMS_H
#define MOD_REFORMS_H

#include "Parser.h"

namespace mappers
{
class ModReforms: commonItems::parser
{
  public:
	ModReforms();
	explicit ModReforms(std::istream& theStream);

	[[nodiscard]] const auto& getReforms() const { return reforms; }
	[[nodiscard]] const auto& getUncivReforms() const { return uncivs; }

  private:
  	void registerKeys();
  	
	std::map<std::string, std::string> reforms;
	std::map<std::string, std::string> uncivs;
};
} // namespace mappers

#endif // MOD_REFORMS_H