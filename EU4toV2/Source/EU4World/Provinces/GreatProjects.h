#ifndef GREATPROJECTS_H
#define GREATPROJECTS_H

#include "newParser.h"
#include <set>

namespace EU4
{
	class GreatProjects : commonItems::parser
	{
		public:
			GreatProjects() = default;
			GreatProjects(std::istream& theStream);
			bool hasGreatProject(const std::string& greatProject) const { return greatProjects.count(greatProject) > 0; }
		
		private:
			std::set<std::string> greatProjects;
	};
}

#endif // GREATPROJECTS_H