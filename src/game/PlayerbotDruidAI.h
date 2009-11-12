#ifndef _PLAYERBOTDRUIDAI_H
#define _PLAYERBOTDRUIDAI_H

#include "PlayerbotClassAI.h"

enum
{
    DruidCombat,
    DruidTank,
    DruidHeal,
    DruidSpell
};

//class Player;

class MANGOS_DLL_SPEC PlayerbotDruidAI : PlayerbotClassAI
{
    public:
        PlayerbotDruidAI(Player* const master, Player* const bot, PlayerbotAI* const ai);
        virtual ~PlayerbotDruidAI();

		bool DoFirstCombatManeuver(Unit*);

        // all combat actions go here
        void DoNextCombatManeuver(Unit*);

        // all non combat actions go here, ex buffs, heals, rezzes
        void DoNonCombatActions();

        // buff a specific player, usually a real PC who is not in group
        void BuffPlayer(Player *target);

    private:
        // Heals the target based off its hps
        void HealTarget (Unit& target, uint8 hp);

		// druid cat/bear/dire bear/moonkin/tree of life forms
		uint32 CAT_FORM, BEAR_FORM, DIRE_BEAR_FORM, MOONKIN_FORM, TREE_OF_LIFE;

        // druid cat attacks
        uint32 PROWL, CLAW, COWER, TIGERS_FURY, RAKE, RIP, FEROCIOUS_BITE, MAIM, MANGLE_CAT, SHRED;

		// druid bear/dire bear attacks & buffs
		uint32 FERAL_CHARGE_BEAR, ENRAGE, FAERIE_FIRE_FERAL, BASH, MAUL, SWIPE_BEAR, DEMORALIZING_ROAR, LACERATE, MANGLE_BEAR, CHALLENGING_ROAR, GROWL;

		// druid attacks & debuffs
		uint32 MOONFIRE, ROOTS, WRATH, STARFALL, STARFIRE, INSECT_SWARM, FAERIE_FIRE, FORCE_OF_NATURE, HURRICANE;

		// druid buffs
		uint32 MARK_OF_THE_WILD, THORNS, MANA_REJUVENATION, INNERVATE, BARKSKIN;

		// druid heals
		uint32 LIFEBLOOM, REJUVENATION, REGROWTH, NOURISH, HEALING_TOUCH, WILD_GROWTH, SWIFTMEND, TRANQUILITY;

        uint32 SpellSequence, DruidSpellCombat;

		bool ChargeSuccess;

		float x, y, z;
};

#endif
