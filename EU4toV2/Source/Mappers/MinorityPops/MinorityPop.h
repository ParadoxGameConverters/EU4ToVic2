#ifndef MINORITY_POP_H
#define MINORITY_POP_H

#include "Parser.h"

namespace mappers
{
	class MinorityPop : commonItems::parser
	{
	public:
		explicit MinorityPop(std::istream& theStream);
		
		[[nodiscard]] const auto& getReligion() const { return religion; }
		[[nodiscard]] const auto& getCulture() const { return culture; }

	private:
		std::string religion;
		std::string culture;
	};
};

#endif // MINORITY_POP_H
