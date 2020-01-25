#ifndef EU4_COUNTRIES_H
#define EU4_COUNTRIES_H

#include "newParser.h"
#include "../EU4Version.h"
#include <map>
#include <memory>
#include "../../Mappers/IdeaEffects/IdeaEffectMapper.h"
#include "../../Mappers/CultureGroups/CultureGroups.h"
#include "EU4Country.h"

namespace EU4
{
	class Countries: public commonItems::parser
	{
	public:
		Countries(const Version& theVersion, 
			std::istream& theStream, 
			const mappers::IdeaEffectMapper& ideaEffectMapper, 
			const mappers::CultureGroups& cultureGroupsMapper);
		
		[[nodiscard]] const auto& getTheCountries() const { return theCountries; }

	private:
		std::map<std::string, std::shared_ptr<Country>> theCountries;
	};
}

#endif // EU4_COUNTRIES_H