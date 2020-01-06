#ifndef V2POP_H_
#define V2POP_H_

#include <string>
#include <vector>
using namespace std;

class V2Pop
{
	public:
		V2Pop(string type, int size, string culture, string religion);

		void output(FILE*) const;
		bool combine(const V2Pop& rhs);

		void	changeSize(int delta)					{ size += delta; }
		void	incrementSupportedRegimentCount()	{ supportedRegiments++; }
		void	setCulture(string _culture)			{ culture = _culture; }
		void	setReligion(string _religion)			{ religion = _religion; }

		int		getSize()							const	{ return size; }
		string	getType()							const	{ return type; }
		string	getCulture()						const	{ return culture; }
		string	getReligion()						const	{ return religion; }
		int		getSupportedRegimentCount()	const	{ return supportedRegiments; }
		bool isSlavePop() const { return ((type == "slaves") || (culture.substr(0, 4) == "afro")); }

	private:
		string	type;
		int		size;
		string	culture;
		string	religion;
		int		supportedRegiments;
};


int getNextPopId();



#endif