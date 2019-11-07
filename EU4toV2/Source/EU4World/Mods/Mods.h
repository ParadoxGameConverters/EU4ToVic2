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



#ifndef EU4_MODS_H_
#define EU4_MODS_H_



#include "newParser.h"
#include "../../Configuration.h"
#include <map>
#include <optional>
#include <string>



namespace EU4
{

class Mods: commonItems::parser
{
	public:
		Mods(std::istream& theStream, Configuration& theConfiguration);

		std::optional<std::string> getModPath(const std::string& modName) const;

	private:
		void loadEU4ModDirectory(const Configuration& theConfiguration);
		void loadSteamWorkshopDirectory(const Configuration& theConfiguration);
		void loadCK2ExportDirectory(const Configuration& theConfiguration);
		void loadModDirectory(const std::string& searchDirectory);

		std::map<std::string, std::string> possibleMods;
		std::map<std::string, std::string> possibleCompressedMods;
};

}


#endif // EU4_MODS_H_