
#include "PlayerbotPriestAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;

PlayerbotPriestAI::PlayerbotPriestAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    if((HEAL = ai->getSpellId ("greater heal"))>0)
        HEAL           = ai->getSpellId("greater heal");
    else if((HEAL = ai->getSpellId ("heal"))>0 && (HEAL = ai->getSpellId ("greater heal"))==0)
        HEAL           = ai->getSpellId("heal");
    else if((HEAL = ai->getSpellId ("greater heal"))==0 && (HEAL = ai->getSpellId ("heal"))==0)
        HEAL           = ai->getSpellId("lesser heal");

	RENEW              = ai->getSpellId("renew");
	GREAT_HEAL		   = ai->getSpellId("greater heal");
    FLASH_HEAL         = ai->getSpellId("flash heal");
    REZZ               = ai->getSpellId("resurrection");
    SMITE              = ai->getSpellId("smite");
    MANA_BURN          = ai->getSpellId("mana burn");
    HOLY_NOVA          = ai->getSpellId("holy nova");
    HOLY_FIRE          = ai->getSpellId("holy fire");
    DESPERATE_PRAYER   = ai->getSpellId("desperate prayer");
    PRAYER_OF_HEALING  = ai->getSpellId("prayer of healing");
    CIRCLE_OF_HEALING  = ai->getSpellId("circle of healing");
    BINDING_HEAL       = ai->getSpellId("binding heal");
    PRAYER_OF_MENDING  = ai->getSpellId("prayer of mending");
    //SHADOWMAGIC
    FADE               = ai->getSpellId("fade");
    PAIN               = ai->getSpellId("shadow word: pain");
    MIND_BLAST         = ai->getSpellId("mind blast");
    SCREAM             = ai->getSpellId("psychic scream");
    MIND_FLAY          = ai->getSpellId("mind flay");
    DEVOURING_PLAGUE   = ai->getSpellId("devouring plague");
    SHADOW_PROTECTION  = ai->getSpellId("shadow protection");
    VAMPIRIC_TOUCH     = ai->getSpellId("vampiric touch");
    PRAYER_OF_SHADOW_PROTECTION = ai->getSpellId("prayer of shadow protection");
    SHADOWFIEND        = ai->getSpellId("shadowfiend");
    MIND_SEAR          = ai->getSpellId("mind sear");
	SHADOWFORM		   = ai->getSpellId("shadowform");
    //DISCIPLINE
	PENANCE			   = ai->getSpellId("penance");
    INNER_FIRE         = ai->getSpellId("inner fire");
    PWS                = ai->getSpellId("power word: shield");
    if((FORTITUDE = ai->getSpellId ("prayer of fortitude"))==1)
        FORTITUDE      = ai->getSpellId("prayer of fortitude");
    if((FORTITUDE = ai->getSpellId ("power word: fortitude"))==1 && (FORTITUDE = ai->getSpellId ("prayer of fortitude"))==0)
        FORTITUDE      = ai->getSpellId("power word: fortitude");

    FEAR_WARD          = ai->getSpellId("fear ward");
    if((DSPIRIT = ai->getSpellId ("prayer of spirit"))==1)
        DSPIRIT        = ai->getSpellId("prayer of spirit");
    if((DSPIRIT = ai->getSpellId ("divine spirit"))==1 && (DSPIRIT = ai->getSpellId ("prayer of spirit"))==0)
        DSPIRIT        = ai->getSpellId("divine spirit");

    MASS_DISPEL        = ai->getSpellId("mass dispel");
    POWER_INFUSION     = ai->getSpellId("power infusion");
	DIVINE_SPIRIT      = ai->getSpellId("divine spirit");
	INNER_FOCUS        = ai->getSpellId("inner focus");
}

PlayerbotPriestAI::~PlayerbotPriestAI() {}

void PlayerbotPriestAI::HealTarget(Unit &target, uint8 hp)
{
    PlayerbotAI* ai = GetAI();

    if (hp < 40 && PENANCE > 0 && ai->GetManaPercent() >= 16 && ai->CastSpell(PENANCE, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting penance.");
	}
	else if (hp < 50 && PWS > 0 && ai->CastSpell(PWS, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting PWS.");
	}
	else if (hp < 60 && FLASH_HEAL > 0 && ai->GetManaPercent() >= 20 && ai->CastSpell(FLASH_HEAL, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting flash heal.");
	}
    else if (hp < 70 && CIRCLE_OF_HEALING > 0 && ai->GetManaPercent() >= 24 && ai->CastSpell(CIRCLE_OF_HEALING, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting circle of healing.");
    }
	else if (hp < 80 && GREAT_HEAL > 0 && ai->GetManaPercent() >= 36 && ai->CastSpell(GREAT_HEAL, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting greater heal.");
    }
    else if (hp < 90 && RENEW > 0 && ai->GetManaPercent() >= 19 && ai->CastSpell(RENEW, target))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting renew.");
    }

} // end HealTarget

bool PlayerbotPriestAI::DoFirstCombatManeuver(Unit *pTarget)
{
	LastSpellHoly = LastSpellShadowMagic = LastSpellDiscipline = 0;
	return false;
}

void PlayerbotPriestAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            (ai->HasAura(SCREAM,*pTarget) && ai->GetHealthPercent() < 60 && ai->CastSpell(HEAL)) ||
                ai->CastSpell(PAIN) ||
                (ai->GetHealthPercent() < 80 && ai->CastSpell(RENEW)) ||
                (ai->GetPlayerBot()->GetDistance(pTarget) <= 5 && ai->CastSpell(SCREAM)) ||
                ai->CastSpell(MIND_BLAST) ||
                (ai->GetHealthPercent() < 20 && ai->CastSpell(GREAT_HEAL)) ||
                ai->CastSpell(SMITE);
            return;
    }

    // ------- Non Duel combat ----------

    //ai->SetMovementOrder( PlayerbotAI::MOVEMENT_FOLLOW, GetMaster() ); // dont want to melee mob

    Player *m_bot = GetPlayerBot();
    Group *m_group = m_bot->GetGroup();
	PlayerbotAI::CombatOrderType co = ai->GetCombatOrder();

    // Heal myself
    if (ai->GetHealthPercent() < 75 && FADE > 0 && !m_bot->HasAura(FADE, 0) && ai->CastSpell(FADE))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting fade.");
    }
    else if (ai->GetHealthPercent() < 50 && PWS > 0 && !m_bot->HasAura(PWS, 0) && ai->CastSpell(PWS))
    {
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting pws on myself.");
    }
    else if (ai->GetHealthPercent() < 80)
        HealTarget (*m_bot, ai->GetHealthPercent());

    // Heal master
    uint32 masterHP = GetMaster()->GetHealth()*100 / GetMaster()->GetMaxHealth();
    if (GetMaster()->isAlive())
    {
        if (masterHP < 50 && PWS > 0 && !GetMaster()->HasAura(PWS, 0) && ai->CastSpell(PWS, *(GetMaster())))
		{
			if( ai->GetManager()->m_confDebugWhisper )
				ai->TellMaster("I'm casting pws on you.");
		}
        else if (masterHP < 90)
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
            if( memberHP < 90 )
                HealTarget( *m_groupMember, memberHP );
        }
    }

    // Damage Spells
    ai->SetInFront( pTarget );

	m_bot->Attack(pTarget, true);

	if (m_bot->GetDistance(pTarget)>30)
	{
		ai->DoCombatMovement();
		return;
	}

	if (co&PlayerbotAI::ORDERS_HEAL) SpellSequence = SPELL_HEAL;
	else if (m_bot->HasAura(SHADOWFORM, 0)) SpellSequence = SPELL_SHADOWMAGIC;

	std::ostringstream out;
    switch (SpellSequence)
    {
		case SPELL_HEAL:
			out << "Case Heal";
			break;

        case SPELL_HOLY:
			out << "Case Holy";
            if (SMITE > 0 && !pTarget->HasAura(SMITE, 0) && LastSpellHoly <1 && ai->GetManaPercent() >= 17 && ai->CastSpell(SMITE, *pTarget))
			{
				out << "> Smite";
				LastSpellHoly++;
			}
            else if (MANA_BURN > 0 && pTarget->GetPower(POWER_MANA) > 0 && LastSpellHoly <2 && ai->GetManaPercent() < 70 && ai->GetManaPercent() >= 14 && ai->CastSpell(MANA_BURN, *pTarget))
			{
				out << "> Mana Burn";
				LastSpellHoly++;
			}
            else if (HOLY_NOVA > 0 && LastSpellHoly <3 && ai->GetManaPercent() >= 22 && ai->CastSpell(HOLY_NOVA))
			{
				out << "> Holy Nova";
				LastSpellHoly++;
			}
			else if (HOLY_FIRE > 0 && !pTarget->HasAura(HOLY_FIRE, 0) && LastSpellHoly <4 && ai->GetManaPercent() >= 13 && ai->CastSpell(HOLY_FIRE, *pTarget))
			{
				out << "> Holy Fire";
				LastSpellHoly++;
			}
			else
				LastSpellHoly = 0;
			break;

        case SPELL_SHADOWMAGIC:
			out << "Case Shadow";
            if (VAMPIRIC_TOUCH > 0 && !pTarget->HasAura(VAMPIRIC_TOUCH, 0) && LastSpellShadowMagic <1 && ai->GetManaPercent() >= 18 && ai->CastSpell(VAMPIRIC_TOUCH, *pTarget))
			{
				out << "> Vampiric Touch";
				LastSpellShadowMagic++;
			}
			else if (PAIN > 0 && !pTarget->HasAura(PAIN, 0) && LastSpellShadowMagic <2 && ai->GetManaPercent() >= 25 && ai->CastSpell(PAIN, *pTarget))
			{
				out << "> Shadow Word: Pain";
				LastSpellShadowMagic++;
			}
            else if (MIND_BLAST > 0 && LastSpellShadowMagic <3 && ai->GetManaPercent() >= 19 && ai->CastSpell(MIND_BLAST, *pTarget))
			{
				out << "> Mind Blast";
				LastSpellShadowMagic++;
			}
			else if (SCREAM > 0 && ai->GetAttackerCount()>=4 && LastSpellShadowMagic <4 && ai->GetManaPercent() >= 15 && ai->CastSpell(SCREAM))
			{
				out << "> Scream";
				LastSpellShadowMagic++;
			}
            else if (MIND_FLAY > 0 && !pTarget->HasAura(MIND_FLAY, 0) && LastSpellShadowMagic <5 && ai->GetManaPercent() >= 10 && ai->CastSpell(MIND_FLAY, *pTarget))
			{
				out << "> Mind Flay";
				LastSpellShadowMagic++;
			}
            else if (DEVOURING_PLAGUE > 0 && !pTarget->HasAura(DEVOURING_PLAGUE, 0) && LastSpellShadowMagic <6 && ai->GetManaPercent() >= 28 && ai->CastSpell(DEVOURING_PLAGUE, *pTarget))
			{
				out << "> Devouring Plague";
				LastSpellShadowMagic++;
			}
            else if (SHADOWFIEND > 0 && LastSpellShadowMagic <7 && ai->CastSpell(SHADOWFIEND))
			{
				out << "> Shadowfiend";
				LastSpellShadowMagic++;
			}
            else if (MIND_SEAR > 0 && ai->GetAttackerCount()>=3 && LastSpellShadowMagic <8 && ai->GetManaPercent() >= 28 && ai->CastSpell(MIND_SEAR, *pTarget))
			{
				out << "> Mind Sear";
				LastSpellShadowMagic++;
			}
            else
				LastSpellShadowMagic=0;

        case SPELL_DISCIPLINE:
			out << "Case Dicipline";
            if (POWER_INFUSION > 0 && LastSpellDiscipline <1 && ai->GetManaPercent() >= 16 && !m_bot->HasAura(POWER_INFUSION, 0) && ai->CastSpell(POWER_INFUSION, *m_bot))
			{
				out << "> Power Infusion";
				LastSpellDiscipline++;
			}
            else if (MASS_DISPEL > 0 && LastSpellDiscipline <2 && ai->GetManaPercent() >= 33 && ai->CastSpell(MASS_DISPEL))
			{
				out << "> Mass Dispel";
				LastSpellDiscipline++;
			}
			else if (INNER_FOCUS > 0 && LastSpellDiscipline <3 && !m_bot->HasAura(INNER_FOCUS, 0) && ai->CastSpell(INNER_FOCUS, *m_bot))
			{
				out << "> Inner Focus";
				LastSpellDiscipline++;
			}
            else if (PENANCE > 0 && LastSpellDiscipline <4 && ai->GetManaPercent() >= 16 && ai->CastSpell(PENANCE, *pTarget))
			{
				out << "> Penance";
				LastSpellDiscipline++;
			}
			else
				LastSpellDiscipline = 0;
			break;
    }
	if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );
} // end DoNextCombatManeuver

void PlayerbotPriestAI::DoNonCombatActions()
{
	PlayerbotAI* ai = GetAI();
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    SpellSequence = SPELL_HOLY;

    // buff myself
    if (FORTITUDE > 0)
        (!m_bot->HasAura(FORTITUDE, 0) && ai->CastSpell (FORTITUDE, *m_bot));

    if (INNER_FIRE > 0)
        (!m_bot->HasAura(INNER_FIRE, 0) && ai->CastSpell (INNER_FIRE, *m_bot));

	// buff master
	if (FORTITUDE > 0)
		(!GetMaster()->HasAura(FORTITUDE, 0) && ai->CastSpell(FORTITUDE,*(GetMaster())) );

	// hp & mana check
	if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
         m_bot->SetStandState(UNIT_STAND_STATE_STAND);

	if (GetAI()->GetManaPercent() < 60 || GetAI()->GetHealthPercent() < 60)
		GetAI()->Feast();

    // buff and heal master's group
    if (GetMaster()->GetGroup())
    {
        Group::MemberSlotList const& groupSlot = GetMaster()->GetGroup()->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *tPlayer = sObjectMgr.GetPlayer(uint64 (itr->guid));
            if( !tPlayer )
                continue;

            // first rezz em
            if ( !tPlayer->isAlive() && !tPlayer->GetPlayerbotAI() )
            {
                std::string msg = "rezzing ";
                msg += tPlayer->GetName();
                GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
                ai->CastSpell(REZZ, *tPlayer);
                // rez is only 10 sec, but give time for lag
                ai->SetIgnoreUpdateTime(17);
            }
            else if( tPlayer->isAlive() )
            {
                // buff and heal
                (!tPlayer->HasAura(FORTITUDE,0) && ai->CastSpell (FORTITUDE, *tPlayer));
                (HealTarget(*tPlayer, tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth()));
            }
        }
    }
} // end DoNonCombatActions

void PlayerbotPriestAI::BuffPlayer(Player* target)
{
    GetAI()->CastSpell(FORTITUDE, *target);
}
