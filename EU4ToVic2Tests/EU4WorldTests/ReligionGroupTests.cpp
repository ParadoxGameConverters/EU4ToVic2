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



#include "gtest/gtest.h"
#include "../EU4toV2/Source/EU4World/ReligionGroup.h"
#include <sstream>



TEST(EU4World_EU4ReligionGroupTests, sameReligionReturnsSameReligion)
{
	std::stringstream input;
	input << "= {\n";
	input << "	defender_of_faith = yes\n";
	input << "	can_form_personal_unions = yes\n";
	input << "	center_of_religion = 385 # Mecca\n";
	input << "	flags_with_emblem_percentage = 33\n";
	input << "	flag_emblem_index_range = { 110 110 }\n";
	input << "	ai_will_propagate_through_trade = yes\n";
	input << "	religious_schools = {\n";
	input << "		hanafi_school = {\n";
	input << "			potential_invite_scholar = { \n";
	input << "				knows_of_scholar_country_capital_trigger = yes\n";
	input << "			}\n";
	input << "			# THIS = Nation wanting to invite the scholar, FROM = Nation that follows that school\n";
	input << "			can_invite_scholar = {\n";
	input << "				adm_power = 50\n";
	input << "				if = {\n";
	input << "					limit = {\n";
	input << "						NOT = { religion = sunni }\n";
	input << "						NOT = { religion = ibadi }\n";
	input << "					}\n";
	input << "					NOT = { piety = -0.5 }\n";
	input << "				}\n";
	input << "				reverse_has_opinion = { who = FROM value = 150 }\n";
	input << "				NOT = { has_country_modifier = hanafi_scholar_modifier }\n";
	input << "				OR = {\n";
	input << "					alliance_with = FROM\n";
	input << "					is_subject_of = FROM\n";
	input << "					overlord_of = FROM\n";
	input << "				}\n";
	input << "				hidden_progressive_opinion_for_scholar_trigger = yes\n";
	input << "			}\n";
	input << "			# THIS = Nation wanting to invite the scholar, FORM = Nation that sends scholar\n";
	input << "			on_invite_scholar = {\n";
	input << "				add_adm_power = -50\n";
	input << "				clear_religious_scholar_modifiers_effect = yes\n";
	input << "				custom_tooltip = INVITE_SCHOLAR_COUNTRY_TEXT\n";
	input << "				add_country_modifier = { name = hanafi_scholar_modifier duration = 7300 }\n";
	input << "			}\n";
	input << "			# If you don't set this it will expand on_invite_scholar effect which might be super big\n";
	input << "			invite_scholar_modifier_display = hanafi_scholar_modifier\n";
	input << "			picture = \"GFX_icon_muslim_school_hanafi\"\n";
	input << "			# religion_sub_modifier = no is possible, will hide icon in religious screen\n";
	input << "			adm_tech_cost_modifier = -0.05\n";
	input << "		}\n";
	input << "	}\n";
	input << "	zoroastrian = {\n";
	input << "		icon = 26\n";
	input << "		color = { 127 178 51 }\n";
	input << "		province = { \n";
	input << "			local_missionary_strength = -0.02\n";
	input << "		}\n";
	input << "		country = {\n";
	input << "			tolerance_own = 2\n";
	input << "			trade_efficiency = 0.1\n";
	input << "		}\n";
	input << "		country_as_secondary = { \n";
	input << "			merchants = 1\n";
	input << "			tolerance_own = 1\n";
	input << "		}\n";
	input << "		heretic = { MAZDAKI MANICHEAN }\n";
	input << "	}\n";
	input << "	harmonized_modifier = harmonized_zoroastrian_group\n";
	input << "	crusade_name = HOLY_WAR\n";
	input << "}";

	EU4::ReligionGroup religionGroup("zoroastrian_group", input);
	std::map<std::string, EU4::Religion> theReligions = religionGroup.takeReligions();

	ASSERT_TRUE(theReligions.find("zoroastrian") != theReligions.end());
}


TEST(EU4World_EU4ReligionGroupTests, onlyReligionsAreImported)
{
	std::stringstream input;
	input << "= {\n";
	input << "	defender_of_faith = yes\n";
	input << "	can_form_personal_unions = yes\n";
	input << "	center_of_religion = 385 # Mecca\n";
	input << "	flags_with_emblem_percentage = 33\n";
	input << "	flag_emblem_index_range = { 110 110 }\n";
	input << "	ai_will_propagate_through_trade = yes\n";
	input << "	religious_schools = {\n";
	input << "		hanafi_school = {\n";
	input << "			potential_invite_scholar = { \n";
	input << "				knows_of_scholar_country_capital_trigger = yes\n";
	input << "			}\n";
	input << "			# THIS = Nation wanting to invite the scholar, FROM = Nation that follows that school\n";
	input << "			can_invite_scholar = {\n";
	input << "				adm_power = 50\n";
	input << "				if = {\n";
	input << "					limit = {\n";
	input << "						NOT = { religion = sunni }\n";
	input << "						NOT = { religion = ibadi }\n";
	input << "					}\n";
	input << "					NOT = { piety = -0.5 }\n";
	input << "				}\n";
	input << "				reverse_has_opinion = { who = FROM value = 150 }\n";
	input << "				NOT = { has_country_modifier = hanafi_scholar_modifier }\n";
	input << "				OR = {\n";
	input << "					alliance_with = FROM\n";
	input << "					is_subject_of = FROM\n";
	input << "					overlord_of = FROM\n";
	input << "				}\n";
	input << "				hidden_progressive_opinion_for_scholar_trigger = yes\n";
	input << "			}\n";
	input << "			# THIS = Nation wanting to invite the scholar, FORM = Nation that sends scholar\n";
	input << "			on_invite_scholar = {\n";
	input << "				add_adm_power = -50\n";
	input << "				clear_religious_scholar_modifiers_effect = yes\n";
	input << "				custom_tooltip = INVITE_SCHOLAR_COUNTRY_TEXT\n";
	input << "				add_country_modifier = { name = hanafi_scholar_modifier duration = 7300 }\n";
	input << "			}\n";
	input << "			# If you don't set this it will expand on_invite_scholar effect which might be super big\n";
	input << "			invite_scholar_modifier_display = hanafi_scholar_modifier\n";
	input << "			picture = \"GFX_icon_muslim_school_hanafi\"\n";
	input << "			# religion_sub_modifier = no is possible, will hide icon in religious screen\n";
	input << "			adm_tech_cost_modifier = -0.05\n";
	input << "		}\n";
	input << "	}\n";
	input << "	zoroastrian = {\n";
	input << "		icon = 26\n";
	input << "		color = { 127 178 51 }\n";
	input << "		province = { \n";
	input << "			local_missionary_strength = -0.02\n";
	input << "		}\n";
	input << "		country = {\n";
	input << "			tolerance_own = 2\n";
	input << "			trade_efficiency = 0.1\n";
	input << "		}\n";
	input << "		country_as_secondary = { \n";
	input << "			merchants = 1\n";
	input << "			tolerance_own = 1\n";
	input << "		}\n";
	input << "		heretic = { MAZDAKI MANICHEAN }\n";
	input << "	}\n";
	input << "	harmonized_modifier = harmonized_zoroastrian_group\n";
	input << "	crusade_name = HOLY_WAR\n";
	input << "}";

	EU4::ReligionGroup religionGroup("zoroastrian_group", input);
	std::map<std::string, EU4::Religion> theReligions = religionGroup.takeReligions();

	ASSERT_EQ(theReligions.size(), 1);
}