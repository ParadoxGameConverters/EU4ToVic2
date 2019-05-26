/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */


#ifndef DATE_ITEM_H_
#define DATE_ITEM_H_



#include "Date.h"
#include "newParser.h"
#include <string>



namespace EU4
{

enum class DateItemType
{
	OWNER_CHANGE,
	CULTURE_CHANGE,
	RELIGION_CHANGE,
	OTHER_CHANGE
};


class DateItem: commonItems::parser
{
	public:
		DateItem(const std::string& dateString, const std::string& typeString, std::istream& theStream);

		DateItemType getType() const { return type; }
		date getDate() const { return theDate; }
		std::string getData() const { return data; }

	private:
		DateItemType type = DateItemType::OTHER_CHANGE;
		date theDate;
		std::string data;
};

}



#endif // DATE_ITEM_H_