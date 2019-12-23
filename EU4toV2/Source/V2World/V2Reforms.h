/*Copyright (c) 2018 The Paradox Game Converters Project

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



#ifndef V2REFORMS_H_
#define V2REFORMS_H_



#include <memory>
#include <stdio.h>
#include <string>
using namespace std;



namespace EU4
{
	class Country;
}

class V2Country;



class V2Reforms {
	public:
		V2Reforms(const V2Country*, const std::shared_ptr<EU4::Country>);
		void output(FILE*) const;
	private:

		bool abolishSlavery = false;
		double slavery = 5.0;
		double vote_franchise = 5.0;
		double upper_house_composition = 5.0;
		double voting_system = 5.0;
		double public_meetings = 5.0;
		double press_rights = 5.0;
		double trade_unions = 5.0;
		double political_parties = 5.0;
};


class V2UncivReforms {
	public:
		V2UncivReforms(int westernizationProgress, double milFocus, double socioEcoFocus, V2Country* country);
		void output(FILE*) const;
	private:
		bool reforms[16] = { 0 };
};


#endif // V2REFORMS_H_