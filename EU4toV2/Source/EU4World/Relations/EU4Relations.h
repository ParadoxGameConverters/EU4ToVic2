#ifndef EU4RELATIONS_H_
#define EU4RELATIONS_H_

#include "Date.h"
#include "newParser.h"
#include "EU4RelationDetails.h"
#include <map>

namespace EU4
{
	class EU4Relations : commonItems::parser
	{
	public:
		EU4Relations(std::istream& theStream);
		std::map<std::string, EU4RelationDetails> getRelations() const { return relations; }

	private:
		std::map<std::string, EU4RelationDetails> relations;
	};
}



#endif