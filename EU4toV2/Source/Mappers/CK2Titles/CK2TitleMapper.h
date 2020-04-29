#ifndef CK2_TITLE_MAPPER_H
#define CK2_TITLE_MAPPER_H

#include <map>
#include <optional>
#include <set>
#include "Parser.h"

namespace mappers
{
	class CK2TitleMapper: commonItems::parser
	{
	public:
		CK2TitleMapper();
		explicit CK2TitleMapper(std::istream& theStream);

		[[nodiscard]] std::optional<std::string> getTitle(const std::string& name) const;
		[[nodiscard]] bool doesTitleExist(const std::string& title) const;
		[[nodiscard]] std::optional<std::string> getRandomIslamicFlag() const;
		[[nodiscard]] std::optional<std::string> getRandomIndianFlag() const;

	private:
		void registerKeys();
		
		std::map<std::string, std::string> titleMap; // <name, title>
		std::set<std::string> titles;
		std::vector<std::string> islamicFlags;
		std::vector<std::string> indianFlags;
	};
}



#endif // CK2_TITLE_MAPPER_H