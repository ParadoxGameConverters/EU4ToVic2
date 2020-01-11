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



#include "EU4Version.h"
#include "Log.h"
#include "ParserHelpers.h"



EU4::Version::Version(std::string version)
{
	int dot = version.find_first_of('.');	// the dots separating the version parts
	firstPart = std::stoi(version.substr(0, dot));

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	secondPart = std::stoi(version.substr(0, dot));

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	thirdPart = std::stoi(version.substr(0, dot));

	version = version.substr(dot + 1, version.size());
	dot = version.find_first_of('.');
	fourthPart = std::stoi(version.substr(0, dot));
}


EU4::Version::Version(std::istream& input)
{
	registerKeyword("first", [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		firstPart = firstString.getInt();
	});
	registerKeyword("second", [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		secondPart = firstString.getInt();
	});
	registerKeyword("third", [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		thirdPart = firstString.getInt();
	});
	registerKeyword("forth", [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		fourthPart = firstString.getInt();
	});
	registerKeyword(std::regex("name"), commonItems::ignoreItem);

	parseStream(input);
	clearRegisteredKeywords();
}


bool EU4::Version::operator >= (const EU4::Version& rhs) const
{
	if (firstPart > rhs.firstPart)
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart > rhs.secondPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart > rhs.thirdPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart == rhs.thirdPart) && (fourthPart >= rhs.fourthPart))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool EU4::Version::operator > (const EU4::Version& rhs) const
{
	if (firstPart > rhs.firstPart)
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart > rhs.secondPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart > rhs.thirdPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart == rhs.thirdPart) && (fourthPart > rhs.fourthPart))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool EU4::Version::operator < (const EU4::Version& rhs) const
{
	if (firstPart < rhs.firstPart)
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart < rhs.secondPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart < rhs.thirdPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart == rhs.thirdPart) && (fourthPart < rhs.fourthPart))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool EU4::Version::operator <= (const EU4::Version& rhs) const
{
	if (firstPart < rhs.firstPart)
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart < rhs.secondPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart < rhs.thirdPart))
	{
		return true;
	}
	else if ((firstPart == rhs.firstPart) && (secondPart == rhs.secondPart) && (thirdPart == rhs.thirdPart) && (fourthPart <= rhs.fourthPart))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool EU4::Version::operator == (const EU4::Version& rhs) const
{
	return ((firstPart == rhs.firstPart) &&
			(secondPart == rhs.secondPart) &&
			(thirdPart == rhs.thirdPart) &&
			(fourthPart == rhs.fourthPart));
}


bool EU4::Version::operator != (const EU4::Version& rhs) const
{
	return ((firstPart != rhs.firstPart) ||
			(secondPart != rhs.secondPart) ||
			(thirdPart != rhs.thirdPart) ||
			(fourthPart != rhs.fourthPart));
}


std::ostream& EU4::operator<<(std::ostream& out, const EU4::Version& version)
{
	out << version.firstPart << '.';
	out << version.secondPart << '.';
	out << version.thirdPart << '.';
	out << version.fourthPart;
	return out;
}