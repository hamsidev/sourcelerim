#include "stdafx.h"
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "guild.h"
#include "guild_manager.h"
#include "char_manager.h"
#include "questmanager.h"
#include "start_position.h"
#include "sectree_manager.h"
#include "ronark_map.h"

EVENTINFO(ronark_timer_info)
{
	bool isLogin;
	ronark_timer_info()  : isLogin(false) {}
};

EVENTFUNC(ronark_timer_event)
{
	auto* info = dynamic_cast<ronark_timer_info *>( event->info );
	if (info == nullptr) { return 0; }
	if (info->isLogin) {
		CRonarkMapManager::instance().StartEvent();
	}
	else {
		BroadcastNotice("Ronark Arenaya indi!");
		CRonarkMapManager::instance().SpawnRonark();
	}

	return 0;
}

/*** Unclassed Functions ***/
bool SortGuilds(CRonarkMapManager::TeamData i, CRonarkMapManager::TeamData j)
{
	return (i.iDamage > j.iDamage);
}
/*** End Of Unclassed Functions ***/

/*** Team Data Begin ***/
int CRonarkMapManager::STeamData::GetAccumulatedJoinerCount()
{
	return set_pidJoiner.size();
}

int CRonarkMapManager::STeamData::GetCurJointerCount()
{
	return iMemberCount;
}

void CRonarkMapManager::STeamData::AppendMember(LPCHARACTER ch)
{
	set_pidJoiner.insert(ch->GetPlayerID());
	++iMemberCount;
}

void CRonarkMapManager::STeamData::RemoveMember(LPCHARACTER ch)
{
	--iMemberCount;
}

void CRonarkMapManager::STeamData::AppendDamage(int damage)
{
	iDamage += damage;
}

void CRonarkMapManager::STeamData::Initialize()
{
	dwID = 0;
	pkGuild = NULL;
	iMemberCount = 0;
	iDamage = 0;
	set_pidJoiner.clear();
}
/*** Team Data End ***/

/*** Public Team Data Functions ***/
CRonarkMapManager::TeamData* CRonarkMapManager::GetTable(DWORD dwID)
{
	TeamDataMap::iterator itor = m_TeamDataMap.find(dwID);

	if (itor == m_TeamDataMap.end())
		return NULL;

	return itor->second;
}

DWORD CRonarkMapManager::GetGuildDamage(DWORD dwID)
{
	CRonarkMapManager::TeamData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iDamage;
}

DWORD CRonarkMapManager::GetGuildMemberCount(DWORD dwID)
{
	CRonarkMapManager::TeamData* c_pTable = GetTable(dwID);
	if (!c_pTable)
		return 0;

	return c_pTable->iMemberCount;
}
/*** End Of Public Team Data Functions ***/

/*** Public Append/Remove ***/
void CRonarkMapManager::IncMember(LPCHARACTER ch)
{
	if (!ch)
		return;

	if (!ch->IsPC())
		return;

	if (!ch->GetGuild())
		return;

	// AppendGuild
	TeamData* pCheck = GetTable(ch->GetGuild()->GetID());
	if (!pCheck)
	{
		AppendGuild(ch->GetGuild());
	}

	TeamData* pGuild = GetTable(ch->GetGuild()->GetID());

	if (pGuild)
	{
		pGuild->AppendMember(ch);
		sys_log(0, "CRonarkMapManager:: Ronark Append Member: %s %d", ch->GetName(), ch->GetGuild()->GetID());
	}

	m_set_pkChr.insert(ch);
	ch->SetPKMode(PK_MODE_GUILD);
}

void CRonarkMapManager::DecMember(LPCHARACTER ch)
{
	if (!ch)
		return;

	if (!ch->IsPC())
		return;

	if (!ch->GetGuild())
		return;

	TeamData* pGuild = GetTable(ch->GetGuild()->GetID());

	if (pGuild)
	{
		pGuild->RemoveMember(ch);
		sys_log(0, "CRonarkMapManager:: Ronark Remove Member: %s %d", ch->GetName(), ch->GetGuild()->GetID());
	}

	m_set_pkChr.erase(ch);
}

void CRonarkMapManager::AppendGuild(CGuild* guild)
{
	TeamData* pGuildData = new TeamData;
	pGuildData->dwID = guild->GetID();
	pGuildData->pkGuild = guild;
	pGuildData->iMemberCount = 0;
	pGuildData->iDamage = 0;
	m_TeamDataMap.insert(TeamDataMap::value_type(guild->GetID(), pGuildData));
	// we insert the list.
}

void CRonarkMapManager::OnDamage(CGuild* guild, int damage)
{
	TeamData* pGuild = GetTable(guild->GetID());

	if (pGuild)
	{
		pGuild->AppendDamage(damage);
	}

	SendScorePacket();
}
/*** End Of Public Append Remove ***/

/*** Callable Funcs ***/
DWORD CRonarkMapManager::GetWinnerGuild()
{
	std::vector<CRonarkMapManager::TeamData> sendLists;

	for (TeamDataMap::iterator itor = m_TeamDataMap.begin(); itor != m_TeamDataMap.end(); ++itor)
		sendLists.push_back(*itor->second);

	std::stable_sort(sendLists.begin(), sendLists.end(), SortGuilds);

	std::vector<CRonarkMapManager::TeamData>::iterator guild;
	int list = 0;
	for (guild = sendLists.begin(); guild != sendLists.end(); ++guild)
	{
		++list;
		DWORD dwID = guild->dwID;
		if (list == 1)
		{
			return dwID;
		}
	}

	return 0;
}
/*** End Of Callable Funcs ***/

/*** Status And Map ***/

void CRonarkMapManager::StartLoginEvent()
{
	BroadcastNotice("Ronark acildi! Girisler 20 saniye sonra kapatilacak!");
	auto* info = AllocEventInfo<ronark_timer_info>();
	info->isLogin = true;
	destroyEvent = event_create(ronark_timer_event, info, PASSES_PER_SEC(20));
}

void CRonarkMapManager::StartEvent()
{
	m_iActivateStatus = false;
	BroadcastNotice("Ronark girisleri kapatildi! 20 saniye sonra ronark spawn olacak!");
	auto* info = AllocEventInfo<ronark_timer_info>();
	info->isLogin = false;
	destroyEvent = event_create(ronark_timer_event, info, PASSES_PER_SEC(20));
}

void CRonarkMapManager::SpawnRonark() {
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_iRonarkMapIndex);
	if (!pMap)
		return;

	LPCHARACTER ronarkBoss = CHARACTER_MANAGER::instance().SpawnMob(RONARK_BOSS, m_iRonarkMapIndex, pMap->m_setting.iBaseX + 383 * 100, pMap->m_setting.iBaseY + 383 * 100, 0, false, -1, true);
	if (ronarkBoss)
	{
		m_iActivateStatus = RONARK_STATUS_OPENED;
		SetBossVID(ronarkBoss->GetVID());
		sys_log(0, "CRonarkMapManager:: Ronark Boss Summoned In The Map VID(%d) ", GetBossVID());
	}

	sys_log(0, "CRonarkMapManager:: Ronark has start.");
}

void CRonarkMapManager::SetRonarkStatus(int iStatus, int iMap)
{
	m_iRonarkMapIndex = iMap; // set mapindex
	m_iActivateStatus = iStatus; // set status

	if (iStatus == RONARK_STATUS_OPENED)
	{
		// LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_iRonarkMapIndex);
		// if (!pMap)
			// return;

		std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT ronark,id FROM player.guild"));

		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row;
			while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
			{
				int ronark = 0;
				int dwGuildID = 0;
				str_to_number(ronark, row[0]);
				str_to_number(dwGuildID, row[1]);
				if (ronark != 0)
				{
					CGuild* g = CGuildManager::instance().TouchGuild(dwGuildID);
					if (g)
					{
						g->RequestRonarkKing(false);
					}
				}
			}
		}
		StartLoginEvent();
	}
}

int CRonarkMapManager::GetRonarkStatus()
{
	return m_iActivateStatus;
}

bool CRonarkMapManager::IsRonarkMap(int mapindex)
{
	return m_iRonarkMapIndex == mapindex ? true : false;
}

bool CRonarkMapManager::IsRonarkActivate()
{
	return m_iActivateStatus == RONARK_STATUS_OPENED ? true : false;
}
/*** End Of Status And Map ***/

/*** Member Limit Count ***/
void CRonarkMapManager::SetMemberLimitCount(int memberlimit)
{
	m_iMemberLimitCount = memberlimit;
}

int CRonarkMapManager::GetMemberLimitCount()
{
	return m_iMemberLimitCount;
}
/*** End Of Member Limit Count ***/

/*** Packets Begin ***/
namespace
{
	struct FPacket
	{
		FPacket(const void* p, int size) : m_pvData(p), m_iSize(size)
		{
		}

		void operator () (LPCHARACTER ch)
		{
			ch->GetDesc()->Packet(m_pvData, m_iSize);
		}

		const void* m_pvData;
		int m_iSize;
	};

	struct FNotice
	{
		FNotice(const char* psz) : m_psz(psz)
		{
		}

		void operator() (LPCHARACTER ch)
		{
			ch->ChatPacket(CHAT_TYPE_NOTICE, "%s", m_psz);
		}

		const char* m_psz;
	};

	struct FGoToVillageWithoutWinner
	{
		void operator() (LPCHARACTER ch)
		{
			if (ch->GetGuild() && ch->GetGuild()->GetID() != CRonarkMapManager::instance().GetWinnerGuild())
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
	};

	struct FGoToVillage
	{
		void operator() (LPCHARACTER ch)
		{
			ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
		}
	};
};

void CRonarkMapManager::Notice(const char* psz)
{
	FNotice f(psz);
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CRonarkMapManager::Packet(const void* p, int size)
{
	FPacket f(p, size);
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CRonarkMapManager::SendHomeWithoutWinner()
{
	FGoToVillageWithoutWinner f;
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CRonarkMapManager::SendHome()
{
	FGoToVillage f;
	std::for_each(m_set_pkChr.begin(), m_set_pkChr.end(), f);
}

void CRonarkMapManager::SendScorePacket()
{
	if (!IsRonarkActivate()) // for char_manager
		return;

	std::vector<CRonarkMapManager::TeamData> sendLists;
	for (TeamDataMap::iterator itor = m_TeamDataMap.begin(); itor != m_TeamDataMap.end(); ++itor)
		sendLists.push_back(*itor->second);

	std::stable_sort(sendLists.begin(), sendLists.end(), SortGuilds);

	LPCHARACTER pkBoss = CHARACTER_MANAGER::instance().Find(GetBossVID());

	if (!pkBoss)
		return;

	// send
	std::vector<CRonarkMapManager::TeamData>::iterator guild;
	int list = 0;
	for (guild = sendLists.begin(); guild != sendLists.end(); ++guild)
	{
		DWORD dwID = guild->dwID;

		TPacketGCRonark p;
		p.header = HEADER_GC_RONARK;
		p.dwGuildID = dwID;
		p.dwDamage = MINMAX(0, (GetGuildDamage(dwID) * 100) / pkBoss->GetMaxHP(), 100);
		p.dwMemberCount = GetGuildMemberCount(dwID);
		p.dwLimit = GetMemberLimitCount();
		p.iList = list++;

		Packet(&p, sizeof(p));
	}
}
/*** End Of Packets Begin ***/

/*** Event Funcs ***/

EVENTINFO(ronark_info)
{
	DWORD map_index;

	ronark_info()
		: map_index(0)
	{
	}
};

EVENTFUNC(ronark_destroy_event)
{
	ronark_info* info = dynamic_cast<ronark_info*>(event->info);

	if (info == NULL)
	{
		sys_err("ronark_destroy_event> <Factor> NULL pointer");
		return 0;
	}

	CRonarkMapManager::instance().DelEvent();
	CRonarkMapManager::instance().SendHome();
	return 0;
}

void CRonarkMapManager::DelEvent()
{
	destroyEvent = NULL;
}
/*** End Of Event Funcs ***/

/*** Reward Funcs Begin ***/
void CRonarkMapManager::Reward()
{
	SendHomeWithoutWinner(); // send to home
	SetRonarkStatus(CRonarkMapManager::RONARK_STATUS_REWARD, m_iRonarkMapIndex);

	CGuild* g = CGuildManager::instance().TouchGuild(GetWinnerGuild());
	if (g)
	{
		// notice part
		char szNotice[512 + 1];
		snprintf(szNotice, sizeof(szNotice), LC_TEXT("RONARKNOTICE%s"), g->GetName());
		BroadcastNotice(szNotice);
		g->RequestRonarkKing(true);
	}

	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_iRonarkMapIndex);
	if (!pMap)
		return;

	LPCHARACTER pkReward = CHARACTER_MANAGER::instance().SpawnMob(RONARK_REWARD_MOB, m_iRonarkMapIndex, pMap->m_setting.iBaseX + 383 * 100, pMap->m_setting.iBaseY + 383 * 100, 0, false, -1, true);

	if (pkReward)
		SetRewardVID(pkReward->GetVID());

	// set event
	ronark_info* info = AllocEventInfo<ronark_info>();
	info->map_index = m_iRonarkMapIndex;
	destroyEvent = event_create(ronark_destroy_event, info, PASSES_PER_SEC(300));
}
/*** End Of Reward Funcs ***/

void CRonarkMapManager::ResetEvent()
{
	LPCHARACTER pkBoss = CHARACTER_MANAGER::instance().Find(GetBossVID());
	if (pkBoss)
		M2_DESTROY_CHARACTER(pkBoss);
	LPCHARACTER pkReward = CHARACTER_MANAGER::instance().Find(GetRewardVID());
	if (pkReward)
		M2_DESTROY_CHARACTER(pkReward);

	for (TeamDataMap::iterator itor = m_TeamDataMap.begin(); itor != m_TeamDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_TeamDataMap.clear();
	m_ibossVID = 0;
	m_iRonarkMapIndex = 0;
	m_iActivateStatus = 0;
	m_iMemberLimitCount = 500;
	CRonarkMapManager::SendHome(); // for lua
	event_cancel(&destroyEvent);
	m_set_pkChr.clear();
}

void CRonarkMapManager::Destroy()
{
	for (TeamDataMap::iterator itor = m_TeamDataMap.begin(); itor != m_TeamDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_TeamDataMap.clear();
	m_ibossVID = 0;
	m_iRonarkMapIndex = 0;
	m_iActivateStatus = 0;
	m_iMemberLimitCount = 0;
	destroyEvent = NULL;
	m_set_pkChr.clear();
}

CRonarkMapManager::CRonarkMapManager()
{
	ResetEvent();
	m_iMemberLimitCount = 500; // default 500
}

CRonarkMapManager::~CRonarkMapManager()
{
	Destroy();
}