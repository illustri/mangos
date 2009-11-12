
#include "PlayerbotMageAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;

PlayerbotMageAI::PlayerbotMageAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    ARCANE_MISSILES         = ai->getSpellId("arcane missiles"); //ARCANE
    ARCANE_EXPLOSION        = ai->getSpellId("arcane explosion");
    COUNTERSPELL            = ai->getSpellId("counterspell");
    SLOW                    = ai->getSpellId("slow");
    ARCANE_BARRAGE          = ai->getSpellId("arcane barrage");
    ARCANE_BLAST            = ai->getSpellId("arcane blast");
	ARCANE_POWER            = ai->getSpellId("arcane power");
    DAMPEN_MAGIC            = ai->getSpellId("dampen magic");
	AMPLIFY_MAGIC           = ai->getSpellId("amplify magic");
	MAGE_ARMOR              = ai->getSpellId("mage armor");
	MIRROR_IMAGE            = ai->getSpellId("mirror image");
	ARCANE_INTELLECT        = ai->getSpellId("arcane intellect");
    ARCANE_BRILLIANCE       = ai->getSpellId("arcane brilliance");
    DALARAN_INTELLECT       = ai->getSpellId("dalaran intellect");
    DALARAN_BRILLIANCE      = ai->getSpellId("dalaran brilliance");
    MANA_SHIELD             = ai->getSpellId("mana shield");
    CONJURE_WATER           = ai->getSpellId("conjure water");
    CONJURE_FOOD            = ai->getSpellId("conjure food");
    FIREBALL                = ai->getSpellId("fireball"); //FIRE
    FIRE_BLAST              = ai->getSpellId("fire blast");
    FLAMESTRIKE             = ai->getSpellId("flamestrike");
    SCORCH                  = ai->getSpellId("scorch");
    PYROBLAST               = ai->getSpellId("pyroblast");
    BLAST_WAVE              = ai->getSpellId("blast wave");
    COMBUSTION              = ai->getSpellId("combustion");
    DRAGONS_BREATH          = ai->getSpellId("dragon's breath");
    LIVING_BOMB             = ai->getSpellId("living bomb");
	MOLTEN_ARMOR			= ai->getSpellId("molten armor");
    FROSTFIRE_BOLT          = ai->getSpellId("frostfire bolt");
	FIRE_WARD               = ai->getSpellId("fire ward");
    ICY_VEINS               = ai->getSpellId("icy veins"); //FROST
    DEEP_FREEZE             = ai->getSpellId("deep freeze");
    FROSTBOLT               = ai->getSpellId("frostbolt");
    FROST_NOVA              = ai->getSpellId("frost nova");
    BLIZZARD                = ai->getSpellId("blizzard");
    CONE_OF_COLD            = ai->getSpellId("cone of cold");
    ICE_BARRIER             = ai->getSpellId("ice barrier");
    SUMMON_WATER_ELEMENTAL  = ai->getSpellId("summon water elemental");
	FROST_WARD              = ai->getSpellId("frost ward");
    ICE_LANCE               = ai->getSpellId("ice lance");
    FROST_ARMOR             = ai->getSpellId("frost armor");
    ICE_ARMOR               = ai->getSpellId("ice armor");
	ICE_BLOCK               = ai->getSpellId("ice block");
	COLD_SNAP               = ai->getSpellId("cold snap");
}

PlayerbotMageAI::~PlayerbotMageAI() {}

bool PlayerbotMageAI::DoFirstCombatManeuver(Unit *pTarget)
{
	LastSpellFrost = LastSpellFire = LastSpellArcane = 0;
	return false;
}

void PlayerbotMageAI::DoNextCombatManeuver(Unit *pTarget)
{
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            if (FIREBALL > 0)
                ai->CastSpell(FIREBALL);
            return;
    }

    // ------- Non Duel combat ----------

	//ai->SetMovementOrder( PlayerbotAI::MOVEMENT_FOLLOW, GetMaster() ); // dont want to melee mob

    // Damage Spells (primitive example)
    ai->SetInFront( pTarget );
    Player *m_bot = GetPlayerBot();
	Unit* pVictim = pTarget->getVictim();
	float targetDist = m_bot->GetDistance(pTarget);

	m_bot->Attack(pTarget, true);

	if (targetDist>30.0)
	{
		ai->DoCombatMovement();
		return;
	}

	std::ostringstream out;
	switch (SpellSequence)
    {
        case SPELL_FROST:
			out << "Case Frost";
			if (FROSTBOLT > 0 && LastSpellFrost < 1 && ai->GetManaPercent() >= 16 && ai->CastSpell(FROSTBOLT, *pTarget))
			{
				out << " > Frost Bolt";
				LastSpellFrost++;
			}
			else if (ICE_LANCE > 0 && ai->GetManaPercent() >= 7 && LastSpellFrost < 2 && ai->CastSpell(ICE_LANCE, *pTarget))
			{
				out << " > Ice Lance";
				LastSpellFrost++;
			}
			else if (SUMMON_WATER_ELEMENTAL > 0 && LastSpellFrost < 3 && ai->GetManaPercent() >= 16 && ai->CastSpell(SUMMON_WATER_ELEMENTAL))
			{
				out << " > Water Elemental";
				LastSpellFrost++;
			}
			else if (COLD_SNAP > 0 && LastSpellFrost < 11 && LastSpellFrost < 12 && ai->CastSpell(COLD_SNAP, *m_bot))
				out << " > Cold Snap";
            else if (ICY_VEINS > 0 && !m_bot->HasAura(ICY_VEINS, 0) && ai->GetManaPercent() >= 3 && ai->CastSpell(ICY_VEINS, *m_bot))
				out << " > Icy Veins";
			else if (ICE_BARRIER > 0 && ai->GetHealthPercent() < 70 && ai->GetManaPercent() >= 30 && ai->CastSpell(ICE_BARRIER, *m_bot))
				out << " > Ice Barrior";
			else if (ICE_BLOCK > 0 && !m_bot->HasAura(ICE_BLOCK, 0) && ai->GetHealthPercent() < 25 && ai->CastSpell(ICE_BLOCK, *m_bot))
				out << "> Ice Block";
			else if (FROST_NOVA > 0 && !pTarget->HasAura(FROST_NOVA, 0) && targetDist<10 && ai->GetManaPercent() >= 10 && targetDist<10 && ai->CastSpell(FROST_NOVA, *pTarget))
				out << " > Frost Nova";
			else if (CONE_OF_COLD > 0 && !pTarget->HasAura(CONE_OF_COLD, 0) && ai->GetManaPercent() >= 35 && targetDist<10 && ai->CastSpell(CONE_OF_COLD, *pTarget))
				out << " > Cone of Cold";
            else if (BLIZZARD > 0 && ai->GetAttackerCount()>=5 && LastSpellFrost < 8 && ai->GetManaPercent() >= 89 && ai->CastSpell(BLIZZARD, *pTarget))
				out << " > Blizzard";
			else
				LastSpellFrost = 0;
			break;

        case SPELL_FIRE:
			out << "Case Fire";
			if (COMBUSTION > 0 && !m_bot->HasAura(COMBUSTION, 0) && LastSpellFire < 1 && ai->CastSpell(COMBUSTION, *m_bot))
			{
				out << " > Combustion";
				LastSpellFire++;
			}
			else if (BLAST_WAVE > 0 && ai->GetAttackerCount()>=3 && LastSpellFire < 2 && pVictim && ai->GetManaPercent() >= 34 && ai->CastSpell(BLAST_WAVE))
			{
				out << " > Blastwave";
				LastSpellFire++;
			}
            else if (DRAGONS_BREATH > 0 && pVictim && LastSpellFire < 3 && ai->GetManaPercent() >= 37 && targetDist<10 && ai->CastSpell(DRAGONS_BREATH, *pTarget))
			{
				out << " > Dragon's Breath";
				LastSpellFire++;
			}
			else if (FROSTFIRE_BOLT > 0 && !pTarget->HasAura(FROSTFIRE_BOLT, 0) && LastSpellFire < 4 && ai->GetManaPercent() >= 14 && ai->CastSpell(FROSTFIRE_BOLT, *pTarget))
			{
				out << " > Frostfire";
				LastSpellFire++;
			}
			else if (LIVING_BOMB > 0 && !pTarget->HasAura(LIVING_BOMB, 0) && LastSpellFire < 5 && ai->GetManaPercent() >= 27 && ai->CastSpell(LIVING_BOMB, *pTarget))
			{
				out << " > Living Bomb";
				LastSpellFire++;
			}
			else if (PYROBLAST > 0 && !pTarget->HasAura(PYROBLAST, 0) && LastSpellFire < 6 && ai->GetManaPercent() >= 27 && ai->CastSpell(PYROBLAST, *pTarget))
			{
				out << " > Pyroblast";
				LastSpellFire++;
			}
            else if (FIREBALL > 0 && ai->GetManaPercent() >= 23 && LastSpellFire < 7 && ai->CastSpell(FIREBALL, *pTarget))
			{
				out << " > Fireball";
				LastSpellFire++;
			}
            else if (FIRE_BLAST > 0 && ai->GetManaPercent() >= 25 && LastSpellFire < 8 && targetDist<20 && ai->CastSpell(FIRE_BLAST, *pTarget))
			{
				out << " > Fire Blast";
				LastSpellFire++;
			}
            else if (FLAMESTRIKE > 0 && ai->GetManaPercent() >= 35 && LastSpellFire < 9 && !pTarget->HasAura(FLAMESTRIKE, 0) && ai->CastSpell(FLAMESTRIKE, *pTarget))
			{
				out << " > Flamestrike";
				LastSpellFire++;
			}
            else if (SCORCH > 0 && ai->GetManaPercent() >= 10 && LastSpellFire < 10 && ai->CastSpell(SCORCH, *pTarget))
			{
				out << " > Scorch";
				LastSpellFire++;
			}
			else
				LastSpellFire = 0;
			break;

        case SPELL_ARCANE:
			out << "Case Arcane";
			if (COUNTERSPELL > 0 && pTarget->GetPower(POWER_MANA) > 0 && LastSpellArcane < 1 && ai->GetManaPercent() >= 9 && ai->CastSpell(COUNTERSPELL, *pTarget))
			{
				out << " > Counterspell";
				LastSpellArcane++;
			}
            else if (ARCANE_POWER > 0 && ai->GetManaPercent() >= 37 && LastSpellArcane < 2 && ai->CastSpell(ARCANE_POWER, *pTarget))
			{
				out << " > Arcane Power";
				LastSpellArcane++;
			}
			else if (ARCANE_BLAST > 0 && ai->GetManaPercent() >= 8 && LastSpellArcane < 3 && ai->CastSpell(ARCANE_BLAST, *pTarget))
			{
				out << " > Arcane Blast";
				LastSpellArcane++;
			}
			else if (ARCANE_MISSILES > 0 && ai->GetManaPercent() >= 37 && LastSpellArcane < 4 && ai->CastSpell(ARCANE_MISSILES, *pTarget))
			{
				out << " > Arcane Missiles";
				LastSpellArcane++;
			}
            else if (ARCANE_EXPLOSION > 0 && ai->GetAttackerCount()>=3 && LastSpellArcane < 5 && pVictim && ai->GetManaPercent() >= 27 && targetDist<10 && ai->CastSpell(ARCANE_EXPLOSION, *pTarget))
			{
				out << " > Arcane Explosion";
				LastSpellArcane++;
			}
            else if (SLOW > 0 && !pTarget->HasAura(SLOW, 0) && LastSpellArcane < 6 && ai->GetManaPercent() >= 12 && ai->CastSpell(SLOW, *pTarget))
			{
				out << " > Slow";
				LastSpellArcane++;
			}
            else if (ARCANE_BARRAGE > 0 && ai->GetManaPercent() >= 27 && LastSpellArcane < 7 && ai->CastSpell(ARCANE_BARRAGE, *pTarget))
			{
				out << " > Arcane Barrage";
				LastSpellArcane++;
			}
            else if (MIRROR_IMAGE > 0 && ai->GetManaPercent() >= 10 && LastSpellArcane < 8 && ai->CastSpell(MIRROR_IMAGE))
			{
				out << " > Mirror Image";
				LastSpellArcane++;
			}
			else if (MANA_SHIELD > 0 && ai->GetHealthPercent() < 50 && LastSpellArcane < 9 && !m_bot->HasAura(MANA_SHIELD, 0) && ai->GetManaPercent() >= 8 && ai->CastSpell(MANA_SHIELD, *m_bot))
			{
				out << " > Mana Shield";
				LastSpellArcane++;
			}
            else
				LastSpellArcane = 0;
			break;
    }
	if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );
} // end DoNextCombatManeuver

void PlayerbotMageAI::DoNonCombatActions()
{
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    SpellSequence = SPELL_FROST;
	PlayerbotAI* ai = GetAI();

    // buff myself
    if (DALARAN_INTELLECT > 0)
        (!m_bot->HasAura(DALARAN_INTELLECT, 0) && ai->GetManaPercent() >= 31 && ai->CastSpell (DALARAN_INTELLECT, *m_bot));
    else if (ARCANE_INTELLECT > 0)
        (!m_bot->HasAura(ARCANE_INTELLECT, 0) && !m_bot->HasAura(DALARAN_INTELLECT, 0) && ai->GetManaPercent() >= 37 && ai->CastSpell (ARCANE_INTELLECT, *m_bot));

	// buff master's group
    if (GetMaster()->GetGroup())
    {
        Group::MemberSlotList const& groupSlot = GetMaster()->GetGroup()->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *tPlayer = sObjectMgr.GetPlayer(uint64 (itr->guid));
            if( !tPlayer || !tPlayer->isAlive() )
                continue;
			// buff
			(!tPlayer->HasAura(ARCANE_INTELLECT,0) && !tPlayer->HasAura(DALARAN_INTELLECT,0) && ai->GetManaPercent() >= 37 && ai->CastSpell (ARCANE_INTELLECT, *tPlayer));
			(!tPlayer->HasAura(DALARAN_INTELLECT,0) && ai->GetManaPercent() >= 31 && ai->CastSpell (DALARAN_INTELLECT, *tPlayer));
        }
    }

	// hp & mana check
	if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
         m_bot->SetStandState(UNIT_STAND_STATE_STAND);

	if (GetAI()->GetManaPercent() < 60 || GetAI()->GetHealthPercent() < 60)
		GetAI()->Feast();

    // conjure food & water
    if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);

    Item* pItem = ai->FindDrink();

    if (pItem == NULL && CONJURE_WATER && ai->GetBaseManaPercent() >= 48)
    {
        ai->TellMaster("I'm conjuring some water.");
        ai->CastSpell(CONJURE_WATER, *m_bot);
		ai->SetIgnoreUpdateTime(3);
        return;
    }

    pItem = ai->FindFood();

    if (pItem == NULL && CONJURE_FOOD && ai->GetBaseManaPercent() >= 48)
    {
        ai->TellMaster("I'm conjuring some food.");
        ai->CastSpell(CONJURE_FOOD, *m_bot);
		ai->SetIgnoreUpdateTime(3);
    }

	if (m_bot->HasAura(MAGE_ARMOR, 0)) SpellSequence = SPELL_ARCANE;
	else if (m_bot->HasAura(FROST_ARMOR, 0) || m_bot->HasAura(ICE_ARMOR, 0)) SpellSequence = SPELL_FROST;
	else if (m_bot->HasAura(MOLTEN_ARMOR, 0)) SpellSequence = SPELL_FIRE;
	else ai->TellMaster("Tell me which armor to use by cast order");


} // end DoNonCombatActions

void PlayerbotMageAI::BuffPlayer(Player* target)
{
    GetAI()->CastSpell(ARCANE_INTELLECT, *target);
}
