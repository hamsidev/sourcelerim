#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "char.h"
#include "char_manager.h"
#include "battle.h"
#include "item.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "vector.h"
#include "packet.h"
#include "pvp.h"
#include "profiler.h"
#include "guild.h"
#include "affect.h"
#include "unique_item.h"
#include "lua_incl.h"
#include "arena.h"
#include "castle.h"
#include "sectree.h"
#include "ani.h"
#include "locale_service.h"
#include "../../common/CommonDefines.h"
#include "log.h"

#define __DAMAGE_CARPANI__
#ifdef __DAMAGE_CARPANI__
const unsigned long long DAMAGE_CARPANI = 25;
#endif

#ifdef on
	int64_t battle_hit(LPCHARACTER ch, LPCHARACTER victim, int64_t & iRetDam);
#else
	int battle_hit(LPCHARACTER ch, LPCHARACTER victim, int & iRetDam);
#endif

bool battle_distance_valid_by_xy(long x, long y, long tx, long ty)
{
	long distance = DISTANCE_APPROX(x - tx, y - ty);

	if (distance > 170)
		return false;

	return true;
}

bool battle_distance_valid(LPCHARACTER ch, LPCHARACTER victim)
{
	return battle_distance_valid_by_xy(ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY());
}

bool timed_event_cancel(LPCHARACTER ch)
{
	if (ch->m_pkTimedEvent)
	{
		event_cancel(&ch->m_pkTimedEvent);
		return true;
	}

	return false;
}

#ifdef NEW_ICEDAMAGE_SYSTEM
bool battle_is_icedamage(LPCHARACTER pAttacker, LPCHARACTER pVictim)
{
	if (pAttacker && pAttacker->IsPC())
	{
		DWORD race = pAttacker->GetRaceNum();
		const DWORD tmp_dwNDRFlag = pVictim->GetNoDamageRaceFlag();
		if (tmp_dwNDRFlag &&
			(race < MAIN_RACE_MAX_NUM) &&
			(IS_SET(tmp_dwNDRFlag, 1<<race))
		)
		{
			return false;
		}
		const std::set<DWORD> & tmp_setNDAFlag = pVictim->GetNoDamageAffectFlag();
		if (tmp_setNDAFlag.size())
		{
			for (std::set<DWORD>::iterator it = tmp_setNDAFlag.begin(); it != tmp_setNDAFlag.end(); ++it)
			{
				if (!pAttacker->IsAffectFlag(*it))
				{
					return false;
				}
			}
		}
	}
	return true;
}
#endif

bool battle_is_attackable(LPCHARACTER ch, LPCHARACTER victim)
{
	if (victim->IsDead() || victim->IsObserverMode())//@hinata42
		return false;

	if (victim->GetRaceNum() >= 30000 && victim->GetRaceNum() <= 30100)//@hinata4
		return false;

	{
		SECTREE	*sectree = NULL;

		sectree	= ch->GetSectree();
		if (sectree && sectree->IsAttr(ch->GetX(), ch->GetY(), ATTR_BANPK))
			return false;

		sectree = victim->GetSectree();
		if (sectree && sectree->IsAttr(victim->GetX(), victim->GetY(), ATTR_BANPK))
			return false;
	}
#ifdef NEW_ICEDAMAGE_SYSTEM
	if (!battle_is_icedamage(ch, victim))
		return false;
#endif
	if (ch->IsStun() || ch->IsDead())
		return false;

	if (ch->IsPC() && victim->IsPC())
	{
		CGuild* g1 = ch->GetGuild();
		CGuild* g2 = victim->GetGuild();

		if (g1 && g2)
		{
			if (g1->UnderWar(g2->GetID()))
				return true;
		}
	}

	if (IS_CASTLE_MAP(ch->GetMapIndex()) && false==castle_can_attack(ch, victim))
			return false;

	if (CArenaManager::instance().CanAttack(ch, victim) == true)
		return true;

	return CPVPManager::instance().CanAttack(ch, victim);
}

#ifdef on
int64_t battle_melee_attack(LPCHARACTER ch, LPCHARACTER victim)
#else
int battle_melee_attack(LPCHARACTER ch, LPCHARACTER victim)
#endif
{
	if (test_server&&ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	if (!victim || ch == victim)
		return BATTLE_NONE;

	if (test_server&&ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	if (!battle_is_attackable(ch, victim))
		return BATTLE_NONE;

	if (test_server&&ch->IsPC())
		sys_log(0, "battle_melee_attack : [%s] attack to [%s]", ch->GetName(), victim->GetName());

	int distance = DISTANCE_APPROX(ch->GetX() - victim->GetX(), ch->GetY() - victim->GetY());

	if (!victim->IsBuilding())
	{
		int max = 300;

		if (false == ch->IsPC())
		{
			max = (int) (ch->GetMobAttackRange() * 1.15f);
		}
		else
		{
			if (false == victim->IsPC() && BATTLE_TYPE_MELEE == victim->GetMobBattleType())
				max = MAX(300, (int) (victim->GetMobAttackRange() * 1.15f));
		}

		if (distance > max)
		{
			if (test_server)
				sys_log(0, "VICTIM_FAR: %s distance: %d max: %d", ch->GetName(), distance, max);

			return BATTLE_NONE;
		}
	}

	if (timed_event_cancel(ch))
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("전투가 시작 되어 취소 되었습니다."));

	if (timed_event_cancel(victim))
		victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("전투가 시작 되어 취소 되었습니다."));

	ch->SetPosition(POS_FIGHTING);
	ch->SetVictim(victim);

	const PIXEL_POSITION & vpos = victim->GetXYZ();
	ch->SetRotationToXY(vpos.x, vpos.y);

#ifdef on
	int64_t dam;
	int64_t ret = battle_hit(ch, victim, dam);
#else
	int dam;
	int ret = battle_hit(ch, victim, dam);
#endif
	return (ret);
}

void battle_end_ex(LPCHARACTER ch)
{
	if (ch->IsPosition(POS_FIGHTING))
		ch->SetPosition(POS_STANDING);
}

void battle_end(LPCHARACTER ch)
{
	battle_end_ex(ch);
}

#ifdef on
// AG = Attack Grade
// AL = Attack Limit
int64_t CalcBattleDamage(int64_t iDam, int iAttackerLev, int iVictimLev)
{
	if (iDam < 3)
		iDam = number(1, 5); 

	//return CALCULATE_DAMAGE_LVDELTA(iAttackerLev, iVictimLev, iDam);
	return iDam;
}

int64_t CalcMagicDamageWithValue(int64_t iDam, LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	return CalcBattleDamage(iDam, pkAttacker->GetLevel(), pkVictim->GetLevel());
}

int64_t CalcMagicDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	int64_t iDam = 0;

	if (pkAttacker->IsNPC())
	{
		iDam = CalcMeleeDamage(pkAttacker, pkVictim, false, false);	
	}

	iDam += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);

	return CalcMagicDamageWithValue(iDam, pkAttacker, pkVictim);
}
#else
// AG = Attack Grade
// AL = Attack Limit
int CalcBattleDamage(int iDam, int iAttackerLev, int iVictimLev)
{
	if (iDam < 3)
		iDam = number(1, 5);

	//return CALCULATE_DAMAGE_LVDELTA(iAttackerLev, iVictimLev, iDam);
	return iDam;
}

int CalcMagicDamageWithValue(int iDam, LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	return CalcBattleDamage(iDam, pkAttacker->GetLevel(), pkVictim->GetLevel());
}

int CalcMagicDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	int iDam = 0;

	if (pkAttacker->IsNPC())
	{
		iDam = CalcMeleeDamage(pkAttacker, pkVictim, false, false);
	}

	iDam += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);

	return CalcMagicDamageWithValue(iDam, pkAttacker, pkVictim);
}
#endif

float CalcAttackRating(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, bool bIgnoreTargetRating)
{
	int iARSrc;
	int iERSrc;

	{
		int attacker_dx = pkAttacker->GetPolymorphPoint(POINT_DX);
		int attacker_lv = pkAttacker->GetLevel();

		int victim_dx = pkVictim->GetPolymorphPoint(POINT_DX);
		int victim_lv = pkAttacker->GetLevel();

		iARSrc = MIN(90, (attacker_dx * 4	+ attacker_lv * 2) / 6);
		iERSrc = MIN(90, (victim_dx	  * 4	+ victim_lv   * 2) / 6);
	}

	float fAR = ((float) iARSrc + 210.0f) / 300.0f;

	if (bIgnoreTargetRating)
		return fAR;

	float fER = ((float) (iERSrc * 2 + 5) / (iERSrc + 95)) * 3.0f / 10.0f;

	return fAR - fER;
}

int SumAttResBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int attBonusID, int resistBonusID)
{
	return pkAttacker->GetPoint(attBonusID) - pkVictim->GetPoint(resistBonusID);
}

int SumBreakBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int attBonusID, int resistBonusID)
{
	if (!pkVictim->IsPC())
		return 100 - pkVictim->GetPoint(resistBonusID);
	return 100 + pkAttacker->GetPoint(attBonusID) - pkVictim->GetPoint(resistBonusID);
}


#ifdef on
int64_t CalcDamBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int64_t iDam)
#else
int CalcDamBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int iDam)
#endif
{
	LPITEM pkWeapon = pkAttacker->GetWear(WEAR_WEAPON);
	if (pkWeapon)
	{
		switch (pkWeapon->GetSubType())
		{
			case WEAPON_SWORD:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_SWORD, POINT_RESIST_SWORD) / 100;
				break;

			case WEAPON_TWO_HANDED:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_TWOHAND, POINT_RESIST_TWOHAND) / 100;
				break;

			case WEAPON_DAGGER:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_DAGGER, POINT_RESIST_DAGGER) / 100;
				break;

			case WEAPON_BELL:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_BELL, POINT_RESIST_BELL) / 100;
				break;

			case WEAPON_FAN:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_FAN, POINT_RESIST_FAN) / 100;
				break;

			#ifdef ENABLE_WOLFMAN_CHARACTER
			case WEAPON_CLAW:
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_CLAW, POINT_RESIST_CLAW) / 100;
				#if defined(ENABLE_WOLFMAN_CHARACTER) && defined(USE_ITEM_CLAW_AS_DAGGER)
				iDam = iDam * SumBreakBonus(pkAttacker, pkVictim, POINT_ATTBONUS_DAGGER, POINT_RESIST_DAGGER) / 100;
				#endif
				break;
			#endif
		}
	}
	return iDam;
}

#ifdef on
int64_t CalcAttBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int64_t iAtk)
#else
int CalcAttBonus(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int iAtk)
#endif
{

#ifdef on
	int64_t monsterattack = 0;
#endif

	if (!pkVictim->IsPC())
		iAtk += pkAttacker->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_ATTACK_BONUS);

	if (!pkAttacker->IsPC())
	{
		int iReduceDamagePct = pkVictim->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_TRANSFER_DAMAGE);
		iAtk = iAtk * (100 + iReduceDamagePct) / 100;
	}

	if (pkAttacker->IsNPC() && pkVictim->IsPC())
	{
		iAtk = (iAtk * CHARACTER_MANAGER::instance().GetMobDamageRate(pkAttacker)) / 100;
	}

	if (pkVictim->IsNPC())
	{
		if (pkVictim->IsRaceFlag(RACE_FLAG_ANIMAL))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ANIMAL)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_UNDEAD))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_UNDEAD)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_DEVIL))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DEVIL)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_HUMAN))
			iAtk += (iAtk * SumAttResBonus(pkAttacker, pkVictim, POINT_ATTBONUS_HUMAN, POINT_RESIST_HUMAN)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ORC))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ORC)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_MILGYO))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MILGYO)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_INSECT))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_INSECT)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_FIRE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_FIRE)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ICE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ICE)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_DESERT))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_DESERT)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_TREE))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_TREE)) / 100;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_CZ))
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_CZ)) / 100;

#ifdef ENABLE_BONUS_STONE
		if (pkVictim->IsStone())
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_STONE)) / 100;
#endif

#ifdef ENABLE_BONUS_BOSS
		if (pkVictim->IsBoss())
			iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_BOSS)) / 100;
#endif

		iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_MONSTER)) / 100;
	}
	else if (pkVictim->IsPC())
	{
		iAtk += (iAtk * SumAttResBonus(pkAttacker, pkVictim, POINT_ATTBONUS_HUMAN, POINT_RESIST_HUMAN)) / 100;

		switch (pkVictim->GetJob())
		{
			case JOB_WARRIOR:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WARRIOR)) / 100;
				break;

			case JOB_ASSASSIN:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_ASSASSIN)) / 100;
				break;

			case JOB_SURA:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_SURA)) / 100;
				break;

			case JOB_SHAMAN:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_SHAMAN)) / 100;
				break;
#ifdef ENABLE_WOLFMAN_CHARACTER
			case JOB_WOLFMAN:
				iAtk += (iAtk * pkAttacker->GetPoint(POINT_ATTBONUS_WOLFMAN)) / 100;
				break;
#endif
		}
	}

	if (pkAttacker->IsPC())
	{
		switch (pkAttacker->GetJob())
		{
			case JOB_WARRIOR:
				iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_WARRIOR)) / 100;
				break;

			case JOB_ASSASSIN:
				iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_ASSASSIN)) / 100;
				break;

			case JOB_SURA:
				iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_SURA)) / 100;
				break;

			case JOB_SHAMAN:
				iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_SHAMAN)) / 100;
				break;

			#ifdef ENABLE_WOLFMAN_CHARACTER
			case JOB_WOLFMAN:
				iAtk -= (iAtk * pkVictim->GetPoint(POINT_RESIST_WOLFMAN)) / 100;
				break;
			#endif
		}
	}

	if (pkAttacker->IsNPC() && pkVictim->IsPC())
	{
		if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_ELEC))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_ELEC))		/ 10000;
		else if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_FIRE))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_FIRE))		/ 10000;
		else if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_ICE))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_ICE))		/ 10000;
		else if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_WIND))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_WIND))		/ 10000;
		else if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_EARTH))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_EARTH))	/ 10000;
		else if (pkAttacker->IsRaceFlag(RACE_FLAG_ATT_DARK))
			iAtk -= (iAtk * 30 * pkVictim->GetPoint(POINT_RESIST_DARK))		/ 10000;
	}

	if (pkAttacker->IsPC() && pkVictim->IsNPC())
	{
		if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_ELEC))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_ELECT))	/ 10000;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_FIRE))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_FIRE))	/ 10000;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_ICE))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_ICE))	/ 10000;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_WIND))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_WIND))	/ 10000;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_EARTH))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_EARTH))	/ 10000;
		else if (pkVictim->IsRaceFlag(RACE_FLAG_ATT_DARK))
			iAtk += (iAtk * 30 * pkAttacker->GetPoint(POINT_ENCHANT_DARK))	/ 10000;
	}

	return iAtk;
}

#ifdef on
void Item_GetDamage(LPITEM pkItem, int64_t* pdamMin, int64_t* pdamMax)
#else
void Item_GetDamage(LPITEM pkItem, int* pdamMin, int* pdamMax)
#endif
{
	*pdamMin = 0;
	*pdamMax = 1;

	if (!pkItem)
		return;

	switch (pkItem->GetType())
	{
		case ITEM_ROD:
		case ITEM_PICK:
			return;
	}

	if (pkItem->GetType() != ITEM_WEAPON)
		sys_err("Item_GetDamage - !ITEM_WEAPON vnum=%d, type=%d", pkItem->GetOriginalVnum(), pkItem->GetType());

	*pdamMin = pkItem->GetValue(3);
	*pdamMax = pkItem->GetValue(4);
}

#ifdef on
int64_t CalcMeleeDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, bool bIgnoreDefense, bool bIgnoreTargetRating)
#else
int CalcMeleeDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, bool bIgnoreDefense, bool bIgnoreTargetRating)
#endif
{
	LPITEM pWeapon = pkAttacker->GetWear(WEAR_WEAPON);
	bool bPolymorphed = pkAttacker->IsPolymorphed();

	if (pWeapon && !(bPolymorphed && !pkAttacker->IsPolyMaintainStat()))
	{
		if (pWeapon->GetType() != ITEM_WEAPON)
			return 0;

		switch (pWeapon->GetSubType())
		{
			case WEAPON_SWORD:
			case WEAPON_DAGGER:
			case WEAPON_TWO_HANDED:
			case WEAPON_BELL:
			case WEAPON_FAN:
			case WEAPON_MOUNT_SPEAR:
#ifdef ENABLE_WOLFMAN_CHARACTER
			case WEAPON_CLAW:
#endif
				break;

			case WEAPON_BOW:
				sys_err("CalcMeleeDamage should not handle bows (name: %s)", pkAttacker->GetName());
				return 0;

			default:
				return 0;
		}
	}

#ifdef on
	int64_t iDam = 0;
	float fAR = CalcAttackRating(pkAttacker, pkVictim, bIgnoreTargetRating);
	int64_t iDamMin = 0, iDamMax = 0;
#else
	int iDam = 0;
	float fAR = CalcAttackRating(pkAttacker, pkVictim, bIgnoreTargetRating);
	int iDamMin = 0, iDamMax = 0;
#endif

#ifdef on
	// TESTSERVER_SHOW_ATTACKINFO
	int64_t DEBUG_iDamCur = 0;
	int64_t DEBUG_iDamBonus = 0;
	// END_OF_TESTSERVER_SHOW_ATTACKINFO
#else
	// TESTSERVER_SHOW_ATTACKINFO
	int DEBUG_iDamCur = 0;
	int DEBUG_iDamBonus = 0;
	// END_OF_TESTSERVER_SHOW_ATTACKINFO
#endif

	if (bPolymorphed && !pkAttacker->IsPolyMaintainStat())
	{
		// MONKEY_ROD_ATTACK_BUG_FIX
		Item_GetDamage(pWeapon, &iDamMin, &iDamMax);
		// END_OF_MONKEY_ROD_ATTACK_BUG_FIX

		DWORD dwMobVnum = pkAttacker->GetPolymorphVnum();
		const CMob * pMob = CMobManager::instance().Get(dwMobVnum);

		if (pMob)
		{
			int iPower = pkAttacker->GetPolymorphPower();
			iDamMin += pMob->m_table.dwDamageRange[0] * iPower / 100;
			iDamMax += pMob->m_table.dwDamageRange[1] * iPower / 100;
		}
	}
	else if (pWeapon)
	{
		// MONKEY_ROD_ATTACK_BUG_FIX
		Item_GetDamage(pWeapon, &iDamMin, &iDamMax);
		// END_OF_MONKEY_ROD_ATTACK_BUG_FIX
	}
	else if (pkAttacker->IsNPC())
	{
		iDamMin = pkAttacker->GetMobDamageMin();
		iDamMax = pkAttacker->GetMobDamageMax();
	}

	iDam = number(iDamMin, iDamMax) * 2;

	// TESTSERVER_SHOW_ATTACKINFO
	DEBUG_iDamCur = iDam;
	// END_OF_TESTSERVER_SHOW_ATTACKINFO

#ifdef on
	int64_t iAtk = 0;
#else
	int iAtk = 0;
#endif

	// level must be ignored when multiply by fAR, so subtract it before calculation.
	iAtk = pkAttacker->GetPoint(POINT_ATT_GRADE) + iDam - (pkAttacker->GetLevel() * 2);
#ifdef on
	iAtk = (int64_t) (iAtk * fAR);
#else
	iAtk = (int) (iAtk * fAR);
#endif
	iAtk += pkAttacker->GetLevel() * 2; // and add again

	if (pWeapon)
	{
		iAtk += pWeapon->GetValue(5) * 2;

		// 2004.11.12.myevan.TESTSERVER_SHOW_ATTACKINFO
		DEBUG_iDamBonus = pWeapon->GetValue(5) * 2;
		///////////////////////////////////////////////
	}

	iAtk += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS); // party attacker role bonus
#ifdef on
	iAtk = (int64_t) (iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
#else
	iAtk = (int) (iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
#endif

	iAtk = CalcAttBonus(pkAttacker, pkVictim, iAtk);

	int iDef = 0;

	if (!bIgnoreDefense)
	{
		iDef = (pkVictim->GetPoint(POINT_DEF_GRADE) * (100 + pkVictim->GetPoint(POINT_DEF_BONUS)) / 100);

		if (!pkAttacker->IsPC())
			iDef += pkVictim->GetMarriageBonus(UNIQUE_ITEM_MARRIAGE_DEFENSE_BONUS);
	}

#ifdef on
	if (pkAttacker->IsNPC())
		iAtk = (int64_t) (iAtk * pkAttacker->GetMobDamageMultiply());
#else
	if (pkAttacker->IsNPC())
		iAtk = (int) (iAtk * pkAttacker->GetMobDamageMultiply());
#endif

	iDam = MAX(0, iAtk - iDef);

	if (test_server)
	{
		int DEBUG_iLV = pkAttacker->GetLevel()*2;
		int DEBUG_iST = int((pkAttacker->GetPoint(POINT_ATT_GRADE) - DEBUG_iLV) * fAR);
		int DEBUG_iPT = pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);
#ifdef on
		int64_t DEBUG_iWP = 0;
		int64_t DEBUG_iPureAtk = 0;
		int64_t DEBUG_iPureDam = 0;
#else
		int DEBUG_iWP = 0;
		int DEBUG_iPureAtk = 0;
		int DEBUG_iPureDam = 0;
#endif

		char szRB[32] = "";
		char szGradeAtkBonus[32] = "";

#ifdef on
		DEBUG_iWP = int64_t(DEBUG_iDamCur * fAR);
#else
		DEBUG_iWP = int(DEBUG_iDamCur * fAR);
#endif
		DEBUG_iPureAtk = DEBUG_iLV + DEBUG_iST + DEBUG_iWP+DEBUG_iDamBonus;
		DEBUG_iPureDam = iAtk - iDef;

		if (pkAttacker->IsNPC())
		{
			snprintf(szGradeAtkBonus, sizeof(szGradeAtkBonus), "=%d*%.1f", DEBUG_iPureAtk, pkAttacker->GetMobDamageMultiply());
#ifdef on
			DEBUG_iPureAtk = int64_t(DEBUG_iPureAtk * pkAttacker->GetMobDamageMultiply());
#else
			DEBUG_iPureAtk = int(DEBUG_iPureAtk * pkAttacker->GetMobDamageMultiply());
#endif
		}

		if (DEBUG_iDamBonus != 0)
			snprintf(szRB, sizeof(szRB), "+RB(%d)", DEBUG_iDamBonus);

		char szPT[32] = "";

		if (DEBUG_iPT != 0)
			snprintf(szPT, sizeof(szPT), ", PT=%d", DEBUG_iPT);

		char szUnknownAtk[32] = "";

		if (iAtk != DEBUG_iPureAtk)
			snprintf(szUnknownAtk, sizeof(szUnknownAtk), "+?(%d)", iAtk-DEBUG_iPureAtk);

		char szUnknownDam[32] = "";

		if (iDam != DEBUG_iPureDam)
			snprintf(szUnknownDam, sizeof(szUnknownDam), "+?(%d)", iDam-DEBUG_iPureDam);

		char szMeleeAttack[128];

		snprintf(szMeleeAttack, sizeof(szMeleeAttack),
				"%s(%d)-%s(%d)=%d%s, ATK=LV(%d)+ST(%d)+WP(%d)%s%s%s, AR=%.3g%s",
				pkAttacker->GetName(),
				iAtk,
				pkVictim->GetName(),
				iDef,
				iDam,
				szUnknownDam,
				DEBUG_iLV,
				DEBUG_iST,
				DEBUG_iWP,
				szRB,
				szUnknownAtk,
				szGradeAtkBonus,
				fAR,
				szPT);

		pkAttacker->ChatPacket(CHAT_TYPE_TALKING, "%s", szMeleeAttack);
		pkVictim->ChatPacket(CHAT_TYPE_TALKING, "%s", szMeleeAttack);
	}

	return CalcBattleDamage(iDam, pkAttacker->GetLevel(), pkVictim->GetLevel());
}

#ifdef on
int64_t CalcArrowDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, LPITEM pkBow, LPITEM pkArrow, bool bIgnoreDefense)
#else
int CalcArrowDamage(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, LPITEM pkBow, LPITEM pkArrow, bool bIgnoreDefense)
#endif
{
	if (!pkBow || pkBow->GetType() != ITEM_WEAPON || pkBow->GetSubType() != WEAPON_BOW)
		return 0;

	if (!pkArrow)
		return 0;

	int iDist = (int) (DISTANCE_SQRT(pkAttacker->GetX() - pkVictim->GetX(), pkAttacker->GetY() - pkVictim->GetY()));
	//int iGap = (iDist / 100) - 5 - pkBow->GetValue(5) - pkAttacker->GetPoint(POINT_BOW_DISTANCE);
	int iGap = (iDist / 100) - 5 - pkAttacker->GetPoint(POINT_BOW_DISTANCE);
#ifdef on
	int64_t iPercent = 100 - (iGap * 5);
#else
	int iPercent = 100 - (iGap * 5);
#endif

#ifdef ENABLE_QUIVER_SYSTEM
	if (pkArrow->GetSubType() == WEAPON_QUIVER)
		iPercent = 100;
#endif

	if (iPercent <= 0)
		return 0;
	else if (iPercent > 100)
		iPercent = 100;

#ifdef on
	int64_t iDam = 0;
#else
	int iDam = 0;
#endif

	float fAR = CalcAttackRating(pkAttacker, pkVictim, false);
	iDam = number(pkBow->GetValue(3), pkBow->GetValue(4)) * 2 + pkArrow->GetValue(3);
#ifdef on
	int64_t iAtk;
#else
	int iAtk;
#endif

	// level must be ignored when multiply by fAR, so subtract it before calculation.
	iAtk = pkAttacker->GetPoint(POINT_ATT_GRADE) + iDam - (pkAttacker->GetLevel() * 2);
#ifdef on
		iAtk = (int64_t) (iAtk * fAR);
#else
		iAtk = (int) (iAtk * fAR);
#endif
	iAtk += pkAttacker->GetLevel() * 2; // and add again

	// Refine Grade
	iAtk += pkBow->GetValue(5) * 2;

	iAtk += pkAttacker->GetPoint(POINT_PARTY_ATTACKER_BONUS);
#ifdef on
	iAtk = (int64_t) (iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
#else
	iAtk = (int) (iAtk * (100 + (pkAttacker->GetPoint(POINT_ATT_BONUS) + pkAttacker->GetPoint(POINT_MELEE_MAGIC_ATT_BONUS_PER))) / 100);
#endif

	iAtk = CalcAttBonus(pkAttacker, pkVictim, iAtk);

	int iDef = 0;

	if (!bIgnoreDefense)
		iDef = (pkVictim->GetPoint(POINT_DEF_GRADE) * (100 + pkAttacker->GetPoint(POINT_DEF_BONUS)) / 100);

	if (pkAttacker->IsNPC())
#ifdef on
		iAtk = (int64_t) (iAtk * pkAttacker->GetMobDamageMultiply());
#else
		iAtk = (int) (iAtk * pkAttacker->GetMobDamageMultiply());
#endif

	iDam = MAX(0, iAtk - iDef);

#ifdef on
	int64_t iPureDam = iDam;
#else
	int iPureDam = iDam;
#endif

	iPureDam = (iPureDam * iPercent) / 100;

	if (test_server)
	{
		pkAttacker->ChatPacket(CHAT_TYPE_INFO, "ARROW %s -> %s, DAM %d DIST %d GAP %d %% %d",
				pkAttacker->GetName(),
				pkVictim->GetName(),
				iPureDam,
				iDist, iGap, iPercent);
	}

	return iPureDam;
	//return iDam;
}

void NormalAttackAffect(LPCHARACTER pkAttacker, LPCHARACTER pkVictim)
{
	if (pkAttacker->GetPoint(POINT_POISON_PCT) && !pkVictim->IsAffectFlag(AFF_POISON))
	{
		if (number(1, 100) <= pkAttacker->GetPoint(POINT_POISON_PCT))
			pkVictim->AttackedByPoison(pkAttacker);
	}
#ifdef ENABLE_WOLFMAN_CHARACTER
	if (pkAttacker->GetPoint(POINT_BLEEDING_PCT) && !pkVictim->IsAffectFlag(AFF_BLEEDING))
	{
		if (number(1, 100) <= pkAttacker->GetPoint(POINT_BLEEDING_PCT))
			pkVictim->AttackedByBleeding(pkAttacker);
	}
#endif
	int iStunDuration = 2;
	if (pkAttacker->IsPC() && !pkVictim->IsPC())
		iStunDuration = 4;

	AttackAffect(pkAttacker, pkVictim, POINT_STUN_PCT, IMMUNE_STUN,  AFFECT_STUN, POINT_NONE,        0, AFF_STUN, iStunDuration, "STUN");
	AttackAffect(pkAttacker, pkVictim, POINT_SLOW_PCT, IMMUNE_SLOW,  AFFECT_SLOW, POINT_MOV_SPEED, -30, AFF_SLOW, 20,		"SLOW");
}

#ifdef on
int64_t battle_hit(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int64_t & iRetDam)
#else
int battle_hit(LPCHARACTER pkAttacker, LPCHARACTER pkVictim, int & iRetDam)
#endif
{
	if (test_server)
		sys_log(0, "battle_hit : [%s] attack to [%s] : dam :%d type :%d", pkAttacker->GetName(), pkVictim->GetName(), iRetDam);

#ifdef on
	int64_t iDam = CalcMeleeDamage(pkAttacker, pkVictim);
#else
	int iDam = CalcMeleeDamage(pkAttacker, pkVictim);
#endif

	if (iDam <= 0)
		return (BATTLE_DAMAGE);

	NormalAttackAffect(pkAttacker, pkVictim);

	iDam = CalcDamBonus(pkAttacker, pkVictim, iDam);

	float attMul = pkAttacker->GetAttMul();
	float tempIDam = iDam;
	iDam = attMul * tempIDam + 0.5f;

	iRetDam = iDam;

	if (pkVictim->Damage(pkAttacker, iDam, DAMAGE_TYPE_NORMAL))
		return (BATTLE_DEAD);

	return (BATTLE_DAMAGE);
}

DWORD GET_ATTACK_SPEED(LPCHARACTER ch)
{
	if (NULL == ch)
		return 1000;

	LPITEM item = ch->GetWear(WEAR_WEAPON);
	DWORD default_bonus = SPEEDHACK_LIMIT_BONUS;
	DWORD riding_bonus = 0;

	if (ch->IsRiding())
	{
		riding_bonus = 50;
	}

	DWORD ani_speed = ani_attack_speed(ch);
	DWORD real_speed = (ani_speed * 100) / (default_bonus + ch->GetPoint(POINT_ATT_SPEED) + riding_bonus);

	if (item && item->GetSubType() == WEAPON_DAGGER)
		real_speed /= 2;
#ifdef ENABLE_WOLFMAN_CHARACTER
	else if (item && item->GetSubType() == WEAPON_CLAW)
		real_speed /= 2;
#endif

	return real_speed;
}

void SET_ATTACK_TIME(LPCHARACTER ch, LPCHARACTER victim, DWORD current_time)
{
	if (NULL == ch || NULL == victim)
		return;

	if (!ch->IsPC())
		return;

	ch->m_kAttackLog.dwVID = victim->GetVID();
	ch->m_kAttackLog.dwTime = current_time;
}

void SET_ATTACKED_TIME(LPCHARACTER ch, LPCHARACTER victim, DWORD current_time)
{
	if (NULL == ch || NULL == victim)
		return;

	if (!ch->IsPC())
		return;

	victim->m_AttackedLog.dwPID			= ch->GetPlayerID();
	victim->m_AttackedLog.dwAttackedTime= current_time;
}

bool IS_SPEED_HACK(LPCHARACTER ch, LPCHARACTER victim, DWORD current_time)
{
	if(!gHackCheckEnable) return false;

	if (ch->m_kAttackLog.dwVID == victim->GetVID())
	{
		if (current_time - ch->m_kAttackLog.dwTime < GET_ATTACK_SPEED(ch))
		{
			INCREASE_SPEED_HACK_COUNT(ch);

			if (test_server)
			{
				sys_log(0, "%s attack hack! time (delta, limit)=(%u, %u) hack_count %d",
						ch->GetName(),
						current_time - ch->m_kAttackLog.dwTime,
						GET_ATTACK_SPEED(ch),
						ch->m_speed_hack_count);

				ch->ChatPacket(CHAT_TYPE_INFO, "%s attack hack! time (delta, limit)=(%u, %u) hack_count %d",
						ch->GetName(),
						current_time - ch->m_kAttackLog.dwTime,
						GET_ATTACK_SPEED(ch),
						ch->m_speed_hack_count);
			}

			SET_ATTACK_TIME(ch, victim, current_time);
			SET_ATTACKED_TIME(ch, victim, current_time);
			return true;
		}
	}

	SET_ATTACK_TIME(ch, victim, current_time);

	if (victim->m_AttackedLog.dwPID == ch->GetPlayerID())
	{
		if (current_time - victim->m_AttackedLog.dwAttackedTime < GET_ATTACK_SPEED(ch))
		{
			INCREASE_SPEED_HACK_COUNT(ch);

			if (test_server)
			{
				sys_log(0, "%s Attack Speed HACK! time (delta, limit)=(%u, %u), hack_count = %d",
						ch->GetName(),
						current_time - victim->m_AttackedLog.dwAttackedTime,
						GET_ATTACK_SPEED(ch),
						ch->m_speed_hack_count);

				ch->ChatPacket(CHAT_TYPE_INFO, "Attack Speed Hack(%s), (delta, limit)=(%u, %u)",
						ch->GetName(),
						current_time - victim->m_AttackedLog.dwAttackedTime,
						GET_ATTACK_SPEED(ch));
			}

			SET_ATTACKED_TIME(ch, victim, current_time);
			return true;
		}
	}

	SET_ATTACKED_TIME(ch, victim, current_time);
	return false;
}
// vaffanculo a chi t'e morto martysama
