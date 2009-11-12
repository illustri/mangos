// a simple DK class by rrtn :)

#include "PlayerbotDeathKnightAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;
PlayerbotDeathKnightAI::PlayerbotDeathKnightAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai) {

   PLAGUE_STRIKE     = ai->getSpellId("plague strike"); // Unholy
   DEATH_GRIP        = ai->getSpellId("death grip");
   DEATH_COIL        = ai->getSpellId("death coil");
   DEATH_STRIKE      = ai->getSpellId("death strike");
   UNHOLY_BLIGHT     = ai->getSpellId("unholy blight");
   SCOURGE_STRIKE    = ai->getSpellId("scourge strike");
   DEATH_AND_DECAY   = ai->getSpellId("death and decay");
   BONE_SHIELD		 = 49222; // buffs
   RAISE_DEAD        = ai->getSpellId("raise dead"); // pets (TODO: check for pets exist)
   SUMMON_GARGOYLE   = 49206;
   ARMY_OF_THE_DEAD  = 42650;
   ICY_TOUCH         = ai->getSpellId("icy touch"); // Frost
   OBLITERATE        = ai->getSpellId("obliterate");
   HOWLING_BLAST     = ai->getSpellId("howling blast");
   FROST_STRIKE      = ai->getSpellId("frost strike");
   CHAINS_OF_ICE     = ai->getSpellId("chains of ice");
   RUNE_STRIKE       = ai->getSpellId("rune strike");
   ICY_CLUTCH        = ai->getSpellId("icy clutch");
   KILLING_MACHINE   = ai->getSpellId("killing machine"); // buffs
   DEATHCHILL        = ai->getSpellId("deathchill");
   HORN_OF_WINTER    = ai->getSpellId("horn of winter");
   ICEBOUND_FORTITUDE= ai->getSpellId("icebound fortitude");
   BLOOD_STRIKE      = ai->getSpellId("blood strike"); // Blood
   PESTILENCE        = ai->getSpellId("pestilence");
   STRANGULATE       = ai->getSpellId("strangulate");
   BLOOD_BOIL        = ai->getSpellId("blood boil");
   HEART_STRIKE      = ai->getSpellId("heart strike");
   MARK_OF_BLOOD     = ai->getSpellId("mark of blood"); // buffs
   RUNE_TAP			 = ai->getSpellId("rune tap");
   VAMPIRIC_BLOOD    = ai->getSpellId("vampiric blood");
   DEATH_PACT        = ai->getSpellId("death pact");
   DEATH_RUNE_MASTERY= ai->getSpellId("death rune mastery");
   UNHOLY_PRESENCE   = 48265; // presence (TODO: better spell == presence)
   FROST_PRESENCE    = 48263;
   BLOOD_PRESENCE    = 48266;
}

PlayerbotDeathKnightAI::~PlayerbotDeathKnightAI() {}

bool PlayerbotDeathKnightAI::DoFirstCombatManeuver(Unit *pTarget)
{
	PlayerbotAI* ai = GetAI();
	Player *m_bot = GetPlayerBot();

	LastSpellUnholyDK = LastSpellFrostDK = LastSpellBloodDK = 0;

	if (HORN_OF_WINTER> 0 && !m_bot->HasAura(HORN_OF_WINTER,0) && ai->CastSpell(HORN_OF_WINTER, *m_bot))
	{
		if( ai->GetManager()->m_confDebugWhisper )
            ai->TellMaster("First > Horn of Winter");
		return true;
	}
	if (BONE_SHIELD > 0 && !m_bot->HasAura(BONE_SHIELD, 0) && ai->CastSpell (BONE_SHIELD, *m_bot))
	{
		if( ai->GetManager()->m_confDebugWhisper )
            ai->TellMaster("First > Bone Shield");
		return true;
	}
	if (MARK_OF_BLOOD > 0 && !pTarget->HasAura(MARK_OF_BLOOD, 0) && ai->CastSpell(MARK_OF_BLOOD, *pTarget))
	{
		if( ai->GetManager()->m_confDebugWhisper )
            ai->TellMaster("First > Mark of Blood");
		return true;
	}
	return false;
}

void PlayerbotDeathKnightAI::DoNextCombatManeuver(Unit *pTarget)
{
   PlayerbotAI* ai = GetAI();
   if (!ai)
       return;

   switch (ai->GetScenarioType())
   {
       case PlayerbotAI::SCENARIO_DUEL:
           ai->CastSpell(PLAGUE_STRIKE);
           return;
   }

   // ------- Non Duel combat ----------

   //ai->SetMovementOrder( PlayerbotAI::MOVEMENT_FOLLOW, GetMaster() ); // dont want to melee mob

   // DK Attcks: Unholy, Frost & Blood

   // damage spells
   ai->SetInFront( pTarget );//<---
   Player *m_bot = GetPlayerBot();
   Unit* pVictim = pTarget->getVictim();
   m_bot->Attack(pTarget, true);

   if (m_bot->GetDistance(pTarget)>ATTACK_DISTANCE)
   {
	   ai->DoCombatMovement();
	   return;
   }

   std::ostringstream out;
   switch (SpellSequence)
   {
       case SPELL_DK_UNHOLY:
		   out << "Case Unholy";
		   if (ARMY_OF_THE_DEAD > 0 && ai->GetAttackerCount()>=5 && LastSpellUnholyDK < 1 && ai->CastSpell(ARMY_OF_THE_DEAD))
           {
			   ai->SetIgnoreUpdateTime(7);
               out << "> Army of the Dead";
			   LastSpellUnholyDK++;
           }
           else if (PLAGUE_STRIKE > 0 && !pTarget->HasAura(PLAGUE_STRIKE, 0) && LastSpellUnholyDK < 2 && ai->CastSpell(PLAGUE_STRIKE, *pTarget))
		   {
			   out << "> Plague Strike";
			   LastSpellUnholyDK++;
		   }
		   else if (DEATH_GRIP > 0 && !pTarget->HasAura(DEATH_GRIP, 0) && LastSpellUnholyDK < 3 && ai->CastSpell(DEATH_GRIP, *pTarget))
		   {
			   out << "> Death Grip";
			   LastSpellUnholyDK++;
		   }
           else if (DEATH_COIL > 0 && LastSpellUnholyDK < 4 && ai->GetRunicPower() >= 40 && ai->CastSpell(DEATH_COIL, *pTarget))
		   {
			   out << "> Death Coil";
			   LastSpellUnholyDK++;
		   }
           else if (DEATH_STRIKE > 0 && !pTarget->HasAura(DEATH_STRIKE, 0) && LastSpellUnholyDK < 5 && ai->CastSpell(DEATH_STRIKE, *pTarget))
		   {
			   out << "> Death Strike";
			   LastSpellUnholyDK++;
		   }
           else if (UNHOLY_BLIGHT > 0 && !m_bot->HasAura(UNHOLY_BLIGHT, 0) && !pTarget->HasAura(UNHOLY_BLIGHT, 0) && LastSpellUnholyDK < 6 && ai->GetRunicPower() >= 40 && ai->CastSpell(UNHOLY_BLIGHT))
		   {
			   out << "> Unholy Blight";
			   LastSpellUnholyDK++;
		   }
           else if (SCOURGE_STRIKE > 0 && LastSpellUnholyDK < 7 && ai->CastSpell(SCOURGE_STRIKE, *pTarget))
		   {
			   out << "> Scourge Strike";
			   LastSpellUnholyDK++;
		   }
		   else if (DEATH_AND_DECAY > 0 && ai->GetAttackerCount()>=3 && !pTarget->HasAura(DEATH_AND_DECAY, 0) && LastSpellUnholyDK < 8 && ai->CastSpell(DEATH_AND_DECAY))
           {
			   ai->SetIgnoreUpdateTime(1);
               out << "> Death and Decay";
			   LastSpellUnholyDK++;
           }
		   else if (SUMMON_GARGOYLE > 0 && !m_bot->HasAura(ARMY_OF_THE_DEAD, 0) && LastSpellUnholyDK < 9 && ai->GetRunicPower() >= 50 && ai->CastSpell(SUMMON_GARGOYLE))
           {
			   ai->SetIgnoreUpdateTime(2);
               out << "> Summon Gargoyle";
			   LastSpellUnholyDK++;
           }
           else if (RAISE_DEAD > 0 && !m_bot->HasAura(ARMY_OF_THE_DEAD, 0) && LastSpellUnholyDK < 10 && ai->CastSpell(RAISE_DEAD))
		   {
			   out << "> Raise Dead";
			   LastSpellUnholyDK++;
		   }
           else
			   LastSpellUnholyDK = 0;
		   break;

       case SPELL_DK_FROST:
		   out << "Case Frost";
		   if (DEATHCHILL > 0 && !m_bot->HasAura(DEATHCHILL, 0) && !m_bot->HasAura(KILLING_MACHINE, 0) && ai->CastSpell (DEATHCHILL, *m_bot))
		   {
			   out << "> Deathchill";
			   break;
		   }
		   if (KILLING_MACHINE > 0 && !m_bot->HasAura(KILLING_MACHINE, 0) && !m_bot->HasAura(DEATHCHILL, 0) && ai->CastSpell (KILLING_MACHINE, *m_bot))
		   {
			   out << "> Killing Machine";
			   break;
		   }
		   if (ICY_TOUCH > 0 && !pTarget->HasAura(ICY_TOUCH, 0) && LastSpellFrostDK < 1 && ai->CastSpell(ICY_TOUCH, *pTarget))
		   {
			   out << "> Icy Touch";
			   LastSpellFrostDK++;
		   }
           else if (OBLITERATE > 0 && LastSpellFrostDK < 2 && ai->CastSpell(OBLITERATE, *pTarget))
		   {
			   out << "> Obliterate";
			   LastSpellFrostDK++;
		   }
           else if (FROST_STRIKE > 0 && LastSpellFrostDK < 3 && ai->GetRunicPower() >= 32 && ai->CastSpell(FROST_STRIKE, *pTarget))
		   {
			   out << "> Frost Strike";
			   LastSpellFrostDK++;
		   }
           else if (HOWLING_BLAST > 0 && ai->GetAttackerCount()>=3 && LastSpellFrostDK < 4 && ai->CastSpell(HOWLING_BLAST, *pTarget))
		   {
			   out << "> Howling Blast";
			   LastSpellFrostDK++;
		   }
		   else if (CHAINS_OF_ICE > 0 && !pTarget->HasAura(CHAINS_OF_ICE, 0) && LastSpellFrostDK < 5 && ai->CastSpell(CHAINS_OF_ICE, *pTarget))
		   {
			   out << "> Chains of Ice";
			   LastSpellFrostDK++;
		   }
           else if (RUNE_STRIKE > 0 && LastSpellFrostDK < 6 && ai->GetRunicPower() >= 20 && ai->CastSpell(RUNE_STRIKE, *pTarget))
		   {
			   out << "> Rune Strike";
			   LastSpellFrostDK++;
		   }
           else if (ICY_CLUTCH > 0 && !pTarget->HasAura(ICY_CLUTCH, 0) && LastSpellFrostDK < 7 && ai->CastSpell(ICY_CLUTCH, *pTarget))
		   {
			   out << "> Icy Clutch";
			   LastSpellFrostDK++;
		   }
		   else if (ICEBOUND_FORTITUDE > 0 && ai->GetHealthPercent() < 50 && !m_bot->HasAura(ICEBOUND_FORTITUDE, 0) && LastSpellFrostDK < 8 && ai->GetRunicPower() >= 20 && ai->CastSpell(ICEBOUND_FORTITUDE, *m_bot))
		   {
			   out << "> Icebound Fortitude";
			   LastSpellFrostDK++;
		   }
           else
			   LastSpellFrostDK = 0;
		   break;

       case SPELL_DK_BLOOD:
		   out << "Case Blood";
		   if (ICY_TOUCH > 0 && LastSpellBloodDK < 1 && !pTarget->HasAura(ICY_TOUCH, 0) && ai->CastSpell(ICY_TOUCH, *pTarget))
		   {
			   out << "> Icy Touch";
			   LastSpellBloodDK++;
		   }
		   else if (PLAGUE_STRIKE > 0 && LastSpellBloodDK < 2 && !pTarget->HasAura(PLAGUE_STRIKE, 0) && ai->CastSpell(PLAGUE_STRIKE, *pTarget))
		   {
			   LastSpellBloodDK++;
			   out << "> Plague Strike";
		   }
		   else if (HEART_STRIKE > 0 && LastSpellBloodDK < 4 && ai->CastSpell(HEART_STRIKE, *pTarget))
		   {
			   out << "> Heart Strike";
			   LastSpellBloodDK++;
		   }
		   else if (BLOOD_STRIKE > 0 && LastSpellBloodDK < 4 && ai->CastSpell(BLOOD_STRIKE, *pTarget))
		   {
			   out << "> Blood Strike";
			   LastSpellBloodDK++;
		   }
           else if (OBLITERATE > 0 && LastSpellBloodDK < 5 && ai->CastSpell(OBLITERATE, *pTarget))
		   {
			   out << "> Obliterate";
			   LastSpellBloodDK++;
		   }
		   else if (DEATH_COIL > 0 && LastSpellBloodDK < 6 && ai->GetRunicPower() >= 40 && ai->CastSpell(DEATH_COIL, *pTarget))
		   {
			   out << "> Death Coil";
			   LastSpellBloodDK++;
		   }
		   else
			   LastSpellBloodDK=0;
		   break;
   }
	if( ai->GetManager()->m_confDebugWhisper )
        ai->TellMaster( out.str().c_str() );

} // end DoNextCombatManeuver

void PlayerbotDeathKnightAI::DoNonCombatActions()
{
   Player * m_bot = GetPlayerBot();
   PlayerbotAI* ai = GetAI();

   if (!m_bot)
       return;

   // hp check
   if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
       m_bot->SetStandState(UNIT_STAND_STATE_STAND);

   if (GetAI()->GetHealthPercent() < 60)
	   GetAI()->Feast();

   if (m_bot->HasAura(UNHOLY_PRESENCE, 0)) SpellSequence = SPELL_DK_UNHOLY;
   else if (m_bot->HasAura(FROST_PRESENCE, 0)) SpellSequence = SPELL_DK_FROST;
   else if (m_bot->HasAura(BLOOD_PRESENCE, 0)) SpellSequence = SPELL_DK_BLOOD;
   else
	   ai->TellMaster("Tell me which presence use by cast order");
} // end DoNonCombatActions
