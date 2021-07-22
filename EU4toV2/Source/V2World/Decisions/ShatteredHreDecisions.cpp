#include "V2World.h"

void V2::World::createShatteredHreDecisions(const std::string& v2HreTag)
{
	const auto& adjective = getCountry(v2HreTag)->getLocalisation().getLocalAdjective();

	std::string outLoc;
	outLoc = ";;;;;;;;;;;;x,,,,,,,,,,,,,\n";
	outLoc += "integrate_empire_title_title;Centralize the " + adjective +
				 " Empire: Integrate the Empire Title;Centraliser le Saint Empire Romain: Intégrer le titre impérial;Zentralisierung des Heiligen Römischen "
				 "Reiches: Integrieren Sie den kaiserlichen Titel;Centralizacja Œwiête Cesarstwo Rzymskie: Zintegruj tytuł imperialny;Centralizar el Sacro Imperio "
				 "Romano: Integra el título imperial;Centralizzare il Sacro Romano Impero: Integra il titolo imperiale;Centralizálja a Szent Római Birodalom: "
				 "Integrálja a császári címet;Centralizujte Svatou øímskou øíši: Integrujte císařský titul;Centralizálja a Szent Római Birodalom: Integrálja a "
				 "császári címet;Centraliseren van het Heilige Roomse Rijk: Integreer de keizerlijke titel;Centralize o Sacro Império Romano: Integre o título "
				 "imperial;Öåíòðàëèçîâàòü Ñâÿòóþ Ðèìñêóþ èìïåðèþ: Èíòåãðèðîâàòü èìïåðñêèé òèòóë;Keskitä Pyhän roomalaisen keisarikunnan: Integroi keisarillinen "
				 "otsikko;x\n";
	outLoc +=
		 "integrate_empire_title_desc;In order to centralize the " + adjective +
		 " Empire, first we need to establish our authority over the Imperial crown;Afin de centraliser l'Empire, nous devons d'abord établir notre autorité sur "
		 "la couronne impériale;Um das Imperium zu zentralisieren, müssen wir zuerst unsere Autorität über die Kaiserkrone etablieren;Aby scentralizować "
		 "Imperium, najpierw musimy ustanowić naszą władzę nad cesarską koroną;Para centralizar el Imperio, primero debemos establecer nuestra autoridad sobre "
		 "la corona imperial;Per centralizzare l'Impero, prima dobbiamo stabilire la nostra autorità sulla corona imperiale;A Birodalom központosítása érdekében "
		 "először meg kell állapítanunk a birodalmi korona felett fennálló tekintélyünket;Abychom mohli říši centralizovat, musíme nejprve ustanovit naši "
		 "autoritu nad imperiální korunou;A Birodalom központosítása érdekében először meg kell állapítanunk a birodalmi korona felett fennálló "
		 "tekintélyünket;Om het rijk te centraliseren, moeten we eerst onze autoriteit over de keizerlijke kroon vestigen;Para centralizar o Império, primeiro "
		 "precisamos estabelecer nossa autoridade sobre a coroa imperial;×òîáû öåíòðàëèçîâàòü Èìïåðèþ, ñíà÷àëà íàì íóæíî óñòàíîâèòü ñâîþ âëàñòü íàä "
		 "Èìïåðàòîðñêîé êîðîíîé;Imperiumin keskittämiseksi meidän on ensin luotava valtaamme keisarilliseen kruunuun;x\n";
	outLoc += "centralize_hre_title;Centralize the " + adjective +
				 " Empire;Centraliser le Saint Empire Romain;Zentralisierung des Heiligen Römischen Reiches;Centralizacja Œwiête Cesarstwo Rzymskie;Centralizar el "
				 "Sacro Imperio Romano;Centralizzare il Sacro Romano Impero;Centralizálja a Szent Római Birodalom;Centralizujte Svatou øímskou øíši;Centralizálja "
				 "a Szent Római Birodalom;Centraliseren van het Heilige Roomse Rijk;Centralize o Sacro Império Romano;Öåíòðàëèçîâàòü Ñâÿòóþ Ðèìñêóþ "
				 "èìïåðèþ;Keskitä Pyhän roomalaisen keisarikunnan;x\n";
	outLoc += "centralize_hre_desc;As the " + adjective +
				 " Emperor, we are prestigious enough to attempt to centralize the Empire under ourselves.;En tant que Saint Empereur Romain, nous sommes assez "
				 "prestigieux pour essayer de centraliser l'Empire sous nos mains.;Als der Heilige Römische Kaiser sind wir prestigevoll genug, um zu versuchen, "
				 "das Reich unter uns zu zentralisieren.;Jako cesarz rzymski, jesteœmy na tyle presti¿owa próba scentralizowania Imperium pod siebie.;Como el "
				 "emperador romano santo, somos bastante prestigiosos para intentar centralizar el imperio debajo de nosotros mismos.;Come Sacro Romano "
				 "Imperatore, siamo abbastanza prestigiosi per tentare di centralizzare l'Impero sotto il nostro dominio.;Mivel a római császár, mi tekintélyes "
				 "elég próbálja központosítani a Birodalom magunkat.;Jako svatý øímský císaø jsme dostatecnì prestižní, abychom se pokusili centralizovat Øíši pod "
				 "sebou.;Mivel a római császár, mi tekintélyes elég próbálja központosítani a Birodalom magunkat.;Zoals het Heilige Roomse keizer, we zijn "
				 "prestigieus genoeg om te proberen om het rijk te centraliseren onder onszelf.;Como o Santo Imperador Romano, somos suficientemente prestigiados "
				 "para tentar centralizar o Império sob nós mesmos.;Êàê èìïåðàòîð Ñâÿùåííîé Ðèìñêîé èìïåðèè, ìû äîñòàòî÷íî ïðåñòèæíû, ÷òîáû ïîïûòàòüñÿ "
				 "öåíòðàëèçîâàòü Èìïåðèþ ïîä ñîáîé.;Koska keisari, olemme arvostettuja tarpeeksi yrittää keskittää Imperiumi alle itse.;x\n";
	localisations["00_shattered_hre.csv"] += outLoc;

	std::string output;
	output = "political_decisions = {\n";
	output += "\tintegrate_empire_title = {\n";
	output += "\t\tpotential = {\n";
	output += "\t\t\tcapital_scope = { is_core = " + v2HreTag + " }\n";
	output += "\t\t\tOR = {\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\tNOT = { has_global_flag = liberal_revolutions_should_now_fire }\n";
	output += "\t\t\t\t\thas_country_flag = emperor_hre\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\thas_country_flag = union_is_hre\n";
	output += "\t\t\t\t\tNOT = {\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = africa }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = north_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = south_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = oceania }\n";
	output += "\t\t\t\t\t\t#tag = FSA\n";
	output += "\t\t\t\t\t\ttag = CSA\n";
	output += "\t\t\t\t\t\ttag = MGL\n";
	output += "\t\t\t\t\t\ttag = SPQ\n";
	output += "\t\t\t\t\t\ttag = SLA\n";
	output += "\t\t\t\t\t\ttag = " + v2HreTag + "\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t}\n";
	output += "\t\t\tis_our_vassal = " + v2HreTag + "\n";
	output += "\t\t}\n";
	output += "\t\tallow = {\n";
	output += "\t\t\tOR = {\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\tis_greater_power = yes\n";
	output += "\t\t\t\t\tprestige = 65\n";
	output += "\t\t\t\t\thas_country_flag = emperor_hre\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\thas_country_flag = union_is_hre\n";
	output += "\t\t\t\t\tprestige = 45\n";
	output += "\t\t\t\t\tnationalism_n_imperialism = 1\n";
	output += "\t\t\t\t\twar = no\n";
	output += "\t\t\t\t\tis_greater_power = yes\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t}\n";
	output += "\t\t}\n";
	output += "\n";
	output += "\t\teffect = {\n";
	output += "\t\t\tinherit = " + v2HreTag + "\n";
	output += "\t\t}\n";
	output += "\t}\n";
	output += "\tcentralize_hre = {\n";
	output += "\t\tnews = yes\n";
	output += "\t\tnews_desc_long = \"hre_NEWS_LONG\"\n";
	output += "\t\tnews_desc_medium = \"hre_NEWS_MEDIUM\"\n";
	output += "\t\tnews_desc_short = \"hre_NEWS_SHORT\"\n";
	output += "\t\tpotential = {\n";
	output += "\t\t\tcapital_scope = { is_core = " + v2HreTag + " }\n";
	output += "\t\t\tOR = {\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\tNOT = { has_global_flag = liberal_revolutions_should_now_fire }\n";
	output += "\t\t\t\t\thas_country_flag = emperor_hre\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\thas_country_flag = union_is_hre\n";
	output += "\t\t\t\t\tNOT = {\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = africa }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = north_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = south_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = oceania }\n";
	output += "\t\t\t\t\t\t#tag = FSA\n";
	output += "\t\t\t\t\t\ttag = CSA\n";
	output += "\t\t\t\t\t\ttag = MGL\n";
	output += "\t\t\t\t\t\ttag = SPQ\n";
	output += "\t\t\t\t\t\ttag = SLA\n";
	output += "\t\t\t\t\t\ttag = " + v2HreTag + "\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t}\n";
	output += "\t\t\tNOT = {\n";
	output += "\t\t\t\texists = " + v2HreTag + "\n";
	output += "\t\t\t}\n";
	output += "\t\t}\n";
	output += "\t\tallow = {\n";
	output += "\t\t\tOR = {\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\tis_greater_power = yes\n";
	output += "\t\t\t\t\tprestige = 65\n";
	output += "\t\t\t\t\thas_country_flag = emperor_hre\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tAND = {\n";
	output += "\t\t\t\t\thas_country_flag = union_is_hre\n";
	output += "\t\t\t\t\tprestige = 45\n";
	output += "\t\t\t\t\tnationalism_n_imperialism = 1\n";
	output += "\t\t\t\t\twar = no\n";
	output += "\t\t\t\t\tis_greater_power = yes\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t}\n";
	output += "\t\t}\n";
	output += "\t\t\n";
	output += "\t\teffect = {\n";
	output += "\t\t\tchange_tag = " + v2HreTag + "\n";
	output += "\t\t\tclr_country_flag = emperor_hre\n";
	output += "\t\t\tany_country = {\n";
	output += "\t\t\t\tlimit = {\n";
	output += "\t\t\t\t\tis_culture_group = THIS\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tset_country_flag = union_is_hre\n";
	output += "\t\t\t}\n";
	output += "\t\t\tany_country = {\n";
	output += "\t\t\t\tlimit = {\n";
	output += "\t\t\t\t\thas_country_flag = member_hre\n";
	output += "\t\t\t\t\tin_sphere = THIS\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tTHIS = {\n";
	output += "\t\t\t\t\tprestige = 10\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tcountry_event = 250001\n";
	output += "\t\t\t\tclr_country_flag = member_hre\n";
	output += "\t\t\t}\n";
	output += "\t\t\tany_country = {\n";
	output += "\t\t\t\tlimit = {\n";
	output += "\t\t\t\t\thas_country_flag = member_hre\n";
	output += "\t\t\t\t\tNOT = {\n";
	output += "\t\t\t\t\t\tin_sphere = THIS\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tcountry_event = 250003\n";
	output += "\t\t\t\tclr_country_flag = member_hre\n";
	output += "\t\t\t}\n";
	output += "\t\t\tany_country = {\n";
	output += "\t\t\t\tlimit = {\n";
	output += "\t\t\t\t\thas_country_flag = member_hre\n";
	output += "\t\t\t\t\tNOT = {\n";
	output += "\t\t\t\t\t\tin_sphere = THIS\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tcountry_event = 250003\n";
	output += "\t\t\t\tclr_country_flag = member_hre\n";
	output += "\t\t\t}\n";
	output += "\t\t\tany_country = {\n";
	output += "\t\t\t\tlimit = {\n";
	output += "\t\t\t\t\tNOT = { is_greater_power = yes }\n";
	output += "\t\t\t\t\thas_country_flag = union_is_hre\n";
	output += "\t\t\t\t\tNOT = { has_country_flag = post_colonial_country }\n";
	output += "\t\t\t\t\tNOT = {\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = africa }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = north_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = south_america }\n";
	output += "\t\t\t\t\t\tcapital_scope = { continent = oceania }\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t\tis_culture_group = THIS\n";
	output += "\t\t\t\t\tin_sphere = THIS\n";
	output += "\t\t\t\t\tOR = {\n";
	output += "\t\t\t\t\t\tis_vassal = no\n";
	output += "\t\t\t\t\t\tvassal_of = THIS\n";
	output += "\t\t\t\t\t}\n";
	output += "\t\t\t\t}\n";
	output += "\t\t\t\tcountry_event = 11101\n";
	output += "\t\t\t}\n";
	output += "\t\t\tcultural_union = { country_event = 250000 }\n";
	output += "\t\t\tadd_accepted_culture = union\n";
	output += "\t\t}\n";
	output += "\t}\n";
	output += "}\n";

	decisions["shattered_hre.txt"] = output;
}