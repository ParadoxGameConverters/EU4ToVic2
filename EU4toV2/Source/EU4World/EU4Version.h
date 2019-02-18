/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#ifndef EU4_VERSION_H_
#define EU4_VERSION_H_



#include "newParser.h"
#include <string>
#include <memory>



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

		Version(std::string version);
		Version(std::istream& theStream);

		bool operator >= (const Version& rhs) const;
		bool operator > (const Version& rhs) const;
		bool operator < (const Version& rhs) const;
		bool operator <= (const Version& rhs) const;
		bool operator == (const Version& rhs) const;

	private:
		int firstPart = 0;
		int secondPart = 0;
		int thirdPart = 0;
		int fourthPart = 0;
};

}



#endif // EU4_VERSION_H_