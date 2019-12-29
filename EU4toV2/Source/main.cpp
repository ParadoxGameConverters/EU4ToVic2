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



#include "Log.h"
#include "EU4ToVic2Converter.h"
#include "OSCompatibilityLayer.h"
#include <stdexcept>
#include <string>



std::string getSaveFileName(const int argc, const char * argv[]);
int main(const int argc, const char * argv[])
{
	try
	{
		LOG(LogLevel::Info) << "Converter version 1.0J \"Jan Mayen\" - compatible with 1.29 and previous";
		LOG(LogLevel::Info) << "Built " << __TIMESTAMP__;
		LOG(LogLevel::Debug) << "Current directory is " << Utils::getCurrentDirectory();

		std::string EU4SaveFileName;
		if (argc >= 2)
		{
			LOG(LogLevel::Info) << "Using input file " << argv[1];
			EU4SaveFileName = argv[1];
		}
		else
		{
			LOG(LogLevel::Info) << "No input file given, defaulting to input.eu4";
			EU4SaveFileName = "input.eu4";
		}
		ConvertEU4ToVic2(EU4SaveFileName);

		return 0;
	}

	catch (const std::exception& e)
	{
		LOG(LogLevel::Error) << e.what();
		return -1;
	}
}