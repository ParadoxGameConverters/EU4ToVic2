#ifndef VERSION_PARSER_H
#define VERSION_PARSER_H

#include "newParser.h"

namespace mappers
{
	class VersionParser : commonItems::parser
	{
	public:
		VersionParser();
		VersionParser(std::istream& theStream);
		std::string name;
		std::string version;
		std::string descriptionLine;

		friend std::ostream& operator<<(std::ostream& output, const VersionParser& versionParser);

	private:
		void registerKeys();
	};
}

#endif // VERSION_PARSER_H