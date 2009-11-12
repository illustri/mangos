    /*
    Name    : PlayerbotPaladinAI.cpp
    Complete: maybe around 27% :D
    Author  : Natsukawa
    Version : 0.35
    */

#include "PlayerbotPaladinAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;

PlayerbotPaladinAI::PlayerbotPaladinAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{

    RETRIBUTION_AURA             = ai->getSpellId("retribution aura");
    SHADOW_RESISTANCE_AURA       = ai->getSpellId("shadow resistance aura");
    DEVOTION_AURA                = ai->getSpellId("devotion aura");
    FIRE_RESISTANCE_AURA         = ai->getSpellId("fire resistance aura");
    FROST_RESISTANCE_AURA        = ai->getSpellId("frost resistance aura");
    CONCENTRATION_AURA           = ai->getSpellId("concentration aura");
    CRUSADER_AURA                = ai->getSpellId("crusader aura");
    CRUSADER_STRIKE              = ai->getSpellId("crusader strike");
    SEAL_OF_COMMAND              = 20375; //For some reason getSpellId was not working. Replaced with spell id
    JUDGEMENT_OF_LIGHT           = ai->getSpellId("judgement of light");
    JUDGEMENT_OF_WISDOM          = ai->getSpellId("judgement of wisdom");
    FLASH_OF_LIGHT               = ai->getSpellId("flash of light");
    HOLY_LIGHT                   = ai->getSpellId("holy light");
    DIVINE_SHIELD                = 642; // ai->getSpellId("divine shield");
    HAND_OF_PROTECTION           = 10278;
    BLESSING_OF_MIGHT            = ai->getSpellId("blessing of might");
    GREATER_BLESSING_OF_MIGHT    = ai->getSpellId("greater blessing of might");
    BLESSING_OF_WISDOM           = ai->getSpellId("blessing of wisdom");
    GREATER_BLESSING_OF_WISDOM   = ai->getSpellId("greater blessing of wisdom");
    BLESSING_OF_KINGS            = ai->getSpellId("blessing of kings");
    GREATER_BLESSING_OF_KINGS    = ai->getSpellId("greater blessing of kings");
    HAMMER_OF_JUSTICE            = ai->getSpellId("hammer of justice");
    RIGHTEOUS_FURY               = ai->getSpellId("righteous fury");
    CONSECRATION                 = ai->getSpellId("consecration");
    AVENGING_WRATH               = ai->getSpellId("avenging wrath");
    HAMMER_OF_WRATH              = ai->getSpellId("hammer of wrath");
	DIVINE_STORM				 = ai->getSpellId("divine storm");
	SEAL_OF_VENGEANCE			 = ai->getSpellId("seal of vengeance");
}

PlayerbotPaladinAI::~PlayerbotPaladinAI() {}

void PlayerbotPaladinAI::HealTarget(Unit &target, uint8 hp)
{
    PlayerbotAI* ai = GetAI();

    if (hp < 50 && FLASH_OF_LIGHT > 0 && ai->GetManaPercent() >= 16 && ai->CastSpell(FLASH_OF_LIGHT, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting flash of light.");
	}
	else if (hp < 75 && HOLY_LIGHT  > 0 && ai->GetManaPercent() >= 29 && ai->CastSpell(HOLY_LIGHT, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting holy light.");
	}
} // end HealTarget


bool PlayerbotPaladinAI::DoFirstCombatManeuver(Unit *pTarget)
{
	CombatCounter = 0;
	return false;
}

void PlayerbotPaladinAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
	if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            if (HAMMER_OF_JUSTICE > 0)
                ai->CastSpell(HAMMER_OF_JUSTICE);
            return;
    }

    // damage spells
    ai->SetInFront( pTarget );
    Player *m_bot = GetPlayerBot();
	Group *m_group = m_bot->GetGroup();
	PlayerbotAI::CombatOrderType co = ai->GetCombatOrder();

	m_bot->Attack(pTarget, true);

	if (m_bot->GetDistance(pTarget)>ATTACK_DISTANCE)
	{
		ai->DoCombatMovement();
		return;
	}

    //Shield master if low hp.
    uint32 masterHP = GetMaster()->GetHealth()*100 / GetMaster()->GetMaxHealth();

    if (GetMaster()->isAlive() && masterHP < 25 && HAND_OF_PROTECTION > 0 && !GetMaster()->HasAura(HAND_OF_PROTECTION, 0) && ai->CastSpell(HAND_OF_PROTECTION, *(GetMaster())))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("Hand of Protection");
	}

    // Heal master
    if (GetMaster()->isAlive())
    {
        if (masterHP < 75)
            HealTarget (*GetMaster(), masterHP);
    }

    // Heal group
    if( m_group )
    {
        Group::MemberSlotList const& groupSlot = m_group->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *m_groupMember = sObjectMgr.GetPlayer( itr->guid );
            if( !m_groupMember || !m_groupMember->isAlive() )
                continue;

            uint32 memberHP = m_groupMember->GetHealth()*100 / m_groupMember->GetMaxHealth();
            if( memberHP < 30 )
                HealTarget( *m_groupMember, memberHP );
        }
    }

    if (SHADOW_RESISTANCE_AURA > 0 && !m_bot->HasAura(SHADOW_RESISTANCE_AURA, 0) && pTarget->getClass() == CLASS_WARLOCK)
        ai->CastSpell (SHADOW_RESISTANCE_AURA, *m_bot);

    if (DEVOTION_AURA > 0 && !m_bot->HasAura(DEVOTION_AURA, 0) && pTarget->getClass() == CLASS_WARRIOR)
        ai->CastSpell (DEVOTION_AURA, *m_bot);

    if (FIRE_RESISTANCE_AURA > 0 && !m_bot->HasAura(FIRE_RESISTANCE_AURA, 0) && pTarget->getClass() == CLASS_MAGE)
        ai->CastSpell (FIRE_RESISTANCE_AURA, *m_bot);

    if (RETRIBUTION_AURA > 0 && !m_bot->HasAura(RETRIBUTION_AURA, 0) && pTarget->getClass() == CLASS_PRIEST)
        ai->CastSpell (RETRIBUTION_AURA, *m_bot);

    if (DEVOTION_AURA > 0 && !m_bot->HasAura(DEVOTION_AURA, 0) && pTarget->getClass() == CLASS_SHAMAN)
        ai->CastSpell (DEVOTION_AURA, *m_bot);

    if (DEVOTION_AURA > 0 && !m_bot->HasAura(DEVOTION_AURA, 0) && pTarget->getClass() == CLASS_ROGUE)
        ai->CastSpell (DEVOTION_AURA, *m_bot);

    if (DEVOTION_AURA > 0 && !m_bot->HasAura(DEVOTION_AURA, 0) && pTarget->getClass() == CLASS_PALADIN)
        ai->CastSpell (DEVOTION_AURA, *m_bot);

    if (ai->GetHealthPercent() < 75 || co&PlayerbotAI::ORDERS_HEAL) SpellSequence = Healing;
    else SpellSequence = Combat;

	std::ostringstream out;
    switch (SpellSequence)
    {
        case Combat:
            out << "Case Combat";
            if (SEAL_OF_VENGEANCE > 0 && CombatCounter < 1 && ai->GetManaPercent() >= 25 && ai->CastSpell (SEAL_OF_COMMAND, *m_bot))
			{
				out << "> Seal of Vengeance";
				CombatCounter++;
			}
            else if (CRUSADER_STRIKE > 0 && CombatCounter < 2 && ai->GetManaPercent() >=15 && ai->CastSpell (CRUSADER_STRIKE, *pTarget))
			{
				out << "> Crusader Strike";
				CombatCounter++;
			}
			else if (JUDGEMENT_OF_LIGHT > 0 && CombatCounter < 3 && ai->GetManaPercent() >=15 && ai->CastSpell (JUDGEMENT_OF_LIGHT, *pTarget))
			{
				out << "> Judgement of Light";
				CombatCounter++;
			}
			else if (DIVINE_STORM > 0 && CombatCounter < 4 && ai->GetManaPercent() >=12 && ai->CastSpell (DIVINE_STORM, *pTarget))
			{
				out << "> Divine Storm";
				CombatCounter++;
			}
			else if (HAMMER_OF_JUSTICE > 0 && CombatCounter < 5 && ai->GetManaPercent() >=15 && ai->CastSpell (HAMMER_OF_JUSTICE, *pTarget))
			{
				out << "> Hammer of Justice";
				CombatCounter++;
			}
            else
				CombatCounter = 0;
			break;

        case Healing:
            out << "Case Healing";
            if (FLASH_OF_LIGHT > 0 && ai->GetHealthPercent() < 50 && ai->GetManaPercent() >= 16 && ai->CastSpell(FLASH_OF_LIGHT, *m_bot))
				out << "> Flash of Light";
			else if (HOLY_LIGHT > 0 && ai->GetHealthPercent() < 75 && ai->GetManaPercent() >= 20 && ai->CastSpell(HOLY_LIGHT, *m_bot))
				out << "> Holy Light";
			break;
	}

	if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );

    if (HAMMER_OF_WRATH > 0 && pTarget->GetHealth() < pTarget->GetMaxHealth()*0.2 && ai->GetManaPercent() >=15)
        ai->CastSpell(HAMMER_OF_WRATH, *pTarget);

    if (AVENGING_WRATH > 0 && ai->GetManaPercent() >= 8)
        ai->CastSpell(AVENGING_WRATH);

    if (DIVINE_SHIELD > 0 && ai->GetHealthPercent() < 30 && ai->GetManaPercent() >= 3)
        ai->CastSpell(DIVINE_SHIELD, *m_bot);
}

void PlayerbotPaladinAI::DoNonCombatActions()
{
	PlayerbotAI* ai = GetAI();
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    // buff myself
    if (BLESSING_OF_MIGHT > 0 && !m_bot->HasAura(BLESSING_OF_MIGHT, 0))
        ai->CastSpell (BLESSING_OF_MIGHT, *m_bot);

    if (RIGHTEOUS_FURY > 0 && !m_bot->HasAura(RIGHTEOUS_FURY, 0))
        ai->CastSpell (RIGHTEOUS_FURY, *m_bot);

    if (SEAL_OF_COMMAND > 0)
        ai->CastSpell (SEAL_OF_COMMAND, *m_bot);

    //Select Class buff seq.
    ///Process Who is my master --> get the player class --> aura already present if not then proced --> cast the spell
    //Priest
    if (BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_PRIEST && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0) && !GetMaster()->HasAura(BLESSING_OF_WISDOM, 0))
        ai->CastSpell (BLESSING_OF_WISDOM, *(GetMaster()));

    if (GREATER_BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_PRIEST && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0))
        ai->CastSpell (GREATER_BLESSING_OF_WISDOM, *(GetMaster()));

    //Mage
    if (BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_MAGE && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0) && !GetMaster()->HasAura(BLESSING_OF_WISDOM, 0))
        ai->CastSpell (BLESSING_OF_WISDOM, *(GetMaster()));

    if (GREATER_BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_MAGE && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0))
        ai->CastSpell (GREATER_BLESSING_OF_WISDOM, *(GetMaster()));

    //Paladin
    if (BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_PALADIN && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0) && !GetMaster()->HasAura(BLESSING_OF_WISDOM, 0))
        ai->CastSpell (BLESSING_OF_WISDOM, *(GetMaster()));

    if (GREATER_BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_PALADIN && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0))
        ai->CastSpell (GREATER_BLESSING_OF_WISDOM, *(GetMaster()));

    //Warlock
    if (BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_WARLOCK && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0) && !GetMaster()->HasAura(BLESSING_OF_WISDOM, 0))
        ai->CastSpell (BLESSING_OF_WISDOM, *(GetMaster()));

    if (GREATER_BLESSING_OF_WISDOM > 0 && GetMaster()->getClass() == CLASS_WARLOCK && !GetMaster()->HasAura(GREATER_BLESSING_OF_WISDOM, 0))
        ai->CastSpell (GREATER_BLESSING_OF_WISDOM, *(GetMaster()));

    //Warrior
    if (BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_WARRIOR && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0) && !GetMaster()->HasAura(BLESSING_OF_MIGHT, 0) && !GetMaster()->HasAura(DEFENSIVE_STANCE, 0))
        ai->CastSpell (BLESSING_OF_MIGHT, *(GetMaster()));

    if (GREATER_BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_WARRIOR && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0) && !GetMaster()->HasAura(DEFENSIVE_STANCE, 0))
        ai->CastSpell (GREATER_BLESSING_OF_MIGHT, *(GetMaster()));

    if (BLESSING_OF_KINGS > 0 && GetMaster()->getClass() == CLASS_WARRIOR && !GetMaster()->HasAura(GREATER_BLESSING_OF_KINGS, 0) && !GetMaster()->HasAura(BLESSING_OF_KINGS, 0) && !GetMaster()->HasAura(BERSERKER_STANCE, 0) && !GetMaster()->HasAura(BATTLE_STANCE, 0))
        ai->CastSpell (BLESSING_OF_KINGS, *(GetMaster()));

    if (GREATER_BLESSING_OF_KINGS > 0 && GetMaster()->getClass() == CLASS_WARRIOR && !GetMaster()->HasAura(GREATER_BLESSING_OF_KINGS, 0) && !GetMaster()->HasAura(BERSERKER_STANCE, 0) && !GetMaster()->HasAura(BATTLE_STANCE, 0))
        ai->CastSpell (GREATER_BLESSING_OF_KINGS, *(GetMaster()));

    //Rogue
    if (BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_ROGUE && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0) && !GetMaster()->HasAura(BLESSING_OF_MIGHT, 0))
        ai->CastSpell (BLESSING_OF_MIGHT, *(GetMaster()));

    if (GREATER_BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_ROGUE && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0))
        ai->CastSpell (GREATER_BLESSING_OF_MIGHT, *(GetMaster()));

    //Shaman
    if (BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_SHAMAN && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0) && !GetMaster()->HasAura(BLESSING_OF_MIGHT, 0))
        ai->CastSpell (BLESSING_OF_MIGHT, *(GetMaster()));

    if (GREATER_BLESSING_OF_MIGHT > 0 && GetMaster()->getClass() == CLASS_SHAMAN && !GetMaster()->HasAura(GREATER_BLESSING_OF_MIGHT, 0))
        ai->CastSpell (GREATER_BLESSING_OF_MIGHT, *(GetMaster()));

	// hp & mana check
	if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
         m_bot->SetStandState(UNIT_STAND_STATE_STAND);

	if (GetAI()->GetManaPercent() < 60 || GetAI()->GetHealthPercent() < 60)
		GetAI()->Feast();

    //This is a paladin, self healing maybe? ;D Out of combat he can take care of him self, no ned to be healed.
    //Causes server to crash in some cases /disabled for now/
/*    if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);

    if (HOLY_LIGHT > 0 && ai->GetHealthPercent() < 98 && ai->GetManaPercent() >= 41)
        ai->CastSpell(HOLY_LIGHT);
*/
}

void PlayerbotPaladinAI::BuffPlayer(Player* target)
{
    GetAI()->CastSpell(BLESSING_OF_MIGHT, *target);
}
