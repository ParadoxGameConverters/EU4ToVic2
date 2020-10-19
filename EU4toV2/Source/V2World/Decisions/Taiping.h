#ifndef TAIPING_CSA_H
#define TAIPING_CSA_H

#include "Parser.h"

namespace V2
{
class Taiping: commonItems::parser
{
  public:
	Taiping() = default;
	explicit Taiping(std::istream& theStream);

	[[nodiscard]] const auto& getTagEffectMap() const { return tagEffectMap; }
	[[nodiscard]] const auto& getNonCountrySpecificEffects() const { return nonCountrySpecificEffects; }

	[[nodiscard]] const auto& getTag() const { return tag; }
	[[nodiscard]] const auto& getHistory() const { return history; }
	[[nodiscard]] const auto& getCountries() const { return countries; }
	[[nodiscard]] const auto& getCountryCores() const { return countryCores; }
	[[nodiscard]] const auto& getCore() const { return core; }

  private:
  	std::map<std::string, std::string> tagEffectMap;
  	std::vector<std::string> nonCountrySpecificEffects;
	std::vector<std::string> countryCores;

  	std::vector<std::string> bodies;
  	std::string tag;
  	std::map<std::string, std::string> history;
	std::map<std::string, std::map<std::string, std::string>> countries;
	std::string core;
};

class Limit: commonItems::parser
{
public:
	Limit() = default;
	Limit(std::istream& theStream);

	[[nodiscard]] const auto& getTag() const { return tag; }

private:
	std::string tag;	
};
} // namespace V2


#endif // TAIPING_CSA_H