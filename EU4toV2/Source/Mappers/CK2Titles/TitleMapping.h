#ifndef TITLE_MAPPING_H
#define TITLE_MAPPING_H

#include "newParser.h"

namespace mappers
{
	class TitleMapping : commonItems::parser
	{
	public:
		TitleMapping(std::istream& theSteam);

		bool hasIslamicRegion() const;
		bool hasIndianRegion() const;

		std::string getName() const { return name; }
		std::string getID() const { return ID; }

	private:
		std::string name;
		std::string ID;
		std::string region = "";
	};
}

#endif // TITLE_MAPPING_H