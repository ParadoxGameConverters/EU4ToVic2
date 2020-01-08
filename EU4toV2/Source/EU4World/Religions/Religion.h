#ifndef RELIGION_H
#define RELIGION_H

#include <string>

namespace EU4
{
	class Religion
	{
	public:
		Religion(std::string _name, std::string _group): name(_name), group(_group) {}

		// exactly one of these four functions should return true for any given pairing
		bool isSameReligion(const Religion& other) const;	// e.g. catholic <-> catholic
		bool isRelatedTo(const Religion& other) const;		// e.g. orthodox <-> catholic
		bool isInfidelTo(const Religion& other) const;		// e.g. sunni <-> catholic

	private:
		std::string name;
		std::string group;
	};
}

#endif // RELIGION_H
