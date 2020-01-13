#ifndef GOVERNMENT_MAPPER_H
#define GOVERNMENT_MAPPER_H

#include <map>
#include "newParser.h"

namespace mappers
{
	class GovernmentMapper : commonItems::parser
	{
	public:
		GovernmentMapper();
		GovernmentMapper(std::istream& theStream);
		std::string matchGovernment(const std::string& sourceGovernment) const;

	private:
		void registerKeys();
		std::map<std::string, std::string> governmentMap;
	};
};

#endif // GOVERNMENT_MAPPER_H