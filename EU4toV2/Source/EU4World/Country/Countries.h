#ifndef EU4_COUNTRIES_H
#define EU4_COUNTRIES_H

#include "newParser.h"
#include "../EU4Version.h"
#include <map>
#include <memory>
#include "../../Mappers/Ideas/IdeaEffectMapper.h"
#include "../../Mappers/CultureGroups/CultureGroups.h"
#include "EU4Country.h"

namespace EU4
{
	class countries: public commonItems::parser
	{
	public:
		countries(const EU4::Version& theVersion, std::istream& theStream, const mappers::IdeaEffectMapper& ideaEffectMapper, const mappers::CultureGroups& cultureGroupsMapper);
		std::map<std::string, std::shared_ptr<EU4::Country>> getTheCountries() const { return theCountries; }

	private:
		std::map<std::string, std::shared_ptr<EU4::Country>> theCountries;
	};
}

#endif // EU4_COUNTRIES_H