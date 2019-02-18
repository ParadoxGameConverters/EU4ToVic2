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


#ifndef BLOCKED_TECH_SCHOOLS_H
#define BLOCKED_TECH_SCHOOLS_H



#include "newParser.h"
#include <memory>
#include <string>
#include <vector>



namespace Vic2
{

class blockedTechSchools: commonItems::parser
{
	public:
		blockedTechSchools(std::istream& theStream);

		blockedTechSchools() = default;
		blockedTechSchools(const blockedTechSchools&) = default;
		blockedTechSchools(blockedTechSchools&&) = default;
		blockedTechSchools& operator=(const blockedTechSchools&) = default;
		blockedTechSchools& operator=(blockedTechSchools&&) = default;
		~blockedTechSchools() = default;

		bool isTechSchoolBlocked(const std::string& techSchool) const;

	private:
		std::vector<std::string> theBlockedTechSchools;
};


class blockedTechSchoolsFile: commonItems::parser
{
	public:
		blockedTechSchoolsFile();
		~blockedTechSchoolsFile() = default;

		std::unique_ptr<blockedTechSchools> takeBlockedTechSchools() { return std::move(theBlockedTechSchools); }

	private:
		blockedTechSchoolsFile(const blockedTechSchoolsFile&) = delete;
		blockedTechSchoolsFile(blockedTechSchoolsFile&&) = delete;
		blockedTechSchoolsFile& operator=(const blockedTechSchoolsFile&) = delete;
		blockedTechSchoolsFile& operator=(blockedTechSchoolsFile&&) = delete;

		std::unique_ptr<blockedTechSchools> theBlockedTechSchools;
};


}


#endif // BLOCKED_TECH_SCHOOLS_H