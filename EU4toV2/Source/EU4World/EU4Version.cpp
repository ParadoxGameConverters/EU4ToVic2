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
	registerKeyword(std::regex("first"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		firstPart = firstString.getInt();
	});
	registerKeyword(std::regex("second"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		secondPart = firstString.getInt();
	});
	registerKeyword(std::regex("third"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		thirdPart = firstString.getInt();
	});
	registerKeyword(std::regex("forth"), [this](const std::string& unused, std::istream& theStream){
		commonItems::singleInt firstString(theStream);
		fourthPart = firstString.getInt();
	});
	registerKeyword(std::regex("name"), commonItems::ignoreItem);

	parseStream(input);
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