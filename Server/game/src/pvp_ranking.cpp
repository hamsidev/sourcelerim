#include "stdafx.h"

#ifdef ENABLE_PVP_RANKING
#include "char.h"
#include "char_manager.h"
#include "pvp_ranking.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "p2p.h"
#include "questmanager.h"
#include "event.h"
#include "party.h"
#include "start_position.h"
#include "sectree_manager.h"

CPvPRanking::CPvPRanking()
	: m_pStatus (0),
	  m_pMaxKill (0),
	  m_pIsEnd (0),
	  m_pTimeEvent (0)
{
}

CPvPRanking::~CPvPRanking()
{
	Destroy();
}

void CPvPRanking::Destroy()
{
	m_map_pvpRank.clear();
	m_map_empireRank.clear();
	m_pStatus = 0;
	m_pIsEnd = 0;
	m_pTimeEvent = 0;
}

void CPvPRanking::AddKill (LPCHARACTER ch)
{
	if (!ch)
	{
		return;
	}

	if (m_pIsEnd == 1)
	{
		return;
	}

	TPvPRankMap::iterator it = m_map_pvpRank.find (ch->GetVID());
	#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
	TEmpireRankMap::iterator itEmpire = m_map_empireRank.find (ch->GetEmpire());
	if (itEmpire == m_map_empireRank.end())
	{
		m_map_empireRank.insert (TEmpireRankMap::value_type (ch->GetEmpire(), 1));
	}
	else
	{
		itEmpire->second += 1;
	}
	#endif
	if (it == m_map_pvpRank.end())
	{
		m_map_pvpRank.insert (TPvPRankMap::value_type (ch->GetVID(), TPvPInfo (1, 0)));
		ch->ChatPacket (CHAT_TYPE_NOTICE, LC_TEXT ("Tebrikler! Bir oyuncuyu oldurdunuz."));
		ch->SetRankKill (1);
	}
	else
	{
		it->second.iKill += 1;
		ch->SetRankKill (it->second.iKill);

		if (it->second.iKill == m_pMaxKill)
		{
			char buf[256];
			sprintf (buf, "%s isimli oyuncu %d oldurme sayisina ulasti.Tebrikler!", ch->GetName(), m_pMaxKill);
			BroadcastNotice (LC_TEXT (buf));
			m_pIsEnd = 1;
			//End();
		}

		ch->ChatPacket (CHAT_TYPE_NOTICE, LC_TEXT ("Tebrikler! Bir oyuncuyu oldurdunuz. Toplam Oldurme: %d - Toplam Olum: %d"), it->second.iKill, it->second.iDead);
	}
}

void CPvPRanking::AddDead (LPCHARACTER ch)
{
	if (!ch)
	{
		return;
	}
	if (m_pIsEnd == 1)
	{
		return;
	}
	TPvPRankMap::iterator it = m_map_pvpRank.find (ch->GetVID());

	if (it == m_map_pvpRank.end())
	{
		m_map_pvpRank.insert (TPvPRankMap::value_type (ch->GetVID(), TPvPInfo (0, 1)));
		ch->SetRankDead (1);
	}
	else
	{
		it->second.iDead += 1;
		ch->ChatPacket (CHAT_TYPE_NOTICE, LC_TEXT ("Toplam Oldurme: %d - Toplam Olum: %d"), it->second.iKill, it->second.iDead);
		ch->SetRankDead (it->second.iDead);
	}

}

void CPvPRanking::AddMember (LPCHARACTER ch)
{
	if (!ch)
	{
		return;
	}

	auto it = m_pvp_Member.find (ch->GetVID());

	if (it == m_pvp_Member.end())
	{
		m_pvp_Member.insert (ch->GetVID());
	}

	TPvPRankMap::iterator it2 = m_map_pvpRank.find (ch->GetVID());

	if (it2 == m_map_pvpRank.end())
	{
		if (ch->GetRankKill() > 0 || ch->GetRankDead() > 0)
		{
			m_map_pvpRank.insert (TPvPRankMap::value_type (ch->GetVID(), TPvPInfo (ch->GetRankKill(), ch->GetRankDead())));
		}
	}

	ch->SetPKMode (PK_MODE_BATTLE);

	LPPARTY pParty = ch->GetParty();

	if (pParty)
	{
		if (pParty->GetMemberCount() == 2)
		{
			CPartyManager::instance().DeleteParty (pParty);
		}
		else
		{
			pParty->Quit (ch->GetPlayerID());
		}
	}

	ch->ChatPacket (CHAT_TYPE_COMMAND, "pvp_empty_window");
}

void CPvPRanking::Check()
{
	if (m_map_pvpRank.size() == 0)
	{
		return;
	}

	if (m_pvp_Member.size() == 0)
	{
		return;
	}

	if (m_pTimeEvent == 1)
	{
		return;
	}

	using namespace std;

	priority_queue<pair<DWORD, TPvPRankInfo* >> map_pvpRanking;
	map_pvpRanking.empty();
	for (TPvPRankMap::iterator it = m_map_pvpRank.begin(); it != m_map_pvpRank.end(); ++it)
	{
		TPvPRankInfo* info = new TPvPRankInfo();
		info->iDead = it->second.iDead;
		info->VID = it->first;

		if (it->second.iKill > 0 || it->second.iDead > 0)
		{
			map_pvpRanking.push (std::make_pair (static_cast<DWORD> (it->second.iKill), info));
		}
	}
	TPacketGCPvPRanking sendPacket;
	memset (&sendPacket, 0, sizeof (sendPacket));
	sendPacket.bHeader = HEADER_GC_PVP_RANKING;
	BYTE sendCount = 0;	//maxPlayers
	BYTE endCount = 0;
	LPCHARACTER attackerCh = NULL;

	#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
	long long llEmpire1 = 0;
	long long llEmpire2 = 0;
	long long llEmpire3 = 0;


	TEmpireRankMap::iterator itEmpire1 = m_map_empireRank.find (1);
	TEmpireRankMap::iterator itEmpire2 = m_map_empireRank.find (2);
	TEmpireRankMap::iterator itEmpire3 = m_map_empireRank.find (3);
	if (itEmpire1 != m_map_empireRank.end())
	{
		llEmpire1 = itEmpire1->second;
	}
	if (itEmpire2 != m_map_empireRank.end())
	{
		llEmpire2 = itEmpire2->second;
	}
	if (itEmpire3 != m_map_empireRank.end())
	{
		llEmpire3 = itEmpire3->second;
	}
	#endif

	while (!map_pvpRanking.empty() && sendCount < 10)
	{
		if (attackerCh = CHARACTER_MANAGER::instance().Find (map_pvpRanking.top().second->VID))
		{
			if (attackerCh->IsPC())
			{
				const int chKill = map_pvpRanking.top().first;
				const int chDead = map_pvpRanking.top().second->iDead;
				strlcpy (sendPacket.szNames[sendCount], attackerCh->GetName(), sizeof (sendPacket.szNames[sendCount]));
				sendPacket.bJobs[sendCount] = attackerCh->GetRaceNum();
				sendPacket.bEmpire[sendCount] = attackerCh->GetEmpire();
				sendPacket.bLevels[sendCount] = attackerCh->GetLevel();
				sendPacket.iKill[sendCount] = chKill;
				sendPacket.iDead[sendCount] = chDead;
				sendPacket.bStatus[sendCount] = 1;
				sendPacket.iMaxKill = m_pMaxKill;
				#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
				sendPacket.bIsRankEmpire = IsRankEmpire();
				sendPacket.llEmpire1 = llEmpire1;
				sendPacket.llEmpire2 = llEmpire2;
				sendPacket.llEmpire3 = llEmpire3;
				#endif

				if (m_pIsEnd == 1)
				{
					char szInsertQuery[QUERY_MAX_LEN];
					snprintf (szInsertQuery, sizeof (szInsertQuery), "INSERT INTO player.pvp_ranking_top (name, value) VALUES ('%s', %d)", attackerCh->GetName(), chKill);
					std::unique_ptr<SQLMsg> pMsg (DBManager::instance().DirectQuery (szInsertQuery));
				}

				sendCount++;
				endCount++;
			}
		}
		map_pvpRanking.pop();
	}
	for (const auto& memberId : m_pvp_Member)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().Find (memberId);
		if (ch && ch->GetDesc())
		{
			ch->GetDesc()->Packet (&sendPacket, sizeof (sendPacket));
		}
	}

	if (endCount >= 2 && m_pIsEnd == 1)
	{
		m_pTimeEvent = 1;
		TimeEvent();
	}
}

bool CPvPRanking::CheckPlayerLevel (LPCHARACTER ch)
{
	if (!ch)
	{
		return false;
	}

	if (ch->GetLevel() > GetMaxLevel() || ch->GetLevel() < GetMinLevel())
	{
		return false;
	}

	return true;
}

void CPvPRanking::End()
{
	m_pStatus = 0;
	quest::CQuestManager::instance().RequestSetEventFlag ("pvp_ranking", 0);
	//Destroy();
	//BroadcastNotice(LC_TEXT("Savas Alani etkinligi sona erdi."));

	TPacketGCPvPRanking sendPacket;
	memset (&sendPacket, 0, sizeof (sendPacket));
	sendPacket.bHeader = HEADER_GC_PVP_RANKING;
	sendPacket.bStatus[0] = 0;

	for (const auto& memberId : m_pvp_Member)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().Find (memberId);
		if (ch && ch->GetDesc())
		{
			ch->GetDesc()->Packet (&sendPacket, sizeof (sendPacket));
		}
	}
	GoHome();
	Reset();

}

struct FWarpAllToVillage
{
	FWarpAllToVillage() {};
	void operator() (LPENTITY ent)
	{
		if (ent->IsType (ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->IsPC())
			{
				BYTE bEmpire =  ch->GetEmpire();
				if (bEmpire == 0)
				{
					sys_err ("Unkonwn Empire %s %d ", ch->GetName(), ch->GetPlayerID());
					return;
				}

				ch->WarpSet (g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
			}
		}
	}
};

EVENTINFO (warp_all_to_village_event_info)
{
	DWORD dwWarpMapIndex;

	warp_all_to_village_event_info()
		: dwWarpMapIndex (0)
	{
	}
};

EVENTFUNC (warp_all_to_village_event)
{
	warp_all_to_village_event_info * info = dynamic_cast<warp_all_to_village_event_info*> (event->info);

	if (info == NULL)
	{
		sys_err ("warp_all_to_village_event> <Factor> Null pointer");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap (info->dwWarpMapIndex);

	if (NULL != pSecMap)
	{
		FWarpAllToVillage f;
		pSecMap->for_each (f);
	}

	return 0;
}

void CPvPRanking::GoHome()
{
	warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

	info->dwWarpMapIndex = GetMapIndex();

	event_create (warp_all_to_village_event, info, PASSES_PER_SEC (10));

	SendNoticeMap (LC_TEXT ("10 saniye icinde herkes sehre isinlanacak."), GetMapIndex(), false);
}

////////////////////////////////////

EVENTINFO (warp_all_to_time_event_info)
{
	DWORD dwWarpMapIndex;

	warp_all_to_time_event_info()
		: dwWarpMapIndex (0)
	{
	}
};

EVENTFUNC (warp_all_to_time_event)
{
	warp_all_to_time_event_info * info = dynamic_cast<warp_all_to_time_event_info*> (event->info);

	if (info == NULL)
	{
		sys_err ("warp_all_to_time_event> <Factor> Null pointer");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap (info->dwWarpMapIndex);

	if (NULL != pSecMap)
	{
		CPvPRanking::Instance().End();
	}

	return 0;
}

void CPvPRanking::TimeEvent()
{
	warp_all_to_time_event_info* info = AllocEventInfo<warp_all_to_time_event_info>();

	info->dwWarpMapIndex = GetMapIndex();

	event_create (warp_all_to_time_event, info, PASSES_PER_SEC (30));

	SendNoticeMap (LC_TEXT ("30 saniye icinde savas alani temizlenecek."), GetMapIndex(), false);
}


////////////////////////////////////

struct FWarpAllToBase
{
	FWarpAllToBase() {};
	void operator() (LPENTITY ent)
	{
		if (ent->IsType (ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->IsPC())
			{
				ch->WarpSet (38300, 63900);
			}
		}
	}
};

EVENTINFO (warp_all_to_base_event_info)
{
	DWORD dwWarpMapIndex;

	warp_all_to_base_event_info()
		: dwWarpMapIndex (0)
	{
	}
};

EVENTFUNC (warp_all_to_base_event)
{
	warp_all_to_base_event_info * info = dynamic_cast<warp_all_to_base_event_info*> (event->info);

	if (info == NULL)
	{
		sys_err ("warp_all_to_base_event> <Factor> Null pointer");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap (info->dwWarpMapIndex);

	if (NULL != pSecMap)
	{
		FWarpAllToBase f;
		pSecMap->for_each (f);
	}

	return 0;
}

void CPvPRanking::WarpToBase()
{
	warp_all_to_base_event_info* info = AllocEventInfo<warp_all_to_base_event_info>();

	info->dwWarpMapIndex = GetMapIndex();

	event_create (warp_all_to_base_event, info, PASSES_PER_SEC (1));
}

void CPvPRanking::Reset()
{
	std::unique_ptr<SQLMsg> pMsg (DBManager::instance().DirectQuery ("UPDATE player.player SET rank_Kill = 0, rank_Dead = 0;"));
	if (pMsg->uiSQLErrno != 0)
	{
		return;
	}
}

void CPvPRanking::Start (BYTE minLevel, BYTE maxLevel, int maxKill, DWORD mapIndex)
{
	m_pStatus = 1;
	m_pMaxKill = maxKill;
	quest::CQuestManager::instance().RequestSetEventFlag ("pvp_ranking_minLevel", minLevel);
	quest::CQuestManager::instance().RequestSetEventFlag ("pvp_ranking_maxLevel", maxLevel);
	quest::CQuestManager::instance().RequestSetEventFlag ("pvp_ranking_maxKill", maxKill);
	quest::CQuestManager::instance().RequestSetEventFlag ("pvp_ranking_mapIndex", mapIndex);
}

BYTE CPvPRanking::GetStatus()
{
	return m_pStatus;
}

BYTE CPvPRanking::GetMaxLevel()
{
	BYTE maxLevel = quest::CQuestManager::instance().GetEventFlag ("pvp_ranking_maxLevel");
	return maxLevel;
}

BYTE CPvPRanking::GetMinLevel()
{
	BYTE minLevel = quest::CQuestManager::instance().GetEventFlag ("pvp_ranking_minLevel");
	return minLevel;
}

DWORD CPvPRanking::GetMapIndex()
{
	DWORD mapIndex = quest::CQuestManager::instance().GetEventFlag ("pvp_ranking_mapIndex");
	return mapIndex;
}

#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
BYTE CPvPRanking::IsRankEmpire()
{
	BYTE status = quest::CQuestManager::instance().GetEventFlag ("bayrak_savasi");
	return status;
}
#endif

#endif