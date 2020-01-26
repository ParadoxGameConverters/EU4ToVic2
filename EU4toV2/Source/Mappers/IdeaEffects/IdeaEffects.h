#ifndef IDEA_EFFECTS_H
#define IDEA_EFFECTS_H

#include "newParser.h"

namespace mappers
{
	class IdeaEffects: commonItems::parser
	{
	public:
		explicit IdeaEffects(std::istream& theStream);

		[[nodiscard]] const auto& getEnforce() const { return enforce; }

		[[nodiscard]] auto getArmy() const { return army - 5; }
		[[nodiscard]] auto getNavy() const { return navy - 5; }
		[[nodiscard]] auto getCommerce() const { return commerce - 5; }
		[[nodiscard]] auto getCulture() const { return culture - 5; }
		[[nodiscard]] auto getIndustry() const { return industry - 5; }

		[[nodiscard]] auto getOrder() const { return order - 5; }
		[[nodiscard]] auto getLiberty() const { return liberty - 5; }
		[[nodiscard]] auto getEquality() const { return equality - 5; }
		[[nodiscard]] auto getLiteracy() const { return literacy - 5; }

		[[nodiscard]] auto getReactionary() const { return reactionary - 5; }
		[[nodiscard]] auto getLiberal() const { return liberal - 5; }

		[[nodiscard]] auto getSlavery() const { return slavery - 5; }
		[[nodiscard]] auto getUpper_house_composition() const { return upper_house_composition - 5;  }
		[[nodiscard]] auto getVote_franchise() const { return vote_franchise - 5; }
		[[nodiscard]] auto getVoting_system() const { return voting_system - 5; }
		[[nodiscard]] auto getPublic_meetings() const { return public_meetings - 5; }
		[[nodiscard]] auto getPress_rights() const { return press_rights - 5;	}
		[[nodiscard]] auto getTrade_unions() const { return trade_unions - 5;	}
		[[nodiscard]] auto getPolitical_parties() const { return political_parties - 5; }

	private:
		// Input values range 0-10. However, we return [-5 to 5] because working in [0 to 10] makes no sense internally.
		std::string enforce;

		int slavery = 5;
		int upper_house_composition = 5;
		int vote_franchise = 5;
		int voting_system = 5;
		int public_meetings = 5;
		int press_rights = 5;
		int trade_unions = 5;
		int political_parties = 5;

		int liberty = 5;
		int equality = 5;
		int order = 5;
		int literacy = 5;

		int reactionary = 5;
		int liberal = 5;

		int army = 5;
		int navy = 5;
		int commerce = 5;
		int culture = 5;
		int industry = 5;
	};
}

#endif // IDEA_EFFECTS_H