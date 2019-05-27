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



#ifndef RELIGION_H_
#define RELIGION_H_



#include <string>



namespace EU4
{


class Religion
{
	public:
		Religion(std::string _name, std::string _group): name(_name), group(_group) {}

		// exactly one of these four functions should return true for any given pairing
		bool isSameReligion(const Religion& other) const;	// e.g. catholic <-> catholic
		bool isRelatedTo(const Religion& other) const;		// e.g. orthodox <-> catholic
		bool isInfidelTo(const Religion& other) const;		// e.g. sunni <-> catholic

	private:
		std::string name;
		std::string group;
};

}



#endif // RELIGION_H_
