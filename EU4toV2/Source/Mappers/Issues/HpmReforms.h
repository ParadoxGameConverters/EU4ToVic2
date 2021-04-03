#ifndef HPM_REFORMS_H
#define HPM_REFORMS_H

#include "Parser.h"

namespace mappers
{
class HpmReforms: commonItems::parser
{
  public:
	HpmReforms();
	explicit HpmReforms(std::istream& theStream);

	[[nodiscard]] const auto& getReforms() const { return reforms; }
	[[nodiscard]] const auto& getUncivReforms() const { return uncivs; }

  private:
  	void registerKeys();
  	
	std::map<std::string, std::string> reforms;
	std::map<std::string, std::string> uncivs;
};
} // namespace mappers

#endif // HPM_REFORMS_H