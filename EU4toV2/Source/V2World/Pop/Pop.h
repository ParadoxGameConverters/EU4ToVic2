#ifndef POP_H
#define POP_H

#include <string>
#include "../../Mappers/Pops/MapperPop.h"

namespace V2
{
	class Pop
	{
	public:
		Pop(std::string _type, int _size, std::string _culture, std::string _religion);
		Pop(std::string _type, const mappers::MapperPop& mapperPop);

		bool combine(const Pop& rhs);
		void changeSize(int delta) { size += delta; }
		void incrementSupportedRegimentCount() { supportedRegiments++; }
		void setCulture(std::string _culture) { culture = _culture; }
		void setReligion(std::string _religion) { religion = _religion; }

		int getSize() const { return size; }
		std::string getType() const { return type; }
		std::string getCulture() const { return culture; }
		std::string getReligion() const { return religion; }
		int getSupportedRegimentCount() const { return supportedRegiments; }
		bool isSlavePop() const { return type == "slaves" || culture.substr(0, 4) == "afro"; }

		friend std::ostream& operator<<(std::ostream& output, const Pop& pop);

	private:
		std::string type;
		int size = 0;
		std::string culture;
		std::string religion;
		int supportedRegiments = 0;
	};
}

#endif // POP_H