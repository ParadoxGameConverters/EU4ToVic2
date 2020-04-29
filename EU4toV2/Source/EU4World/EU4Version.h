#ifndef EU4_VERSION_H
#define EU4_VERSION_H

#include "Parser.h"
#include <string>
#include <ostream>

namespace EU4
{
	class Version: commonItems::parser
	{
	public:
		Version() = default;
		Version(const Version&) = default;
		Version(Version&&) = default;
		Version& operator=(const Version&) = default;
		Version& operator=(Version&&) = default;
		~Version() = default;

		explicit Version(std::string version);
		explicit Version(std::istream& theStream);

		bool operator >= (const Version& rhs) const;
		bool operator > (const Version& rhs) const;
		bool operator < (const Version& rhs) const;
		bool operator <= (const Version& rhs) const;
		bool operator == (const Version& rhs) const;
		bool operator != (const Version& rhs) const;

		friend std::ostream& operator<<(std::ostream&, const Version& version);

	private:
		int firstPart = 0;
		int secondPart = 0;
		int thirdPart = 0;
		int fourthPart = 0;
	};

	std::ostream& operator<<(std::ostream&, const Version& version);
}

#endif // EU4_VERSION_H