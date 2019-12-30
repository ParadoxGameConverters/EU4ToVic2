#ifndef MINORITY_POP_H
#define MINORITY_POP_H

#include <memory>
#include "newParser.h"


namespace mappers
{
	class MinorityPop : commonItems::parser
	{
	public:
		MinorityPop(std::istream& theStream);
		std::string getReligion() const { return religion; }
		std::string getCulture() const { return culture; }

	private:
		std::string religion;
		std::string culture;
	};
};

#endif // MINORITY_POP_H
