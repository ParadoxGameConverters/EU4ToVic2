#include "V2Pop.h"
#include "Log.h"

V2Pop::V2Pop(string _type, int _size, string _culture, string _religion)
{
	type = _type;
	size = _size;
	culture = _culture;
	religion = _religion;
	supportedRegiments = 0;
}

void V2Pop::output(FILE* output) const
{
	if (size > 0)
	{
		fprintf(output, "\t%s=\n", type.c_str());
		fprintf(output, "\t{\n");
		fprintf(output, "\t\tculture = %s\n", culture.c_str());
		fprintf(output, "\t\treligion = %s\n", religion.c_str());
		fprintf(output, "\t\tsize=%d\n", size);
		fprintf(output, "\t}\n");
	}
}


bool V2Pop::combine(const V2Pop& rhs)
{
	if ((culture == rhs.culture) && (religion == rhs.religion) && (type == rhs.type))
	{
		size += rhs.size;
		return true;
	}
	else
	{
		return false;
	}
}
