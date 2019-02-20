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



#ifndef STATE_MAPPER_H
#define STATE_MAPPER_H



#include "newParser.h"
#include <map>
#include <memory>
#include <vector>





namespace Vic2
{

class stateMapper: commonItems::parser
{
	public:
		stateMapper(std::istream& theStream);

		std::vector<int> getAllProvincesInState(int province);
		int getStateIndex(int province);

	private:
		std::map<int, std::vector<int>> stateProvincesMap;
		std::map<int, int> stateIndexMap;
};


class stateMapperFile
{
	public:
		stateMapperFile();
		~stateMapperFile() = default;

		std::unique_ptr<stateMapper> takeStateMapper() { return std::move(theStateMapper); }

	private:
		stateMapperFile(const stateMapperFile&) = delete;
		stateMapperFile(stateMapperFile&&) = delete;
		stateMapperFile& operator=(const stateMapperFile&) = delete;
		stateMapperFile& operator=(stateMapperFile&&) = delete;

		std::unique_ptr<stateMapper> theStateMapper;
};

}



#endif // STATE_MAPPER_H