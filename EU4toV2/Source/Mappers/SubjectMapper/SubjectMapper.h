#ifndef SUBJECT_MAPPER_H
#define SUBJECT_MAPPER_H

#include "newParser.h"
#include <set>

namespace mappers
{
	class SubjectMapper : commonItems::parser
	{
	public:
		SubjectMapper();
		SubjectMapper(std::istream& theStream);
		bool isSubjectInColonies(const std::string& subject) const { return colonies.count(subject) > 0; }
		bool isSubjectInOnesiders(const std::string& subject) const { return onesiders.count(subject) > 0; }
		bool isSubjectInDoublesiders(const std::string& subject) const { return doublesiders.count(subject) > 0; }
		bool isSubjectInTributaries(const std::string& subject) const { return tributaries.count(subject) > 0; }
		bool isSubjectInVassals(const std::string& subject) const { return vassals.count(subject) > 0; }

	private:
		void registerKeys();
		std::set<std::string> colonies;
		std::set<std::string> onesiders;
		std::set<std::string> doublesiders;
		std::set<std::string> tributaries;
		std::set<std::string> vassals;
	};
}

#endif // SUBJECT_MAPPER_H
