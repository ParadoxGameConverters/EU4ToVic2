#include "Localisation.h"
#include "OSCompatibilityLayer.h"

const std::array<std::string, V2::Localisation::numLanguages> V2::Localisation::languages =
	{ "english", "french", "german", "polish", "spanish", "italian", "swedish", "czech", "hungarian", "dutch", "portugese", "russian", "finnish" };

void V2::Localisation::setTag(const std::string& newTag)
{
	tag = newTag;
}

void V2::Localisation::readFromCountry(const EU4::Country& source)
{
	for (size_t i = 0; i < numLanguages; ++i)
	{
		if (!languages[i].empty())
		{
			name[i] = source.getName(languages[i]);
			adjective[i] = source.getAdjective(languages[i]);
		}
	}
}

void V2::Localisation::setPartyKey(size_t partyIndex, const std::string& partyKey)
{
	if (parties.size() <= partyIndex)
	{
		parties.resize(partyIndex + 1);
	}
	parties[partyIndex].key = partyKey;
}

void V2::Localisation::setPartyName(size_t partyIndex, const std::string& language, const std::string& name)
{
	if (parties.size() <= partyIndex)
	{
		parties.resize(partyIndex + 1);
	}
	const auto& languageItr = std::find(languages.begin(), languages.end(), language);
	if (languageItr != languages.end())
	{
		const size_t languageIndex = std::distance(languages.begin(), languageItr);
		parties[partyIndex].name[languageIndex] = name;
	}
}

std::string V2::Localisation::convert(const std::string& text)
{
	if (text.empty()) return std::string();
	return Utils::convertUTF8To8859_15(text);
}

std::string V2::Localisation::getLocalName() const
{
	for (std::string thisname : name)
	{
		if (!thisname.empty()) return thisname;
	}
	return std::string();
}


std::string V2::Localisation::getLocalAdjective() const
{
	for (std::string thisname : adjective)
	{
		if (!thisname.empty()) return thisname;
	}
	return std::string();
}

std::string V2::Localisation::stripAccents(const std::string& text)
{
	std::string accents = "àáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞ";
	std::string without = "aaaaaaaceeeeiiiidnooooooouuuuytyAAAAAAACEEEEIIIIDNOOOOOOOUUUUYT";
	std::string out(text);

	for (unsigned int i = 0; i < accents.size(); i++)
	{
		std::replace(out.begin(), out.end(), accents[i], without[i]);
	}
	return out;
}
