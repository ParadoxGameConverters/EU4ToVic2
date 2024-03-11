#ifndef EU4_COUNTRIES_H
#define EU4_COUNTRIES_H

#include "../../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "EU4Country.h"
#include "GameVersion.h"
#include "Parser.h"
#include <map>
#include <memory>

namespace EU4
{
class Countries: public commonItems::parser
{
  public:
	Countries(const GameVersion& theVersion, std::istream& theStream, const mappers::IdeaEffectMapper& ideaEffectMapper);

	[[nodiscard]] const auto& getTheCountries() const { return theCountries; }

  private:
	std::map<std::string, std::shared_ptr<Country>> theCountries;
};
} // namespace EU4

#endif // EU4_COUNTRIES_H