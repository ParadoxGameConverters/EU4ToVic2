#ifndef RELIGIONS_H
#define RELIGIONS_H

#include "newParser.h"
#include "Religion.h"
#include <map>
#include <optional>

namespace EU4
{
	class Religions: commonItems::parser
	{
	public:
		Religions();
		Religions(std::istream& theStream);
		std::optional<Religion> getReligion(std::string name) const;
		const std::map<std::string, Religion>& getAllReligions() const { return theReligions; }

	private:
		void registerKeys();
		std::map<std::string, Religion> theReligions;
	};
}

#endif // RELIGIONS_H