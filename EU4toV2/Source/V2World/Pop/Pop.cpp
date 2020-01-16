#include "Pop.h"

V2::Pop::Pop(std::string _type, int _size, std::string _culture, std::string _religion) :
	type(_type), size(_size), culture(_culture), religion(_religion) {}

V2::Pop::Pop(std::string _type, mappers::MapperPop mapperpop):
	type(_type), size(mapperpop.getSize()), culture(mapperpop.getCulture()), religion(mapperpop.getReligion()) {}

bool V2::Pop::combine(const V2::Pop& rhs)
{
	if (culture == rhs.culture && religion == rhs.religion && type == rhs.type)
	{
		size += rhs.size;
		return true;
	}
	return false;
}
