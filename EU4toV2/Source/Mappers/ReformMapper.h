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



#ifndef REFORM_MAPPER_H
#define REFORM_MAPPER_H



#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;



class Object;

class ReformProperties
{
public:
	string getForceGov() const { return forceGov; }
	double getLiberty() const { return liberty; }
	double getEquality() const { return equality; }
	double getOrder() const { return order; }
	void setForceGov(string force) { forceGov = force; }
	void setLiberty(double lib) { liberty = lib; }
	void setEquality(double equ) { equality = equ; }
	void setOrder(double ord) { order = ord; }

private:
	string forceGov = "";
	double liberty = 0.0;
	double equality = 0.0;
	double order = 0.0;

};


class ReformMapper
{
	public:
		static ReformProperties matchReform(const string& sourceReform)
		{
			return getInstance()->MatchReform(sourceReform);
		}

	private:
		static ReformMapper* instance;
		static ReformMapper* getInstance()
		{
			if (instance == nullptr)
			{
				instance = new ReformMapper;
			}
			return instance;
		}

		ReformMapper();
		void initReformMap(std::vector<std::shared_ptr<Object>> objs);
		void processRule(shared_ptr<Object> obj);

		ReformProperties MatchReform(const string& sourceReform);

		map<string, ReformProperties> ReformMap;
};



#endif // Reform_MAPPER_H