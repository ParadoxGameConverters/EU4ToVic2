#ifndef EU4_MOD_H
#define EU4_MOD_H

#include "newParser.h"

namespace EU4
{
	class Mod: commonItems::parser
	{
		public:
			Mod(std::istream& theStream);
			std::string getName() const { return name; }
			std::string getPath() const { return path; }
			bool isValid() const { return (name != "") && (path != ""); }
			bool isCompressed() const { return compressed; }

		private:
			std::string name;
			std::string path;
			bool compressed = false;
	};
}

#endif // EU4_MOD_H