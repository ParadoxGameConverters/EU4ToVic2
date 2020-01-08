#include "Religion.h"

 // e.g. catholic <-> catholic
bool EU4::Religion::isSameReligion(const EU4::Religion& other) const
{
	return (name == other.name);
}

// e.g. orthodox <-> catholic
bool EU4::Religion::isRelatedTo(const EU4::Religion& other) const
{
	// not the same religion
	if (!isSameReligion(other) && (group == other.group))
	{
		return true;
	}

	return false;
}

// e.g. sunni <-> catholic
bool EU4::Religion::isInfidelTo(const EU4::Religion& other) const
{
	// different religious groups == infidel
	return (group != other.group);
}