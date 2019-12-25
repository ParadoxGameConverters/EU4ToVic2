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



#include "V2UncivReforms.h"
#include "Log.h"
#include "../Configuration.h"
#include "../EU4World/EU4Country.h"
#include "V2Country.h"
#include "V2State.h"
#include "V2Province.h"


V2UncivReforms::V2UncivReforms(int westernizationProgress, double milFocus, double socioEcoFocus, V2Country* country)
{
	int westernizationCost[16];
	westernizationCost[0]	= 15;
	westernizationCost[1]	= 15;
	westernizationCost[2]	= 15;
	westernizationCost[3]	= 20;
	westernizationCost[4]	= 15;
	westernizationCost[5]	= 15;
	westernizationCost[6]	= 20;
	westernizationCost[7]	= 25;

	westernizationCost[8]	= 10;
	westernizationCost[9]	= 10;
	westernizationCost[10]	= 10;
	westernizationCost[11]	= 10;
	westernizationCost[12]	= 10;
	westernizationCost[13]	= 10;
	westernizationCost[14]	= 10;
	westernizationCost[15]	= 10;

	// Get all valid socio-economic reforms
	double socioEconProgress = westernizationProgress * socioEcoFocus;
	for (unsigned int i = 0; i < 8; i++)
	{
		if (socioEconProgress >= westernizationCost[i] - 0.001)
		{
			reforms[i]			= true;
			socioEconProgress	-= westernizationCost[i];
		}
		else
		{
			reforms[i] = false;
		}
	}

	// Get all valid military reforms
	double milProgress = westernizationProgress * milFocus;
	for (unsigned int i = 8; i < 16; i++)
	{
		if (milProgress >= westernizationCost[i] - 0.001)
		{
			reforms[i]	= true;
			milProgress	-= westernizationCost[i];
		}
		else
		{
			reforms[i] = false;
		}
	}

	// Use remaining progress to get any reforms in preferred category
	double remainingProgress = socioEconProgress + milProgress;
	if (socioEconProgress >= milProgress)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			if (reforms[i]	== true)
			{
				continue;
			}
			else if (remainingProgress >= westernizationCost[i] - 0.001)
			{
				reforms[i]			= true;
				remainingProgress	-=westernizationCost[i];
			}
		}
	}
	else
	{
		for (unsigned int i = 8; i < 16; i++)
		{
			if (reforms[i]	== true)
			{
				continue;
			}
			else if (remainingProgress >= westernizationCost[i] - 0.001)
			{
				reforms[i]			= true;
				remainingProgress	-= westernizationCost[i];
			}
		}
	}

	// Use any remaining progress to get any possible reforms
	for (unsigned int i = 0; i < 16; i++)
	{
		if (reforms[i] == true)
		{
			continue;
		}
		else if (remainingProgress >= westernizationCost[i] - 0.001)
		{
			reforms[i] = true;
			remainingProgress -= westernizationCost[i];
		}
	}

	// Convert remaining progress to RP
	country->addResearchPoints(remainingProgress * 800);

	// Implement special effects from reforms
	if (reforms[5] == true)
	{
		country->addRailroadtoCapitalState();
	}

	if (reforms[9] == true)
	{
		country->addTech("flintlock_rifles");
	}

	if (reforms[10] == true)
	{
		country->addTech("post_napoleonic_thought");
		auto provinces = country->getProvinces();
		auto provItr = provinces.find(country->getCapital());
		if (provItr != provinces.end())
		{
			provItr->second->setFortLevel(2);
		}
	}

	if (reforms[11] == true)
	{
		country->addTech("military_staff_system");
	}

	if (reforms[15] == true)
	{
		country->addTech("post_nelsonian_thought");
	}
}


void V2UncivReforms::output(FILE* output) const
{
	if (reforms[0]) {
		fprintf(output, "land_reform=yes_land_reform\n");
	}
	else
	{
		fprintf(output, "land_reform=no_land_reform\n");
	}

	if (reforms[1]) {
		fprintf(output, "admin_reform=yes_admin_reform\n");
	}
	else
	{
		fprintf(output, "admin_reform=no_admin_reform\n");
	}

	if (reforms[3] && reforms[2]) {
		fprintf(output, "finance_reform=finance_reform_two\n");
	}
	else if (reforms[2]) {
		fprintf(output, "finance_reform=yes_finance_reform\n");
	}
	else
	{
		fprintf(output, "finance_reform=no_finance_reform\n");
	}

	if (reforms[4]) {
		fprintf(output, "education_reform=yes_education_reform\n");
	}
	else
	{
		fprintf(output, "education_reform=no_education_reform\n");
	}

	if (reforms[5]) {
		fprintf(output, "transport_improv=yes_transport_improv\n");
	}
	else
	{
		fprintf(output, "transport_improv=no_transport_improv\n");
	}

	if (reforms[6]) {
		fprintf(output, "pre_indust=yes_pre_indust\n");
	}
	else
	{
		fprintf(output, "pre_indust=no_pre_indust\n");
	}

	if (reforms[7]) {
		fprintf(output, "industrial_construction=yes_industrial_construction\n");
	}
	else
	{
		fprintf(output, "industrial_construction=no_industrial_construction\n");
	}

	if (reforms[8]) {
		fprintf(output, "foreign_training=yes_foreign_training\n");
	}
	else
	{
		fprintf(output, "foreign_training=no_foreign_training\n");
	}

	if (reforms[9]) {
		fprintf(output, "foreign_weapons=yes_foreign_weapons\n");
	}
	else
	{
		fprintf(output, "foreign_weapons=no_foreign_weapons\n");
	}

	if (reforms[10]) {
		fprintf(output, "military_constructions=yes_military_constructions\n");
	}
	else
	{
		fprintf(output, "military_constructions=no_military_constructions\n");
	}

	if (reforms[11]) {
		fprintf(output, "foreign_officers=yes_foreign_officers\n");
	}
	else
	{
		fprintf(output, "foreign_officers=no_foreign_officers\n");
	}

	if (reforms[12]) {
		fprintf(output, "army_schools=yes_army_schools\n");
	}
	else
	{
		fprintf(output, "army_schools=no_army_schools\n");
	}

	if (reforms[13]) {
		fprintf(output, "foreign_naval_officers=yes_foreign_naval_officers\n");
	}
	else
	{
		fprintf(output, "foreign_naval_officers=no_foreign_naval_officers\n");
	}

	if (reforms[14]) {
		fprintf(output, "naval_schools=yes_naval_schools\n");
	}
	else
	{
		fprintf(output, "naval_schools=no_naval_schools\n");
	}

	if (reforms[14]) {
		fprintf(output, "foreign_navies=yes_foreign_navies\n");
	}
	else
	{
		fprintf(output, "foreign_navies=no_foreign_navies\n");
	}
}
