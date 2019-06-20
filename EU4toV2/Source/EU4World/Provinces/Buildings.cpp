/*Copyright(c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */



#include "Buildings.h"
#include "ParserHelpers.h"



EU4::Buildings::Buildings(std::istream& theStream)
{
	registerKeyword(std::regex("[a-zA-Z0-9_]+"), [this](const std::string& building, std::istream& theStream) {
		buildings.insert(building);
		commonItems::ignoreItem(building, theStream);
	});

	parseStream(theStream);
}
EU4::GreatProjects::GreatProjects(std::istream& theStream)
{
	registerKeyword(std::regex("\"{1}[a-zA-Z0-9_]+\"{1}"), [this](const std::string& greatProject, std::istream& theStream) {
		///string greatProjectWithoutQuote = greatProject.substr(1, greatProject.size() - 1);
		greatProjects.insert(greatProject);
		commonItems::ignoreItem(greatProject, theStream);
	});

	parseStream(theStream);
}