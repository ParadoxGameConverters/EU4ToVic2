#ifndef TAIPING_CSA_H
#define TAIPING_CSA_H

#include "Parser.h"

namespace V2
{
class TaipingCSA: commonItems::parser
{
  public:
	TaipingCSA() = default;
	explicit TaipingCSA(std::istream& theStream);

	[[nodiscard]] const auto& getTag() const { return tag; }
	[[nodiscard]] const auto& getHistory() const { return history; }
	[[nodiscard]] const auto& getCountries() const { return countries; }
	[[nodiscard]] const auto& getCountryCores() const { return countryCores; }
	[[nodiscard]] const auto& getCore() const { return core; }

  private:
  	std::string tag;
  	std::map<std::string, std::string> history;
	std::map<std::string, std::map<std::string, std::string>> countries;
	std::map<std::string, std::string> countryCores;
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