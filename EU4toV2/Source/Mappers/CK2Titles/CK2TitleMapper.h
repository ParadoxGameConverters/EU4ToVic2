#ifndef CK2_TITLE_MAPPER_H
#define CK2_TITLE_MAPPER_H

#include <map>
#include <optional>
#include <random>
#include <set>
#include "newParser.h"

namespace mappers
{
	class CK2TitleMapper: commonItems::parser
	{
	public:
		CK2TitleMapper();
		CK2TitleMapper(std::istream& theStream);

		std::optional<std::string> getTitle(std::string name) const;
		bool doesTitleExist(const std::string& title) const;
		std::optional<std::string> getRandomIslamicFlag() const;
		std::optional<std::string> getRandomIndianFlag() const;

	private:
		void registerKeys();
		std::map<std::string, std::string> titleMap; // <name, title>
		std::set<std::string> titles;
		std::vector<std::string> islamicFlags;
		std::vector<std::string> indianFlags;
	};
}



#endif // CK2_TITLE_MAPPER_H