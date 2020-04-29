#ifndef RELIGIONS_H
#define RELIGIONS_H

#include "Parser.h"
#include "Religion.h"
#include <map>
#include <optional>

namespace EU4
{
	class Religions: commonItems::parser
	{
	public:
		Religions();
		explicit Religions(std::istream& theStream);
		
		[[nodiscard]] std::optional<Religion> getReligion(const std::string& name) const;
		[[nodiscard]] const auto& getAllReligions() const { return theReligions; }

	private:
		void registerKeys();
		
		std::map<std::string, Religion> theReligions;
	};
}

#endif // RELIGIONS_H