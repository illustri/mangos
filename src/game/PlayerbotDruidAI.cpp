/*
    Name    : PlayerbotDruidAI.cpp
    Complete: maybe around 33%
    Authors : rrtn, Natsukawa
    Version : 0.42
*/
#include "PlayerbotDruidAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;

PlayerbotDruidAI::PlayerbotDruidAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{

    MOONFIRE            = ai->getSpellId("moonfire"); // attacks
    STARFIRE            = ai->getSpellId("starfire");
    STARFALL            = ai->getSpellId("starfall");
    WRATH               = ai->getSpellId("wrath");
    ROOTS               = ai->getSpellId("roots");
	INSECT_SWARM        = ai->getSpellId("insect swarm");
	FORCE_OF_NATURE     = ai->getSpellId("force of nature");
	HURRICANE           = ai->getSpellId("hurricane");
    MARK_OF_THE_WILD    = ai->getSpellId("mark of the wild"); // buffs
    THORNS              = ai->getSpellId("thorns");
	BARKSKIN            = ai->getSpellId("barkskin");
	INNERVATE           = ai->getSpellId("innervate");
	MANA_REJUVENATION   = ai->getSpellId("mana rejuvenation");
    FAERIE_FIRE         = ai->getSpellId("faerie fire"); // debuffs
    REJUVENATION        = ai->getSpellId("rejuvenation"); // heals
    REGROWTH            = ai->getSpellId("regrowth");
	WILD_GROWTH         = ai->getSpellId("wild growth");
    LIFEBLOOM           = ai->getSpellId("lifebloom");
    NOURISH             = ai->getSpellId("nourish");
    HEALING_TOUCH       = ai->getSpellId("healing touch");
	SWIFTMEND           = ai->getSpellId("swiftmend");
	TRANQUILITY         = ai->getSpellId("tranquility");
    //Druid Forms
	MOONKIN_FORM        = ai->getSpellId("moonkin form");
    DIRE_BEAR_FORM      = ai->getSpellId("dire bear form");
	BEAR_FORM           = ai->getSpellId("bear form");
    CAT_FORM            = ai->getSpellId("cat form");
	TREE_OF_LIFE        = 33891;
    //Cat Attack type's
	PROWL				= ai->getSpellId("prowl");
    RAKE                = ai->getSpellId("rake");            //40 energy
	SHRED				= ai->getSpellId("shred");
    CLAW                = ai->getSpellId("claw");            //45
    COWER               = ai->getSpellId("cower");           //20
    MANGLE_CAT          = ai->getSpellId("mangle (cat)");    //45
    TIGERS_FURY         = ai->getSpellId("tigers's fury");
    //Cat Finishing Move's
    RIP                 = ai->getSpellId("rip");             //30
    FEROCIOUS_BITE      = ai->getSpellId("ferocious bite");  //35
    MAIM                = ai->getSpellId("maim");            //35
	//Bear/Dire Bear Attacks & Buffs
	FERAL_CHARGE_BEAR	= 16979; //ai->getSpellId("feral charge - bear");
	ENRAGE				= ai->getSpellId("enrage");
	BASH                = ai->getSpellId("bash");            //10 rage
	MAUL                = ai->getSpellId("maul");            //15
	SWIPE_BEAR          = ai->getSpellId("swipe (bear)");           //20
	DEMORALIZING_ROAR   = ai->getSpellId("roar");            //10
	LACERATE			= ai->getSpellId("lacerate");
	MANGLE_BEAR			= ai->getSpellId("mangle (bear)");
	CHALLENGING_ROAR    = ai->getSpellId("challenging roar");//15
	GROWL               = ai->getSpellId("growl");

	FAERIE_FIRE_FERAL	= ai->getSpellId("faerie fire (feral)");
}

PlayerbotDruidAI::~PlayerbotDruidAI() {}

bool PlayerbotDruidAI::DoFirstCombatManeuver(Unit *pTarget)
{
	PlayerbotAI* ai = GetAI();
	Player *m_bot = GetPlayerBot();
	PlayerbotAI::CombatOrderType co = ai->GetCombatOrder();
	float targetDist = m_bot->GetDistance(pTarget);

	if (co&PlayerbotAI::ORDERS_TANK)
	{
		if (DIRE_BEAR_FORM > 0 && !m_bot->HasAura(MOONKIN_FORM, 0) && !m_bot->HasAura(DIRE_BEAR_FORM, 0) && ai->CastSpell (DIRE_BEAR_FORM, *m_bot))
			return true;
		if (BEAR_FORM > 0 && !m_bot->HasAura(DIRE_BEAR_FORM, 0) && !m_bot->HasAura(BEAR_FORM, 0) && ai->CastSpell (BEAR_FORM, *m_bot))
			return true;
		if (ENRAGE > 0 && ai->GetRageAmount() < 15 && ai->CastSpell(ENRAGE, *m_bot))
			return true;
		if (FERAL_CHARGE_BEAR > 0 && targetDist>=8 && targetDist<=25 && ai->CastSpell(FERAL_CHARGE_BEAR, *pTarget))
		{
            pTarget->GetContactPoint( m_bot, x, y, z, 3.666666f );
			ChargeSuccess = true;
            return true;
        }
		if (ChargeSuccess)
		{
			m_bot->Relocate( x, y, z );
			return false;
		}
	}
	else if (co&PlayerbotAI::ORDERS_ASSIST)
	{
		if (MOONKIN_FORM > 0 && !m_bot->HasAura(MOONKIN_FORM, 0) && ai->CastSpell(MOONKIN_FORM, *m_bot))
			return false;
	}
	else if (co&PlayerbotAI::ORDERS_HEAL)
	{
		if (TREE_OF_LIFE > 0 && !m_bot->HasAura(TREE_OF_LIFE, 0) && ai->CastSpell(TREE_OF_LIFE, *m_bot))
			return false;
	}
	else
	{
		if (CAT_FORM > 0 && !m_bot->HasAura(CAT_FORM, 0) && ai->CastSpell(CAT_FORM))
			return true;
		if (PROWL > 0 && !m_bot->HasAura(PROWL, 0) && ai->CastSpell(PROWL, *m_bot))
			return false;
	}
	return false;
}

void PlayerbotDruidAI::HealTarget(Unit &target, uint8 hp)
{
    PlayerbotAI* ai = GetAI();
    Player *m_bot = GetPlayerBot();

    if (hp < 70 && REJUVENATION > 0 && !target.HasAura(REJUVENATION, 0) && !target.HasAura(REGROWTH, 0) && ai->GetManaPercent() >=21 && ai->CastSpell(REJUVENATION, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting rejuvenation.");
	}

	if (hp < 60 && LIFEBLOOM > 0 && !target.HasAura(LIFEBLOOM, 0) && ai->GetManaPercent() >= 28 && ai->CastSpell(LIFEBLOOM, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting lifebloom.");
	}

    if (hp < 55 && REGROWTH > 0 && !target.HasAura(REGROWTH, 0) && !target.HasAura(REJUVENATION, 0) && ai->GetManaPercent() >= 33 && ai->CastSpell(REGROWTH, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting regrowth.");
	}

	if (hp < 50 && SWIFTMEND > 0 && target.HasAura(REJUVENATION, 0) || target.HasAura(REGROWTH, 0) && ai->GetManaPercent() >= 16 && ai->CastSpell(SWIFTMEND, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting swiftmend.");
	}

    if (hp < 45 && WILD_GROWTH > 0 && !target.HasAura(WILD_GROWTH, 0) && ai->GetManaPercent() >= 26 && ai->CastSpell(WILD_GROWTH, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting wild growth.");
	}

	if (hp < 30 && NOURISH > 0 && ai->GetManaPercent() >= 18 && ai->CastSpell(NOURISH, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting nourish.");
	}

	if (hp < 25 && HEALING_TOUCH > 0 && ai->GetManaPercent() >= 38 && ai->CastSpell(HEALING_TOUCH, target))
	{
		if( ai->GetManager()->m_confDebugWhisper )
			ai->TellMaster("I'm casting healing touch.");
	}
} // end HealTarget

void PlayerbotDruidAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            ai->CastSpell(MOONFIRE);
            return;
    }

    uint32 masterHP = GetMaster()->GetHealth()*100 / GetMaster()->GetMaxHealth();

    ai->SetInFront( pTarget );
    Player *m_bot = GetPlayerBot();
    Unit* pVictim = pTarget->getVictim();
	PlayerbotAI::CombatOrderType co = ai->GetCombatOrder();
	Group *m_group = m_bot->GetGroup();

	m_bot->Attack(pTarget, true);

    if (co&PlayerbotAI::ORDERS_ASSIST) SpellSequence = DruidSpell;
    else if (co&PlayerbotAI::ORDERS_HEAL) SpellSequence = DruidHeal;
	else
	{
		if (m_bot->GetDistance(pTarget)>ATTACK_DISTANCE)
		{
			ai->DoCombatMovement();
			return;
		}
		if (co&PlayerbotAI::ORDERS_TANK) SpellSequence = DruidTank;
		else SpellSequence = DruidCombat;
	}

	std::ostringstream out;
    switch (SpellSequence)
    {
        case DruidTank:
			out << "Case Druidtank";
			if (FAERIE_FIRE_FERAL > 0 && !pTarget->HasAura(FAERIE_FIRE_FERAL, 0) && ai->CastSpell(FAERIE_FIRE_FERAL, *pTarget))
				out << "> Faerie Fire";
			else if (GROWL > 0 && pVictim != m_bot && !pTarget->HasAura(CHALLENGING_ROAR, 0) && !pTarget->HasAura(GROWL, 0) && ai->CastSpell(GROWL, *pTarget))
				out << "> Growl";
			else if (CHALLENGING_ROAR > 0 && pVictim != m_bot && !pTarget->HasAura(CHALLENGING_ROAR, 0) && !pTarget->HasAura(GROWL, 0) && ai->GetRageAmount()>=15)
				out << "> Challenging Roar";
			else if (DEMORALIZING_ROAR > 0 && !pTarget->HasAura(DEMORALIZING_ROAR, 0) && ai->GetRageAmount() >= 10 && ai->CastSpell(DEMORALIZING_ROAR, *pTarget))
				out << "> Demoralizing Roar";
			else if (MANGLE_BEAR > 0 && !pTarget->HasAura(MANGLE_BEAR, 0) && ai->GetRageAmount() >= 20 && ai->CastSpell(MANGLE_BEAR, *pTarget))
				out << "> Mangle (Bear)";
			else if (LACERATE > 0 && !pTarget->HasAura(LACERATE, 0) && ai->GetRageAmount() >= 15 && ai->CastSpell(LACERATE, *pTarget))
				out << "> Lacerate";
			else if (SWIPE_BEAR > 0 && ai->GetAttackerCount()>=2 && ai->GetRageAmount() >= 20 && ai->CastSpell(SWIPE_BEAR, *pTarget))
				out << "> Swipe (Bear)";
			else if (MAUL > 0 && ai->GetRageAmount() >= 15 && ai->CastSpell(MAUL, *pTarget))
				out << "> Maul";
            break;

        case DruidSpell:
			out << "Case DruidSpell";
			DruidSpellCombat++;
            if(m_bot->HasAura(CAT_FORM, 0))
			{
				m_bot->RemoveAurasDueToSpell(768);
				break;
			}
            if(m_bot->HasAura(BEAR_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(5487);
                break;
            }
			if(m_bot->HasAura(DIRE_BEAR_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(9634);
                break;
            }
			if(m_bot->HasAura(MOONKIN_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(24858);
                break;
            }
            if (FAERIE_FIRE > 0 && DruidSpellCombat < 2 && !pTarget->HasAura(FAERIE_FIRE, 0) && ai->GetManaPercent() >= 8 && ai->CastSpell(FAERIE_FIRE, *pTarget))
				out << "> Farie Fire";
            else if (MOONFIRE > 0 && DruidSpellCombat < 3 && (!pTarget->HasAura(MOONFIRE, 0)) && ai->GetManaPercent() >= 24 && ai->CastSpell(MOONFIRE, *pTarget))
				out << "> Moonfire";
			else if (ROOTS > 0 && DruidSpellCombat < 4 && (!pTarget->HasAura(ROOTS, 0)) && ai->GetManaPercent() >= 8 && ai->CastSpell(ROOTS, *pTarget))
				out << "> Roots";
			else if (HURRICANE > 0 && ai->GetAttackerCount()>=5 && DruidSpellCombat < 4 && ai->GetManaPercent() >= 91 && ai->CastSpell(HURRICANE, *pTarget))
            {
				out << "> Hurricane";
				ai->SetIgnoreUpdateTime(10);
            }
			else if (INNERVATE > 0 && ai->GetManaPercent() < 50 && DruidSpellCombat < 6 && !m_bot->HasAura(INNERVATE, 0) && ai->CastSpell(INNERVATE, *m_bot))
				out << "> Innervate";
            else if (WRATH > 0 && DruidSpellCombat < 7 && ai->GetManaPercent() >= 13 && ai->CastSpell(WRATH, *pTarget))
				out << "> Wrath";
            else if (INSECT_SWARM > 0 && m_bot->HasAura(MOONKIN_FORM, 0) && DruidSpellCombat < 8 && !pTarget->HasAura(INSECT_SWARM, 0) && ai->GetManaPercent() >= 9 && ai->CastSpell(INSECT_SWARM , *pTarget))
				out << "> Insect Swarm";
            else if (STARFIRE > 0 && m_bot->HasAura(MOONKIN_FORM, 0) && DruidSpellCombat < 9 && ai->GetManaPercent() >= 18 && ai->CastSpell(STARFIRE, *pTarget))
				out << "> Starfire";
			else if (FORCE_OF_NATURE > 0 && m_bot->HasAura(MOONKIN_FORM, 0) && DruidSpellCombat < 10 && ai->GetManaPercent() >= 12 && ai->CastSpell(FORCE_OF_NATURE))
				out << "> Force of Nature";
			else if (STARFALL > 0 && m_bot->HasAura(MOONKIN_FORM, 0) && ai->GetAttackerCount()>=3 && DruidSpellCombat < 11 && ai->GetManaPercent() >= 39 && ai->CastSpell(STARFALL, *pTarget))
				out << "> Starfall";
			else if (BARKSKIN > 0 && m_bot->HasAura(MOONKIN_FORM, 0) && ai->GetHealthPercent() < 50 && DruidSpellCombat < 12 && !m_bot->HasAura(BARKSKIN, 0) && ai->CastSpell(BARKSKIN, *m_bot))
				out << "> Barkskin";
			else if (MANA_REJUVENATION > 0 && ai->GetManaPercent() < 50 && DruidSpellCombat < 13 && !m_bot->HasAura(MANA_REJUVENATION, 0) && ai->CastSpell(MANA_REJUVENATION, *m_bot))
				out << "> Mana Rejuvenation";
            else
				DruidSpellCombat = 0;
            break;

        case DruidHeal:
			out << "Case DruidHeal";
            if(m_bot->HasAura(CAT_FORM, 0))
			{
				m_bot->RemoveAurasDueToSpell(768);
				break;
			}
			if(m_bot->HasAura(BEAR_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(5487);
                break;
            }
			if(m_bot->HasAura(DIRE_BEAR_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(9634);
                break;
            }
			if(m_bot->HasAura(MOONKIN_FORM, 0))
            {
                m_bot->RemoveAurasDueToSpell(24858);
                break;
            }
            if (ai->GetHealthPercent() < 70)
			{
				HealTarget(*m_bot, ai->GetHealthPercent());
				out << "Healing myself";
			}
            else if (masterHP < 70)
			{
				HealTarget (*GetMaster(), masterHP);
				out << "Healing master";
			}
			else if( m_group )
			{
				Group::MemberSlotList const& groupSlot = m_group->GetMemberSlots();
				for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
				{
					Player *m_groupMember = sObjectMgr.GetPlayer( itr->guid );
					if( !m_groupMember || !m_groupMember->isAlive() )
						continue;

					uint32 memberHP = m_groupMember->GetHealth()*100 / m_groupMember->GetMaxHealth();
					if( memberHP < 70 )
						HealTarget( *m_groupMember, memberHP );
				}
				out << "Healing group member";
			}
            break;

        case DruidCombat:
			out << "Case Druid Combat";
            if (MAIM > 0 && m_bot->GetComboPoints() >= 1 && pTarget->IsNonMeleeSpellCasted(true) && ai->CastSpell(MAIM, *pTarget))
			{
				out << "> Maim";
				break;
			}
			if (FAERIE_FIRE_FERAL > 0 && !pTarget->HasAura(FAERIE_FIRE_FERAL, 0) && ai->CastSpell(FAERIE_FIRE_FERAL, *pTarget))
			{
				out << "> Faerie Fire";
				break;
			}
            if (m_bot->GetComboPoints() == 5)
            {
                if (RIP > 0 && pTarget->getClass() == CLASS_ROGUE && ai->GetEnergyAmount() >= 30 && ai->CastSpell(RIP, *pTarget))
					out << "> Rogue Rip";
                else if (MAIM > 0 && pTarget->getClass() == CLASS_DRUID && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Druid Maim";
                else if (MAIM > 0 && pTarget->getClass() == CLASS_SHAMAN && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Shaman Maim";
                else if (MAIM > 0 && pTarget->getClass() == CLASS_WARLOCK && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Warlock Maim";
                else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_HUNTER && ai->GetEnergyAmount() >= 35 && ai->CastSpell(FEROCIOUS_BITE, *pTarget))
					out << "> Hunter Ferocious Bite";
                else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_WARRIOR && ai->GetEnergyAmount() >= 35 && ai->CastSpell(FEROCIOUS_BITE, *pTarget))
					out << "> Warrior Ferocious Bite";
                else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_PALADIN && ai->GetEnergyAmount() >= 35 && ai->CastSpell(FEROCIOUS_BITE, *pTarget))
					out << "> Paladin Ferocious Bite";
                else if (FEROCIOUS_BITE > 0 && pTarget->getClass() == CLASS_DEATH_KNIGHT && ai->GetEnergyAmount() >= 25 && ai->CastSpell(FEROCIOUS_BITE, *pTarget))
					out << "> Deathknight Ferocious Bite";
                else if (MAIM > 0 && pTarget->getClass() == CLASS_MAGE && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Mage Maim";
                else if (MAIM > 0 && pTarget->getClass() == CLASS_PRIEST && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Priest Maim";
                else if (MAIM > 0 && ai->GetEnergyAmount() >= 35 && ai->CastSpell(MAIM, *pTarget))
					out << "> Maim";
			}
			else if (SHRED > 0 && pTarget->isInBackInMap(m_bot,1) && ai->GetEnergyAmount() >= 60 && ai->CastSpell (SHRED, *pTarget))
				out << "> Shred";
			else if (RAKE > 0 && ai->GetEnergyAmount() >= 40 && ai->CastSpell (RAKE, *pTarget))
				out << "> Rake";
            else if (CLAW > 0 && ai->GetEnergyAmount() >= 45 && ai->CastSpell (CLAW, *pTarget))
				out << "> Claw";
            else if (MANGLE_CAT > 0 && ai->GetEnergyAmount() >= 45 && ai->CastSpell (MANGLE_CAT, *pTarget))
				out << "> Mangle";
            break;
    }
	if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );
	ChargeSuccess=false;
} // end DoNextCombatManeuver

void PlayerbotDruidAI::DoNonCombatActions()
{
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    if(m_bot->HasAura(CAT_FORM, 0))
        m_bot->RemoveAurasDueToSpell(768);

    if(m_bot->HasAura(BEAR_FORM, 0))
        m_bot->RemoveAurasDueToSpell(5487);

	if(m_bot->HasAura(DIRE_BEAR_FORM, 0))
        m_bot->RemoveAurasDueToSpell(9634);

	if(m_bot->HasAura(MOONKIN_FORM, 0))
        m_bot->RemoveAurasDueToSpell(24858);

	if(m_bot->HasAura(TREE_OF_LIFE, 0))
        m_bot->RemoveAurasDueToSpell(33891);

/*
    // mana myself with MANA_REJUVENATION (*moved to combat: interferes with drinking/eating)
	if (GetAI()->GetManaPercent() < 50 && MANA_REJUVENATION > 0 && !m_bot->HasAura(MANA_REJUVENATION, 0))
		GetAI()->CastSpell(MANA_REJUVENATION, *m_bot);
*/
    // buff myself with MARK_OF_THE_WILD
    if (MARK_OF_THE_WILD > 0 && !m_bot->HasAura(MARK_OF_THE_WILD, 0))
        GetAI()->CastSpell (MARK_OF_THE_WILD, *m_bot);

    // Thorns generates aggro for moonkin
    if (THORNS > 0 && !m_bot->HasAura(THORNS, 0))
        GetAI()->CastSpell (THORNS, *m_bot);
/*
    // buff master with THORNS
    if (THORNS > 0 && !GetMaster()->HasAura(THORNS,0))
        GetAI()->CastSpell (THORNS, *(GetMaster()));
*/
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
            if( !tPlayer || !tPlayer->isAlive() )
                continue;

             // buff and heal
             (!tPlayer->HasAura(MARK_OF_THE_WILD,0) && GetAI()->CastSpell (MARK_OF_THE_WILD, *tPlayer));
			 //(!tPlayer->HasAura(THORNS,0) && GetAI()->CastSpell (THORNS, *tPlayer));
             (HealTarget(*tPlayer, tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth()));
        }
    }
} // end DoNonCombatActions

void PlayerbotDruidAI::BuffPlayer(Player* target)
{
    GetAI()->CastSpell(MARK_OF_THE_WILD, *target);
}
