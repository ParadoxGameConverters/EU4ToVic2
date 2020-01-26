#ifndef RELIGION_H
#define RELIGION_H

#include <string>

namespace EU4
{
	class Religion
	{
	public:
		Religion(std::string _name, std::string _group): name(std::move(_name)), group(std::move(_group)) {}

		[[nodiscard]] bool isSameReligion(const Religion& other) const; // e.g. catholic <-> catholic
		[[nodiscard]] bool isRelatedTo(const Religion& other) const; // e.g. orthodox <-> catholic
		[[nodiscard]] bool isInfidelTo(const Religion& other) const; // e.g. sunni <-> catholic

	private:
		std::string name;
		std::string group;
	};
}

#endif // RELIGION_H
