#ifndef EU4_MOD_H
#define EU4_MOD_H

#include "Parser.h"

namespace EU4
{
	class Mod: commonItems::parser
	{
	public:
		explicit Mod(std::istream& theStream);
		
		[[nodiscard]] const auto& getName() const { return name; }
		[[nodiscard]] const auto& getPath() const { return path; }
		[[nodiscard]] auto isValid() const { return !name.empty() && !path.empty(); }
		[[nodiscard]] auto isCompressed() const { return compressed; }

	private:
		std::string name;
		std::string path;
		bool compressed = false;
	};
}

#endif // EU4_MOD_H