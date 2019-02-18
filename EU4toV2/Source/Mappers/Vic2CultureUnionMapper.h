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



#ifndef VIC2_CULTURE_UNION_MAPPER_H
#define VIC2_CULTURE_UNION_MAPPER_H



#include "newParser.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;



class Object;



class Vic2CultureUnionMapper
{
	public:
		Vic2CultureUnionMapper();
		vector<string> getCoreForCulture(const string& culture);

	private:
		void initUnionMap(shared_ptr<Object> obj);

		map<string, vector<string>> unionMap;
};


class Vic2CultureUnionMapperFile: commonItems::parser
{
	public:
		Vic2CultureUnionMapperFile();
		~Vic2CultureUnionMapperFile() = default;

		std::unique_ptr<Vic2CultureUnionMapper> takeCultureUnionMapper() { return std::move(theCultureUnionMapper); }

	private:
		Vic2CultureUnionMapperFile(const Vic2CultureUnionMapperFile&) = delete;
		Vic2CultureUnionMapperFile(Vic2CultureUnionMapperFile&&) = delete;
		Vic2CultureUnionMapperFile& operator=(const Vic2CultureUnionMapperFile&) = delete;
		Vic2CultureUnionMapperFile& operator=(Vic2CultureUnionMapperFile&&) = delete;

		std::unique_ptr<Vic2CultureUnionMapper> theCultureUnionMapper;
};



#endif //VIC2_CULTURE_UNION_MAPPER_H