
#include "PlayerbotWarlockAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;
PlayerbotWarlockAI::PlayerbotWarlockAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
	// DESTRUCTION
    SHADOW_BOLT           = ai->getSpellId("shadow bolt");
    IMMOLATE              = ai->getSpellId("immolate");
    INCINERATE            = ai->getSpellId("incinerate");
    SEARING_PAIN          = ai->getSpellId("searing pain");
    CONFLAGRATE           = ai->getSpellId("conflagrate");
    SHADOWFURY            = ai->getSpellId("shadowfury");
	CHAOS_BOLT            = ai->getSpellId("chaos bolt");
	SHADOWFLAME           = ai->getSpellId("shadowflame");
	HELLFIRE              = ai->getSpellId("hellfire");
	RAIN_OF_FIRE          = ai->getSpellId("rain of fire");
	SOUL_FIRE             = ai->getSpellId("soul fire"); // soul shard spells
	SHADOWBURN            = ai->getSpellId("shadowburn");
	// CURSE
    CURSE_OF_WEAKNESS     = ai->getSpellId("curse of weakness");
    CURSE_OF_THE_ELEMENTS = ai->getSpellId("curse of the elements");
    CURSE_OF_AGONY        = ai->getSpellId("curse of agony");
    CURSE_OF_EXHAUSTION   = ai->getSpellId("curse of exhaustion");
    CURSE_OF_TONGUES      = ai->getSpellId("curse of tongues");
	CURSE_OF_DOOM         = ai->getSpellId("curse of doom");
	// AFFLICTION
	CORRUPTION            = ai->getSpellId("corruption");
	DRAIN_SOUL            = ai->getSpellId("drain soul");
	DRAIN_LIFE            = ai->getSpellId("drain life");
	DRAIN_MANA            = ai->getSpellId("drain mana");
    LIFE_TAP              = ai->getSpellId("life tap");
    UNSTABLE_AFFLICTION   = ai->getSpellId("unstable affliction");
    HAUNT                 = ai->getSpellId("haunt");
    ATROCITY              = ai->getSpellId("atrocity");
    SEED_OF_CORRUPTION    = ai->getSpellId("seed of corruption");
	DARK_PACT             = ai->getSpellId("dark pact");
	HOWL_OF_TERROR        = ai->getSpellId("howl of terror");
	FEAR                  = ai->getSpellId("fear");
	// DEMONOLOGY
	DEMON_SKIN            = ai->getSpellId("demon skin");
    DEMON_ARMOR           = ai->getSpellId("demon armor");
    FEL_ARMOR             = ai->getSpellId("fel armor");
    SHADOW_WARD           = ai->getSpellId("shadow ward");
    SOULSHATTER           = ai->getSpellId("soulshatter");
	SOUL_LINK             = ai->getSpellId("soul link");
	HEALTH_FUNNEL         = ai->getSpellId("health funnel");
	DETECT_INVISIBILITY   = ai->getSpellId("detect invisibility");
	MASTER_DEMONOLOGIST	  = ai->getSpellId("master demonologist");
	// demon summon
    SUMMON_IMP            = ai->getSpellId("summon imp");
    SUMMON_VOIDWALKER     = ai->getSpellId("summon voidwalker");
    SUMMON_SUCCUBUS       = ai->getSpellId("summon succubus");
    SUMMON_FELHUNTER      = ai->getSpellId("summon felhunter");
    SUMMON_FELGUARD       = ai->getSpellId("summon felguard");
	// demon spells
	FEL_INTELLECT		  = ai->getSpellId("fel intelligence");
	BLOOD_PACT			  = ai->getSpellId("blood pact");
	CONSUME_SHADOWS       = ai->getSpellId("consume shadows");

	m_demonSummonFailed = false;
}

PlayerbotWarlockAI::~PlayerbotWarlockAI() {}

bool PlayerbotWarlockAI::DoFirstCombatManeuver(Unit *pTarget)
{
	PlayerbotAI* ai = GetAI();
    Player *m_bot = GetPlayerBot();
	Pet *pet = m_bot->GetPet();

	LastSpellCurse = LastSpellAffliction = LastSpellDestruction = LastSpellDemonology = 0;

	// check for soul link with demon, also blood & dark pact with imp
    if( pet && SOUL_LINK>0 && !m_bot->HasAura(SOUL_LINK,0) && ai->GetManaPercent() >= 16 && ai->CastSpell(SOUL_LINK,*m_bot) )
		{
			if( ai->GetManager()->m_confDebugWhisper )
				ai->TellMaster( "casting soul link." );
			return false;
		}
	return false;
}


void PlayerbotWarlockAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            if (SHADOW_BOLT > 0)
                ai->CastSpell(SHADOW_BOLT);
            return;
    }

    // ------- Non Duel combat ----------

	//ai->SetMovementOrder( PlayerbotAI::MOVEMENT_FOLLOW, GetMaster() ); // dont want to melee mob

    ai->SetInFront( pTarget );
    Player *m_bot = GetPlayerBot();
	Unit* pVictim = pTarget->getVictim();

	m_bot->Attack(pTarget, true);

	if (m_bot->GetDistance(pTarget)>30)
	{
		ai->DoCombatMovement();
		return;
	}

	// Damage Spells
    ai->SetInFront( pTarget );

    std::ostringstream out;
    switch (SpellSequence)
    {
        case SPELL_DEMONOLOGY:
			out << "Case Curses";
			if (CURSE_OF_AGONY > 0 && !pTarget->HasAura(CURSE_OF_AGONY, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && LastSpellCurse < 2 && ai->GetManaPercent() >= 14 && ai->CastSpell(CURSE_OF_AGONY, *pTarget))
				out << "> Curse of Agony";
			else if (CURSE_OF_THE_ELEMENTS > 0 && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && !pTarget->HasAura(CURSE_OF_AGONY, 0) && !pTarget->HasAura(CURSE_OF_WEAKNESS, 0) && LastSpellCurse < 3 && ai->GetManaPercent() >= 10 && ai->CastSpell(CURSE_OF_THE_ELEMENTS, *pTarget))
				out << "> Curse of Elements";
			else if (CURSE_OF_EXHAUSTION > 0 && !pTarget->HasAura(CURSE_OF_EXHAUSTION, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && !pTarget->HasAura(CURSE_OF_WEAKNESS, 0) && !pTarget->HasAura(CURSE_OF_AGONY, 0) && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS, 0) && LastSpellCurse < 4 && ai->GetManaPercent() >= 6 && ai->CastSpell(CURSE_OF_EXHAUSTION, *pTarget))
				out << "> Curse of Exhaustion";
            else if (CURSE_OF_WEAKNESS > 0 && !pTarget->HasAura(CURSE_OF_WEAKNESS, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && !pTarget->HasAura(CURSE_OF_EXHAUSTION, 0) && !pTarget->HasAura(CURSE_OF_AGONY, 0) && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS, 0) && LastSpellCurse < 5 && ai->GetManaPercent() >= 14 && ai->CastSpell(CURSE_OF_WEAKNESS, *pTarget))
				out << "> Curse of Weakness";
            else if (CURSE_OF_TONGUES > 0 && !pTarget->HasAura(CURSE_OF_TONGUES, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && !pTarget->HasAura(CURSE_OF_WEAKNESS, 0) && !pTarget->HasAura(CURSE_OF_EXHAUSTION, 0) && !pTarget->HasAura(CURSE_OF_AGONY, 0) && !pTarget->HasAura(CURSE_OF_THE_ELEMENTS, 0) && LastSpellCurse < 6 && ai->GetManaPercent() >= 4 && ai->CastSpell(CURSE_OF_TONGUES, *pTarget))
                out << "> Curse of Tongues";
			else
				LastSpellCurse = 0;
			break;

        case SPELL_AFFLICTION:
			out << "Case Affliction:";
            if (LIFE_TAP > 0 && LastSpellAffliction < 2 && ai->GetManaPercent() <= 50 && ai->CastSpell(LIFE_TAP, *m_bot))
				out << "> Life Tap";
			else if (CORRUPTION > 0 && !pTarget->HasAura(CORRUPTION, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && !pTarget->HasAura(SEED_OF_CORRUPTION, 0) && LastSpellAffliction < 3 && ai->GetManaPercent() >= 19 && ai->CastSpell(CORRUPTION, *pTarget))
				out << "> Corruption";
			else if (DRAIN_SOUL > 0 && pTarget->GetHealth() < pTarget->GetMaxHealth()*0.40 && !pTarget->HasAura(DRAIN_SOUL, 0) && LastSpellAffliction < 4 && ai->GetManaPercent() >= 19 && ai->CastSpell(DRAIN_SOUL, *pTarget))
			{
				ai->SetIgnoreUpdateTime(15);
				out << "> Drain Soul";
			}
            else if (DRAIN_LIFE > 0 && LastSpellAffliction < 5 && !pTarget->HasAura(DRAIN_SOUL, 0) && !pTarget->HasAura(SEED_OF_CORRUPTION, 0) && !pTarget->HasAura(DRAIN_LIFE, 0) && !pTarget->HasAura(DRAIN_MANA, 0) && ai->GetHealthPercent() <= 70 && ai->GetManaPercent() >= 23 && ai->CastSpell(DRAIN_LIFE, *pTarget))
			{
				ai->SetIgnoreUpdateTime(5);
				out << "> Drain Life";
			}
			else if (DRAIN_MANA > 0 && pTarget->GetPower(POWER_MANA) > 0 && !pTarget->HasAura(DRAIN_SOUL, 0) && !pTarget->HasAura(DRAIN_MANA, 0) && !pTarget->HasAura(SEED_OF_CORRUPTION, 0) && !pTarget->HasAura(DRAIN_LIFE, 0) && LastSpellAffliction < 6 && ai->GetManaPercent() < 70 && ai->GetManaPercent() >= 17 && ai->CastSpell(DRAIN_MANA, *pTarget))
			{
				ai->SetIgnoreUpdateTime(5);
				out << "> Drain Mana";
			}
            else if (UNSTABLE_AFFLICTION > 0 && LastSpellAffliction < 7 && !pTarget->HasAura(UNSTABLE_AFFLICTION, 0) && !pTarget->HasAura(SHADOWFLAME, 0) && ai->GetManaPercent() >= 20 && ai->CastSpell(UNSTABLE_AFFLICTION, *pTarget))
				out << "> Unstable Affliction";
            else if (HAUNT > 0 && LastSpellAffliction < 8 && !pTarget->HasAura(HAUNT, 0) && ai->GetManaPercent() >= 12 && ai->CastSpell(HAUNT, *pTarget))
				out << "> Haunt";
            else if (ATROCITY > 0 && !pTarget->HasAura(ATROCITY, 0) && LastSpellAffliction < 9 && ai->GetManaPercent() >= 21 && ai->CastSpell(ATROCITY, *pTarget))
				out << "> Atrocity";
            else if (SEED_OF_CORRUPTION > 0 && !pTarget->HasAura(SEED_OF_CORRUPTION, 0) && LastSpellAffliction < 10 && ai->GetManaPercent() >= 34 && ai->CastSpell(SEED_OF_CORRUPTION, *pTarget))
				out << "> Seed of Corruption";
			else if (HOWL_OF_TERROR > 0 && !pTarget->HasAura(HOWL_OF_TERROR, 0) && ai->GetAttackerCount()>3 && LastSpellAffliction < 11 && ai->GetManaPercent() >= 11 && ai->CastSpell(HOWL_OF_TERROR, *pTarget))
				out << "> Howl of Terror";
			else
				LastSpellAffliction = 0;
			break;

          case SPELL_DESTRUCTION:
			  out << "Case Destruction";
			  if (IMMOLATE > 0 && !pTarget->HasAura(IMMOLATE, 0) && LastSpellDestruction < 1 && ai->GetManaPercent() >= 23 && ai->CastSpell(IMMOLATE, *pTarget))
			  {
				  out << "> Immolate";
				  LastSpellDestruction++;
			  }
			  else if (CURSE_OF_AGONY > 0 && !pTarget->HasAura(CURSE_OF_AGONY, 0) && LastSpellCurse < 2 && ai->GetManaPercent() >= 14 && ai->CastSpell(CURSE_OF_AGONY, *pTarget))
			  {
				out << "> Curse of Agony";
				LastSpellDestruction++;
			  }
              else if (CHAOS_BOLT > 0 && LastSpellDestruction < 3 && ai->GetManaPercent() >= 9 && ai->CastSpell(CHAOS_BOLT, *pTarget))
			  {
				  out << "> Chaos Bolt";
				  LastSpellDestruction++;
			  }
			  else if (INCINERATE > 0 && LastSpellDestruction < 6 && ai->GetManaPercent() >= 19 && ai->CastSpell(INCINERATE, *pTarget))
			  {
				  out << "> Incinerate";
				  LastSpellDestruction++;
			  }
			  else if (CONFLAGRATE > 0 && LastSpellDestruction < 7 && ai->GetManaPercent() >= 16 && ai->CastSpell(CONFLAGRATE, *pTarget))
			  {
				  out << "> Conflagrate";
				  LastSpellDestruction++;
			  }
			  else
				  LastSpellDestruction = 0;
			  break;
    }
    if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );
} // end DoNextCombatManeuver

void PlayerbotWarlockAI::DoNonCombatActions()
{
	PlayerbotAI* ai = GetAI();
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

	Pet *pet = m_bot->GetPet();

	if (m_bot->HasAura(FEL_INTELLECT, 0)) SpellSequence = SPELL_AFFLICTION;
	else if (m_bot->HasAura(MASTER_DEMONOLOGIST, 0)) SpellSequence = SPELL_DEMONOLOGY;
	else SpellSequence = SPELL_DESTRUCTION;

    // buff myself  DEMON_SKIN, DEMON_ARMOR, FEL_ARMOR
    if (FEL_ARMOR > 0)
        (!m_bot->HasAura(FEL_ARMOR, 0) && ai->CastSpell(FEL_ARMOR, *m_bot));
    else if (DEMON_ARMOR > 0)
        (!m_bot->HasAura(DEMON_ARMOR, 0) && !m_bot->HasAura(FEL_ARMOR, 0) && ai->CastSpell(DEMON_ARMOR, *m_bot));
    else if (DEMON_SKIN > 0)
        (!m_bot->HasAura(DEMON_SKIN, 0) && !m_bot->HasAura(FEL_ARMOR, 0) && !m_bot->HasAura(DEMON_ARMOR, 0) && ai->CastSpell(DEMON_SKIN, *m_bot));

	// buff myself & master DETECT_INVISIBILITY
    if (DETECT_INVISIBILITY > 0)
        (!m_bot->HasAura(DETECT_INVISIBILITY, 0) && ai->GetManaPercent() >= 2 && ai->CastSpell(DETECT_INVISIBILITY, *m_bot));
    if (DETECT_INVISIBILITY > 0)
        (!GetMaster()->HasAura(DETECT_INVISIBILITY, 0) && ai->GetManaPercent() >= 2 && ai->CastSpell(DETECT_INVISIBILITY, *GetMaster()));

	// hp & mana check
	if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
         m_bot->SetStandState(UNIT_STAND_STATE_STAND);

	if (ai->GetManaPercent() < 60 || ai->GetHealthPercent() < 60)
		ai->Feast();

    // check for demon
    if( SUMMON_FELGUARD>0 || SUMMON_FELHUNTER>0 || SUMMON_SUCCUBUS>0 || SUMMON_VOIDWALKER>0 || SUMMON_IMP>0 && !m_demonSummonFailed )
    {
        if( !pet )
        {
            // summon demon
			if( SUMMON_FELGUARD>0 && ai->CastSpell(SUMMON_FELGUARD,*m_bot) )
                ai->TellMaster( "summoning felguard." );
			else if( SUMMON_FELHUNTER>0 && ai->CastSpell(SUMMON_FELHUNTER,*m_bot) )
                ai->TellMaster( "summoning felhunter." );
			else if( SUMMON_SUCCUBUS>0 && ai->CastSpell(SUMMON_SUCCUBUS,*m_bot) )
                ai->TellMaster( "summoning succubus." );
			else if( SUMMON_VOIDWALKER>0 && ai->CastSpell(SUMMON_VOIDWALKER,*m_bot) )
                ai->TellMaster( "summoning voidwalker." );
			else if( SUMMON_IMP>0 && ai->GetManaPercent() >= 64 && ai->CastSpell(SUMMON_IMP,*m_bot) )
                ai->TellMaster( "summoning imp." );
			else
            {
                m_demonSummonFailed = true;
                ai->TellMaster( "summon demon failed!" );
            }
        }
    }

	// check for soul link with demon, also blood & dark pact with imp
    /*if( pet && SOUL_LINK>0 && !m_bot->HasAura(SOUL_LINK,0) && ai->GetManaPercent() >= 16 && ai->CastSpell(SOUL_LINK,*m_bot) )
		{
			if( ai->GetManager()->m_confDebugWhisper )
				ai->TellMaster( "casting soul link." );
		}
	else if( pet && DARK_PACT>0 && ai->GetManaPercent() <= 75 && pet->GetPower(POWER_MANA) > 0 && ai->CastSpell(DARK_PACT,*m_bot) )
		{
			if( ai->GetManager()->m_confDebugWhisper )
				ai->TellMaster( "casting dark pact." );
		}*/
} // end DoNonCombatActions
