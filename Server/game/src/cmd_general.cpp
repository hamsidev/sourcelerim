#include "stdafx.h"
#ifdef __FreeBSD__
#include <md5.h>
#else
#include "../../libthecore/include/xmd5.h"
#endif

#include "utils.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "item.h"
#include "arena.h"
#include "buffer_manager.h"
#include "unique_item.h"
#include "threeway_war.h"
#include "log.h"
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
#include "MountSystem.h"
#endif
#ifdef __PET_SYSTEM__
#include "PetSystem.h"
#endif
#ifdef ENABLE_PVP_RANKING
#include "pvp_ranking.h"
#endif
#include "../../common/CommonDefines.h"
#include "../../common/VnumHelper.h"

ACMD(do_user_horse_ride)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHorseRiding() == false)
	{
		if (ch->GetMountVnum())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 탈것을 이용중입니다."));
			return;
		}

		if (ch->GetHorse() == NULL)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 먼저 소환해주세요."));
			return;
		}

		ch->StartRiding();
	}
	else
	{
		ch->StopRiding();
	}
}

ACMD(do_user_horse_back)
{
	if (ch->GetHorse() != NULL)
	{
		ch->HorseSummon(false);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 돌려보냈습니다."));
	}
	else if (ch->IsHorseRiding() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말에서 먼저 내려야 합니다."));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 먼저 소환해주세요."));
	}
}

ACMD(do_user_horse_feed)
{
	if (ch->GetMyShop())
		return;

	if (ch->GetHorse() == NULL)
	{
		if (ch->IsHorseRiding() == false)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 먼저 소환해주세요."));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 탄 상태에서는 먹이를 줄 수 없습니다."));
		return;
	}

	DWORD dwFood = ch->GetHorseGrade() + 50054 - 1;

	auto foodTable = ITEM_MANAGER::instance().GetTable(dwFood);
	auto foodName = (foodTable) ? foodTable->szLocaleName : "NONAME"; // @fixme307

	if (ch->CountSpecifyItem(dwFood) > 0)
	{
		ch->RemoveSpecifyItem(dwFood, 1);
		ch->FeedHorse();
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말에게 %s%s 주었습니다."), foodName, "");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템이 필요합니다"), foodName);
	}
}

#define MAX_REASON_LEN		128

EVENTINFO(TimedEventInfo)
{
	DynamicCharacterPtr ch;
	int		subcmd;
	int         	left_second;
	char		szReason[MAX_REASON_LEN];

	TimedEventInfo()
	: ch()
	, subcmd( 0 )
	, left_second( 0 )
	{
		::memset( szReason, 0, MAX_REASON_LEN );
	}
};

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

EVENTINFO(shutdown_event_data)
{
	int seconds;

	shutdown_event_data()
	: seconds( 0 )
	{
	}
};

EVENTFUNC(shutdown_event)
{
	shutdown_event_data* info = dynamic_cast<shutdown_event_data*>( event->info );

	if ( info == NULL )
	{
		sys_err( "shutdown_event> <Factor> Null pointer" );
		return 0;
	}

	int * pSec = & (info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "shutdown_event sec %d", *pSec);

		if (--*pSec == -10)
		{
			const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--*pSec;
		return passes_per_sec;
	}
	else
	{
		char buf[64];
		snprintf(buf, sizeof(buf), LC_TEXT("셧다운이 %d초 남았습니다."), *pSec);
		SendNotice(buf);

		--*pSec;
		return passes_per_sec;
	}
}

void Shutdown(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

	char buf[64];
	snprintf(buf, sizeof(buf), LC_TEXT("%d초 후 게임이 셧다운 됩니다."), iSec);

	SendNotice(buf);

	shutdown_event_data* info = AllocEventInfo<shutdown_event_data>();
	info->seconds = iSec;

	event_create(shutdown_event, info, 1);
}

ACMD(do_shutdown)
{
	sys_err("Accept shutdown command from %s.", (ch) ? ch->GetName() : "NONAME");

	TPacketGGShutdown p;
	p.bHeader = HEADER_GG_SHUTDOWN;
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

	Shutdown(10);
}

EVENTFUNC(timed_event)
{
	TimedEventInfo * info = dynamic_cast<TimedEventInfo *>( event->info );

	if ( info == NULL )
	{
		sys_err( "timed_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEvent = NULL;

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
			case SCMD_QUIT:
			case SCMD_PHASE_SELECT:
				{
					TPacketNeedLoginLogInfo acc_info;
					acc_info.dwPlayerID = ch->GetDesc()->GetAccountTable().id;

					db_clientdesc->DBPacket( HEADER_GD_VALID_LOGOUT, 0, &acc_info, sizeof(acc_info) );

					LogManager::instance().DetailLoginLog( false, ch );
				}
				break;
		}

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
				if (d)
					d->SetPhase(PHASE_CLOSE);
				break;

			case SCMD_QUIT:
				ch->ChatPacket(CHAT_TYPE_COMMAND, "quit");
				if (d) // @fixme197
					d->DelayedDisconnect(1);
				break;

			case SCMD_PHASE_SELECT:
				ch->Disconnect("timed_event - SCMD_PHASE_SELECT");
				if (d)
					d->SetPhase(PHASE_SELECT);
				break;
		}

		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d초 남았습니다."), info->left_second);
		--info->left_second;
	}

	return PASSES_PER_SEC(1);
}

ACMD(do_cmd)
{
	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("취소 되었습니다."));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("로그인 화면으로 돌아 갑니다. 잠시만 기다리세요."));
			break;

		case SCMD_QUIT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("게임을 종료 합니다. 잠시만 기다리세요."));
			break;

		case SCMD_PHASE_SELECT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("캐릭터를 전환 합니다. 잠시만 기다리세요."));
			break;
	}

	int nExitLimitTime = 10;

	if (ch->IsHack(false, true, nExitLimitTime) &&
		false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()) &&
	   	(!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		return;
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
		case SCMD_QUIT:
		case SCMD_PHASE_SELECT:
			{
				TimedEventInfo* info = AllocEventInfo<TimedEventInfo>();

				{
					if (ch->IsPosition(POS_FIGHTING))
						info->left_second = 10;
					else
						info->left_second = 3;
				}

				info->ch		= ch;
				info->subcmd		= subcmd;
				strlcpy(info->szReason, argument, sizeof(info->szReason));

				ch->m_pkTimedEvent	= event_create(timed_event, info, 1);
			}
			break;
	}
}

ACMD(do_mount)
{
}

ACMD(do_fishing)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	ch->SetRotation(atof(arg1));
	ch->fishing();
}

ACMD(do_console)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");
}

ACMD(do_restart)
{
	if (false == ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
		ch->StartRecoveryEvent();
		return;
	}

	if (NULL == ch->m_pkDeadEvent)
		return;

	int iTimeToDead = (event_time(ch->m_pkDeadEvent) / passes_per_sec);
#ifdef ENABLE_PVP_RANKING
	if (ch->GetMapIndex() == CPvPRanking::instance().GetMapIndex() && quest::CQuestManager::instance().GetEventFlag ("pvp_ranking") > 0)
	{
		iTimeToDead = 0;
	}
#endif

	if (subcmd != SCMD_RESTART_TOWN && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		if (!test_server)
		{
			if (ch->IsHack())
			{
				if (false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 재시작 할 수 없습니다. (%d초 남음)"), iTimeToDead - (180 - g_nPortalLimitTime));
					return;
				}
			}
#define eFRS_HERESEC	170
			if (iTimeToDead > eFRS_HERESEC)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 재시작 할 수 없습니다. (%d초 남음)"), iTimeToDead - eFRS_HERESEC);
				return;
			}
		}
	}

	//PREVENT_HACK

	if (subcmd == SCMD_RESTART_TOWN)
	{
		if (ch->IsHack())
		{
			if ((!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG) ||
			   	false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 재시작 할 수 없습니다. (%d초 남음)"), iTimeToDead - (180 - g_nPortalLimitTime));
				return;
			}
		}

#define eFRS_TOWNSEC	173
		if (iTimeToDead > eFRS_TOWNSEC)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 마을에서 재시작 할 수 없습니다. (%d 초 남음)"), iTimeToDead - eFRS_TOWNSEC);
			return;
		}
	}
	//END_PREVENT_HACK

	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");

	ch->GetDesc()->SetPhase(PHASE_GAME);
	ch->SetPosition(POS_STANDING);
	ch->StartRecoveryEvent();

	//FORKED_LOAD

	if (1 == quest::CQuestManager::instance().GetEventFlag("threeway_war"))
	{
		if (subcmd == SCMD_RESTART_TOWN || subcmd == SCMD_RESTART_HERE)
		{
			if (true == CThreeWayWar::instance().IsThreeWayWarMapIndex(ch->GetMapIndex()) &&
					false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
#ifdef __PET_SYSTEM__
				ch->CheckPet();
#endif
				ch->ReviveInvisible(5);
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());

				return;
			}

			if (true == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				if (CThreeWayWar::instance().GetReviveTokenForPlayer(ch->GetPlayerID()) <= 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성지에서 부활 기회를 모두 잃었습니다! 마을로 이동합니다!"));
					ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
				}
				else
				{
					ch->Show(ch->GetMapIndex(), GetSungziStartX(ch->GetEmpire()), GetSungziStartY(ch->GetEmpire()));
				}

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
#ifdef __PET_SYSTEM__
				ch->CheckPet();
#endif
				ch->ReviveInvisible(5);

				return;
			}
		}
	}
	//END_FORKED_LOAD

	if (ch->GetDungeon())
		ch->GetDungeon()->UseRevive(ch);

	if (ch->GetWarMap() && !ch->IsObserverMode())
	{
		CWarMap * pMap = ch->GetWarMap();
		DWORD dwGuildOpponent = pMap ? pMap->GetGuildOpponent(ch) : 0;

		if (dwGuildOpponent)
		{
			switch (subcmd)
			{
				case SCMD_RESTART_TOWN:
					sys_log(0, "do_restart: restart town");
					PIXEL_POSITION pos;

					if (CWarMapManager::instance().GetStartPosition(ch->GetMapIndex(), ch->GetGuild()->GetID() < dwGuildOpponent ? 0 : 1, pos))
						ch->Show(ch->GetMapIndex(), pos.x, pos.y);
					else
						ch->ExitToSavedLocation();

					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
					ch->CheckMount();
#endif
#ifdef __PET_SYSTEM__
				ch->CheckPet();
#endif
					ch->ReviveInvisible(5);
					break;

				case SCMD_RESTART_HERE:
					sys_log(0, "do_restart: restart here");
					ch->RestartAtSamePos();
					//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
					ch->CheckMount();
#endif
#ifdef __PET_SYSTEM__
				ch->CheckPet();
#endif
					ch->ReviveInvisible(5);
					break;
			}

			return;
		}
	}
#ifdef ENABLE_PVP_RANKING
	if (ch->GetMapIndex() == CPvPRanking::instance().GetMapIndex() && quest::CQuestManager::instance().GetEventFlag ("pvp_ranking") > 0)
	{
		switch (subcmd)
		{
			case SCMD_RESTART_TOWN:
				sys_log (0, "do_restart: restart town");
				PIXEL_POSITION pos;

				if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire (ch->GetMapIndex(), ch->GetEmpire(), pos))
				{
					ch->WarpSet (pos.x, pos.y);
				}
				else
				{
					ch->WarpSet (EMPIRE_START_X (ch->GetEmpire()), EMPIRE_START_Y (ch->GetEmpire()));
				}

				ch->PointChange (POINT_HP, 50 - ch->GetHP());
				ch->DeathPenalty (1);
				break;
			case SCMD_RESTART_HERE:
				ch->RestartAtSamePos();
				ch->PointChange (POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange (POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible (5);
				break;
		}
		return;
	}
#endif
	switch (subcmd)
	{
		case SCMD_RESTART_TOWN:
			sys_log(0, "do_restart: restart town");
			PIXEL_POSITION pos;

			if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
				ch->WarpSet(pos.x, pos.y);
			else
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(1);
			break;

		case SCMD_RESTART_HERE:
			sys_log(0, "do_restart: restart here");
			ch->RestartAtSamePos();
			//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(0);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			ch->CheckMount();
#endif
#ifdef __PET_SYSTEM__
				ch->CheckPet();
#endif
			ch->ReviveInvisible(5);
			break;
	}
}

#define MAX_STAT g_iStatusPointSetMaxValue

ACMD(do_stat_reset)
{
	ch->PointChange(POINT_STAT_RESET_COUNT, 12 - ch->GetPoint(POINT_STAT_RESET_COUNT));
}

ACMD(do_stat_minus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 능력을 올릴 수 없습니다."));
		return;
	}

	if (ch->GetPoint(POINT_STAT_RESET_COUNT) <= 0)
		return;

	if (!strcmp(arg1, "st"))
	{
		if (ch->GetRealPoint(POINT_ST) <= JobInitialPoints[ch->GetJob()].st)
			return;

		ch->SetRealPoint(POINT_ST, ch->GetRealPoint(POINT_ST) - 1);
		ch->SetPoint(POINT_ST, ch->GetPoint(POINT_ST) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_ST, 0);
	}
	else if (!strcmp(arg1, "dx"))
	{
		if (ch->GetRealPoint(POINT_DX) <= JobInitialPoints[ch->GetJob()].dx)
			return;

		ch->SetRealPoint(POINT_DX, ch->GetRealPoint(POINT_DX) - 1);
		ch->SetPoint(POINT_DX, ch->GetPoint(POINT_DX) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_DX, 0);
	}
	else if (!strcmp(arg1, "ht"))
	{
		if (ch->GetRealPoint(POINT_HT) <= JobInitialPoints[ch->GetJob()].ht)
			return;

		ch->SetRealPoint(POINT_HT, ch->GetRealPoint(POINT_HT) - 1);
		ch->SetPoint(POINT_HT, ch->GetPoint(POINT_HT) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_HT, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (!strcmp(arg1, "iq"))
	{
		if (ch->GetRealPoint(POINT_IQ) <= JobInitialPoints[ch->GetJob()].iq)
			return;

		ch->SetRealPoint(POINT_IQ, ch->GetRealPoint(POINT_IQ) - 1);
		ch->SetPoint(POINT_IQ, ch->GetPoint(POINT_IQ) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_IQ, 0);
		ch->PointChange(POINT_MAX_SP, 0);
	}
	else
		return;

	ch->PointChange(POINT_STAT, +1);
	ch->PointChange(POINT_STAT_RESET_COUNT, -1);
	ch->ComputePoints();
}

ACMD(do_stat)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 능력을 올릴 수 없습니다."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	// ch->ChatPacket(CHAT_TYPE_INFO, "%s GRP(%d) idx(%u), MAX_STAT(%d), expr(%d)", __FUNCTION__, ch->GetRealPoint(idx), idx, MAX_STAT, ch->GetRealPoint(idx) >= MAX_STAT);
	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + 1);
	ch->SetPoint(idx, ch->GetPoint(idx) + 1);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
	{
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_HT)
	{
		ch->PointChange(POINT_MAX_SP, 0);
	}

	ch->PointChange(POINT_STAT, -1);
	ch->ComputePoints();
}

ACMD(do_pvp)
{
	if (ch->GetArena() != NULL || CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(vid);

	if (!pkVictim)
		return;

	if (pkVictim->IsNPC())
		return;

	if (pkVictim->GetArena() != NULL)
	{
		pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 대련중입니다."));
		return;
	}

	CPVPManager::instance().Insert(ch, pkVictim);
}

ACMD(do_guildskillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetGuild())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드에 속해있지 않습니다."));
		return;
	}

	CGuild* g = ch->GetGuild();
	TGuildMember* gm = g->GetMember(ch->GetPlayerID());
	if (gm->grade == GUILD_LEADER_GRADE)
	{
		DWORD vnum = 0;
		str_to_number(vnum, arg1);
		g->SkillLevelUp(vnum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드 스킬 레벨을 변경할 권한이 없습니다."));
	}
}

ACMD(do_skillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vnum = 0;
	str_to_number(vnum, arg1);

	if (true == ch->CanUseSkill(vnum))
	{
		ch->SkillLevelUp(vnum);
	}
	else
	{
		switch(vnum)
		{
			case SKILL_HORSE_WILDATTACK:
			case SKILL_HORSE_CHARGE:
			case SKILL_HORSE_ESCAPE:
			case SKILL_HORSE_WILDATTACK_RANGE:

			case SKILL_7_A_ANTI_TANHWAN:
			case SKILL_7_B_ANTI_AMSEOP:
			case SKILL_7_C_ANTI_SWAERYUNG:
			case SKILL_7_D_ANTI_YONGBI:

			case SKILL_8_A_ANTI_GIGONGCHAM:
			case SKILL_8_B_ANTI_YEONSA:
			case SKILL_8_C_ANTI_MAHWAN:
			case SKILL_8_D_ANTI_BYEURAK:

			case SKILL_ADD_HP:
			case SKILL_RESIST_PENETRATE:
				ch->SkillLevelUp(vnum);
				break;
		}
	}
}

//
//
ACMD(do_safebox_close)
{
	ch->CloseSafebox();
}

//
//
ACMD(do_safebox_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	ch->ReqSafeboxLoad(arg1);
}

ACMD(do_safebox_change_password)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || strlen(arg1)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 잘못된 암호를 입력하셨습니다."));
		return;
	}

	if (!*arg2 || strlen(arg2)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 잘못된 암호를 입력하셨습니다."));
		return;
	}

	TSafeboxChangePasswordPacket p;

	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szOldPassword, arg1, sizeof(p.szOldPassword));
	strlcpy(p.szNewPassword, arg2, sizeof(p.szNewPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_PASSWORD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 잘못된 암호를 입력하셨습니다."));
		return;
	}

	int iPulse = thecore_pulse();

	if (ch->GetMall())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 창고가 이미 열려있습니다."));
		return;
	}

	if (iPulse - ch->GetMallLoadTime() < passes_per_sec * 10)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 창고를 닫은지 10초 안에는 열 수 없습니다."));
		return;
	}

	ch->SetMallLoadTime(iPulse);

	TSafeboxLoadPacket p;
	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, ch->GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, arg1, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_MALL_LOAD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_close)
{
	if (ch->GetMall())
	{
		ch->SetMallLoadTime(thecore_pulse());
		ch->CloseMall();
		ch->Save();
	}
}

ACMD(do_ungroup)
{
	if (!ch->GetParty())
		return;

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 서버 문제로 파티 관련 처리를 할 수 없습니다."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 던전 안에서는 파티에서 나갈 수 없습니다."));
		return;
	}

	LPPARTY pParty = ch->GetParty();

	if (pParty->GetMemberCount() == 2)
	{
		// party disband
		CPartyManager::instance().DeleteParty(pParty);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<파티> 파티에서 나가셨습니다."));
		//pParty->SendPartyRemoveOneToAll(ch);
		pParty->Quit(ch->GetPlayerID());
		//pParty->SendPartyRemoveAllToOne(ch);
	}
}

ACMD(do_close_shop)
{
	if (ch->GetMyShop())
	{
		ch->CloseMyShop();
		return;
	}
}

ACMD(do_set_walk_mode)
{
	ch->SetNowWalking(true);
	ch->SetWalking(true);
}

ACMD(do_set_run_mode)
{
	ch->SetNowWalking(false);
	ch->SetWalking(false);
}

ACMD(do_war)
{
	CGuild * g = ch->GetGuild();

	if (!g)
		return;

	if (g->UnderAnyWar())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 이미 다른 전쟁에 참전 중 입니다."));
		return;
	}

	char arg1[256], arg2[256];
	DWORD type = GUILD_WAR_TYPE_FIELD; //fixme102 base int modded uint
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
		return;

	if (*arg2)
	{
		str_to_number(type, arg2);

		if ((type >= GUILD_WAR_TYPE_MAX_NUM) || (type < 0))//@hinata27
			type = GUILD_WAR_TYPE_FIELD;
	}

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드전에 대한 권한이 없습니다."));
		return;
	}

	CGuild * opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 그런 길드가 없습니다."));
		return;
	}

	switch (g->GetGuildWarState(opp_g->GetID()))
	{
		case GUILD_WAR_NONE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드가 이미 전쟁 중 입니다."));
					return;
				}

				int iWarPrice = KOR_aGuildWarInfo[type].iWarPrice;

				if (g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 전비가 부족하여 길드전을 할 수 없습니다."));
					return;
				}

				if (opp_g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드의 전비가 부족하여 길드전을 할 수 없습니다."));
					return;
				}
			}
			break;

		case GUILD_WAR_SEND_DECLARE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 선전포고 중인 길드입니다."));
				return;
			}
			break;

		case GUILD_WAR_RECV_DECLARE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드가 이미 전쟁 중 입니다."));
					g->RequestRefuseWar(opp_g->GetID());
					return;
				}
			}
			break;

		case GUILD_WAR_RESERVE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 이미 전쟁이 예약된 길드 입니다."));
				return;
			}
			break;

		case GUILD_WAR_END:
			return;

		default:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 이미 전쟁 중인 길드입니다."));
			g->RequestRefuseWar(opp_g->GetID());
			return;
	}

	if (!g->CanStartWar(type))
	{
		if (g->GetLadderPoint() == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 레더 점수가 모자라서 길드전을 할 수 없습니다."));
			sys_log(0, "GuildWar.StartError.NEED_LADDER_POINT");
		}
		else if (g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드전을 하기 위해선 최소한 %d명이 있어야 합니다."), GUILD_WAR_MIN_MEMBER_COUNT);
			sys_log(0, "GuildWar.StartError.NEED_MINIMUM_MEMBER[%d]", GUILD_WAR_MIN_MEMBER_COUNT);
		}
		else
		{
			sys_log(0, "GuildWar.StartError.UNKNOWN_ERROR");
		}
		return;
	}

	if (!opp_g->CanStartWar(GUILD_WAR_TYPE_FIELD))
	{
		if (opp_g->GetLadderPoint() == 0)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드의 레더 점수가 모자라서 길드전을 할 수 없습니다."));
		else if (opp_g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드의 길드원 수가 부족하여 길드전을 할 수 없습니다."));
		return;
	}

	do
	{
		if (g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드의 길드장이 접속중이 아닙니다."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	do
	{
		if (opp_g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(opp_g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 상대방 길드의 길드장이 접속중이 아닙니다."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	g->RequestDeclareWar(opp_g->GetID(), type);
}

ACMD(do_nowar)
{
	CGuild* g = ch->GetGuild();
	if (!g)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 길드전에 대한 권한이 없습니다."));
		return;
	}

	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<길드> 그런 길드가 없습니다."));
		return;
	}

	g->RequestRefuseWar(opp_g->GetID());
}

ACMD(do_detaillog)
{
	ch->DetailLog();
}

ACMD(do_monsterlog)
{
	ch->ToggleMonsterLog();
}

ACMD(do_pkmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE mode = 0;
	str_to_number(mode, arg1);

	if (mode == PK_MODE_PROTECT)
		return;

	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;
#ifdef ENABLE_PVP_RANKING
	if (ch->GetMapIndex() == CPvPRanking::instance().GetMapIndex() && quest::CQuestManager::instance().GetEventFlag ("pvp_ranking") > 0)
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT ("Savas alaninde bu islemi yapamazsin."));
		return;
	}
#endif

	ch->SetPKMode(mode);
}

ACMD(do_messenger_auth)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	char answer = LOWER(*arg1);
	// @fixme130 AuthToAdd void -> bool
	bool bIsDenied = answer != 'y';
	bool bIsAdded = MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, bIsDenied); // DENY
	if (bIsAdded && bIsDenied)
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 님으로 부터 친구 등록을 거부 당했습니다."), ch->GetName());
	}
}

ACMD(do_setblockmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		BYTE flag = 0;
		str_to_number(flag, arg1);
		ch->SetBlockMode(flag);
	}
}

ACMD(do_unmount)
{
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if(ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;
		
		if (!mountSystem && !mount)
			return;

		if(mount->GetValue(1) != 0)
			mobVnum = mount->GetValue(1);

		if (ch->GetMountVnum())
		{
			if(mountSystem->CountSummoned() == 0)
			{
				mountSystem->Unmount(mobVnum);
			}
		}
		return;
	}
#endif
	if (true == ch->UnEquipSpecialRideUniqueItem())
	{
		ch->RemoveAffect(AFFECT_MOUNT);
		ch->RemoveAffect(AFFECT_MOUNT_BONUS);

		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
		}
	}
	else
	{
		ch->ChatPacket( CHAT_TYPE_INFO, LC_TEXT("인벤토리가 꽉 차서 내릴 수 없습니다."));
	}
}

ACMD(do_observer_exit)
{
	if (ch->IsObserverMode())
	{
		if (ch->GetWarMap())
			ch->SetWarMap(NULL);

		if (ch->GetArena() != NULL || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != NULL)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(NULL);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else
		{
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

ACMD(do_view_equip)
{
	if (ch->GetGMLevel() <= GM_PLAYER)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		DWORD vid = 0;
		str_to_number(vid, arg1);
		LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

		if (!tch)
			return;

		if (!tch->IsPC())
			return;

		tch->SendEquipment(ch);
	}
}

ACMD(do_party_request)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
		return;
	}

	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 파티에 속해 있으므로 가입신청을 할 수 없습니다."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		if (!ch->RequestToParty(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

ACMD(do_party_request_accept)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToParty(tch);
}

ACMD(do_party_request_deny)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToParty(tch);
}

ACMD(do_monarch_warpto)
{
	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주만이 사용 가능한 기능입니다"));
		return;
	}

	if (!ch->IsMCOK(CHARACTER::MI_WARP))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d 초간 쿨타임이 적용중입니다."), ch->GetMCLTime(CHARACTER::MI_WARP));
		return;
	}

	const int WarpPrice = 10000;

	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("국고에 돈이 부족합니다. 현재 : %u 필요금액 : %u"), NationMoney, WarpPrice);
		return;
	}

	int x = 0, y = 0;
	char arg1[256];

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용법: warpto <character name>"));
		return;
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("타제국 유저에게는 이동할수 없습니다"));
				return;
			}

			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("해당 유저는 %d 채널에 있습니다. (현재 채널 %d)"), pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 이동할 수 없습니다."));
				return;
			}

			PIXEL_POSITION pos;

			if (!SECTREE_MANAGER::instance().GetCenterPositionOfMap(pkCCI->lMapIndex, pos))
				ch->ChatPacket(CHAT_TYPE_INFO, "Cannot find map (index %d)", pkCCI->lMapIndex);
			else
			{
				//ch->ChatPacket(CHAT_TYPE_INFO, "You warp to (%d, %d)", pos.x, pos.y);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 에게로 이동합니다"), arg1);
				ch->WarpSet(pos.x, pos.y);

				CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

				ch->SetMC(CHARACTER::MI_WARP);
			}
		}
		else if (NULL == CHARACTER_MANAGER::instance().FindPC(arg1))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "There is no one by that name");
		}

		return;
	}
	else
	{
		if (tch->GetEmpire() != ch->GetEmpire())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("타제국 유저에게는 이동할수 없습니다"));
			return;
		}
		if (!IsMonarchWarpZone(tch->GetMapIndex()))
		{
			ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 이동할 수 없습니다."));
			return;
		}
		x = tch->GetX();
		y = tch->GetY();
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 에게로 이동합니다"), arg1);
	ch->WarpSet(x, y);
	ch->Stop();

	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

	ch->SetMC(CHARACTER::MI_WARP);
}

ACMD(do_monarch_transfer)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용법: transfer <name>"));
		return;
	}

	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주만이 사용 가능한 기능입니다"));
		return;
	}

	if (!ch->IsMCOK(CHARACTER::MI_TRANSFER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d 초간 쿨타임이 적용중입니다."), ch->GetMCLTime(CHARACTER::MI_TRANSFER));
		return;
	}

	const int WarpPrice = 10000;

	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("국고에 돈이 부족합니다. 현재 : %u 필요금액 : %u"), NationMoney, WarpPrice);
		return;
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 제국 유저는 소환할 수 없습니다."));
				return;
			}
			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 님은 %d 채널에 접속 중 입니다. (현재 채널: %d)"), arg1, pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 이동할 수 없습니다."));
				return;
			}
			if (!IsMonarchWarpZone(ch->GetMapIndex()))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 소환할 수 없습니다."));
				return;
			}

			TPacketGGTransfer pgg;

			pgg.bHeader = HEADER_GG_TRANSFER;
			strlcpy(pgg.szName, arg1, sizeof(pgg.szName));
			pgg.lX = ch->GetX();
			pgg.lY = ch->GetY();

			P2P_MANAGER::instance().Send(&pgg, sizeof(TPacketGGTransfer));
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 님을 소환하였습니다."), arg1);

			CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

			ch->SetMC(CHARACTER::MI_TRANSFER);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("입력하신 이름을 가진 사용자가 없습니다."));
		}

		return;
	}

	if (ch == tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("자신을 소환할 수 없습니다."));
		return;
	}

	if (tch->GetEmpire() != ch->GetEmpire())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 제국 유저는 소환할 수 없습니다."));
		return;
	}
	if (!IsMonarchWarpZone(tch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 이동할 수 없습니다."));
		return;
	}
	if (!IsMonarchWarpZone(ch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("해당 지역으로 소환할 수 없습니다."));
		return;
	}

	//tch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
	tch->WarpSet(ch->GetX(), ch->GetY(), ch->GetMapIndex());

	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

	ch->SetMC(CHARACTER::MI_TRANSFER);
}

ACMD(do_monarch_info)
{
	if (CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("나의 군주 정보"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			if (n == ch->GetEmpire())
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s군주] : %s  보유금액 %lld "), EMPIRE_NAME(n), p->name[n], p->money[n]);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s군주] : %s  "), EMPIRE_NAME(n), p->name[n]);

		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주 정보"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s군주] : %s  "), EMPIRE_NAME(n), p->name[n]);

		}
	}
}

ACMD(do_elect)
{
	db_clientdesc->DBPacketHeader(HEADER_GD_COME_TO_VOTE, ch->GetDesc()->GetHandle(), 0);
}

// LUA_ADD_GOTO_INFO
struct GotoInfo
{
	std::string 	st_name;

	BYTE 	empire;
	int 	mapIndex;
	DWORD 	x, y;

	GotoInfo()
	{
		st_name 	= "";
		empire 		= 0;
		mapIndex 	= 0;

		x = 0;
		y = 0;
	}

	GotoInfo(const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void operator = (const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void __copy__(const GotoInfo& c_src)
	{
		st_name 	= c_src.st_name;
		empire 		= c_src.empire;
		mapIndex 	= c_src.mapIndex;

		x = c_src.x;
		y = c_src.y;
	}
};

ACMD(do_monarch_tax)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: monarch_tax <1-50>");
		return;
	}

	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주만이 사용할수 있는 기능입니다"));
		return;
	}

	int tax = 0;
	str_to_number(tax,  arg1);

	if (tax < 1 || tax > 50)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("1-50 사이의 수치를 선택해주세요"));

	quest::CQuestManager::instance().SetEventFlag("trade_tax", tax);

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("세금이 %d %로 설정되었습니다"));

	char szMsg[1024];

	snprintf(szMsg, sizeof(szMsg), "군주의 명으로 세금이 %d %% 로 변경되었습니다", tax);
	BroadcastNotice(szMsg);

	snprintf(szMsg, sizeof(szMsg), "앞으로는 거래 금액의 %d %% 가 국고로 들어가게됩니다.", tax);
	BroadcastNotice(szMsg);

	ch->SetMC(CHARACTER::MI_TAX);
}

static const DWORD cs_dwMonarchMobVnums[] =
{
	191,
	192,
	193,
	194,
	391,
	392,
	393,
	394,
	491,
	492,
	493,
	494,
	591,
	691,
	791,
	1304,
	1901,
	2091,
	2191,
	2206,
	0,
};

ACMD(do_monarch_mob)
{
	char arg1[256];
	LPCHARACTER	tch;

	one_argument(argument, arg1, sizeof(arg1));

	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주만이 사용할수 있는 기능입니다"));
		return;
	}

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: mmob <mob name>");
		return;
	}

#ifdef ENABLE_MONARCH_MOB_CMD_MAP_CHECK // @warme006
	BYTE pcEmpire = ch->GetEmpire();
	BYTE mapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(ch->GetMapIndex());
	if (mapEmpire != pcEmpire && mapEmpire != 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("자국 영토에서만 사용할 수 있는 기능입니다"));
		return;
	}
#endif

	const int SummonPrice = 5000000;

	if (!ch->IsMCOK(CHARACTER::MI_SUMMON))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d 초간 쿨타임이 적용중입니다."), ch->GetMCLTime(CHARACTER::MI_SUMMON));
		return;
	}

	if (!CMonarch::instance().IsMoneyOk(SummonPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("국고에 돈이 부족합니다. 현재 : %u 필요금액 : %u"), NationMoney, SummonPrice);
		return;
	}

	const CMob * pkMob;
	DWORD vnum = 0;

	if (isdigit(*arg1))
	{
		str_to_number(vnum, arg1);

		if ((pkMob = CMobManager::instance().Get(vnum)) == NULL)
			vnum = 0;
	}
	else
	{
		pkMob = CMobManager::Instance().Get(arg1, true);

		if (pkMob)
			vnum = pkMob->m_table.dwVnum;
	}

	DWORD count;

	for (count = 0; cs_dwMonarchMobVnums[count] != 0; ++count)
		if (cs_dwMonarchMobVnums[count] == vnum)
			break;

	if (0 == cs_dwMonarchMobVnums[count])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소환할수 없는 몬스터 입니다. 소환가능한 몬스터는 홈페이지를 참조하세요"));
		return;
	}

	tch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum,
			ch->GetMapIndex(),
			ch->GetX() - number(200, 750),
			ch->GetY() - number(200, 750),
			ch->GetX() + number(200, 750),
			ch->GetY() + number(200, 750),
			true,
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			true);

	if (tch)
	{
		CMonarch::instance().SendtoDBDecMoney(SummonPrice, ch->GetEmpire(), ch);

		ch->SetMC(CHARACTER::MI_SUMMON);
	}
}

static const char* FN_point_string(int apply_number)
{
	switch (apply_number)
	{
		case POINT_MAX_HP:	return LC_TEXT("최대 생명력 +%d");
		case POINT_MAX_SP:	return LC_TEXT("최대 정신력 +%d");
		case POINT_HT:		return LC_TEXT("체력 +%d");
		case POINT_IQ:		return LC_TEXT("지능 +%d");
		case POINT_ST:		return LC_TEXT("근력 +%d");
		case POINT_DX:		return LC_TEXT("민첩 +%d");
		case POINT_ATT_SPEED:	return LC_TEXT("공격속도 +%d");
		case POINT_MOV_SPEED:	return LC_TEXT("이동속도 %d");
		case POINT_CASTING_SPEED:	return LC_TEXT("쿨타임 -%d");
		case POINT_HP_REGEN:	return LC_TEXT("생명력 회복 +%d");
		case POINT_SP_REGEN:	return LC_TEXT("정신력 회복 +%d");
		case POINT_POISON_PCT:	return LC_TEXT("독공격 %d");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_PCT:	return LC_TEXT("독공격 %d");
#endif
		case POINT_STUN_PCT:	return LC_TEXT("스턴 +%d");
		case POINT_SLOW_PCT:	return LC_TEXT("슬로우 +%d");
		case POINT_CRITICAL_PCT:	return LC_TEXT("%d%% 확률로 치명타 공격");
		case POINT_RESIST_CRITICAL:	return LC_TEXT("상대의 치명타 확률 %d%% 감소");
		case POINT_PENETRATE_PCT:	return LC_TEXT("%d%% 확률로 관통 공격");
		case POINT_RESIST_PENETRATE: return LC_TEXT("상대의 관통 공격 확률 %d%% 감소");
		case POINT_ATTBONUS_HUMAN:	return LC_TEXT("인간류 몬스터 타격치 +%d%%");
		case POINT_ATTBONUS_ANIMAL:	return LC_TEXT("동물류 몬스터 타격치 +%d%%");
		case POINT_ATTBONUS_ORC:	return LC_TEXT("웅귀족 타격치 +%d%%");
		case POINT_ATTBONUS_MILGYO:	return LC_TEXT("밀교류 타격치 +%d%%");
		case POINT_ATTBONUS_UNDEAD:	return LC_TEXT("시체류 타격치 +%d%%");
		case POINT_ATTBONUS_DEVIL:	return LC_TEXT("악마류 타격치 +%d%%");
		case POINT_STEAL_HP:		return LC_TEXT("타격치 %d%% 를 생명력으로 흡수");
		case POINT_STEAL_SP:		return LC_TEXT("타력치 %d%% 를 정신력으로 흡수");
		case POINT_MANA_BURN_PCT:	return LC_TEXT("%d%% 확률로 타격시 상대 전신력 소모");
		case POINT_DAMAGE_SP_RECOVER:	return LC_TEXT("%d%% 확률로 피해시 정신력 회복");
		case POINT_BLOCK:			return LC_TEXT("물리타격시 블럭 확률 %d%%");
		case POINT_DODGE:			return LC_TEXT("활 공격 회피 확률 %d%%");
		case POINT_RESIST_SWORD:	return LC_TEXT("한손검 방어 %d%%");
		case POINT_RESIST_TWOHAND:	return LC_TEXT("양손검 방어 %d%%");
		case POINT_RESIST_DAGGER:	return LC_TEXT("두손검 방어 %d%%");
		case POINT_RESIST_BELL:		return LC_TEXT("방울 방어 %d%%");
		case POINT_RESIST_FAN:		return LC_TEXT("부채 방어 %d%%");
		case POINT_RESIST_BOW:		return LC_TEXT("활공격 저항 %d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_CLAW:		return LC_TEXT("두손검 방어 %d%%");
#endif
		case POINT_RESIST_FIRE:		return LC_TEXT("화염 저항 %d%%");
		case POINT_RESIST_ELEC:		return LC_TEXT("전기 저항 %d%%");
		case POINT_RESIST_MAGIC:	return LC_TEXT("마법 저항 %d%%");
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		case POINT_RESIST_MAGIC_REDUCTION:	return LC_TEXT("마법 저항 %d%%");
#endif
		case POINT_RESIST_WIND:		return LC_TEXT("바람 저항 %d%%");
		case POINT_RESIST_ICE:		return LC_TEXT("냉기 저항 %d%%");
		case POINT_RESIST_EARTH:	return LC_TEXT("대지 저항 %d%%");
		case POINT_RESIST_DARK:		return LC_TEXT("어둠 저항 %d%%");
		case POINT_REFLECT_MELEE:	return LC_TEXT("직접 타격치 반사 확률 : %d%%");
		case POINT_REFLECT_CURSE:	return LC_TEXT("저주 되돌리기 확률 %d%%");
		case POINT_POISON_REDUCE:	return LC_TEXT("독 저항 %d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_REDUCE:	return LC_TEXT("독 저항 %d%%");
#endif
		case POINT_KILL_SP_RECOVER:	return LC_TEXT("%d%% 확률로 적퇴치시 정신력 회복");
		case POINT_EXP_DOUBLE_BONUS:	return LC_TEXT("%d%% 확률로 적퇴치시 경험치 추가 상승");
		case POINT_GOLD_DOUBLE_BONUS:	return LC_TEXT("%d%% 확률로 적퇴치시 돈 2배 드롭");
		case POINT_ITEM_DROP_BONUS:	return LC_TEXT("%d%% 확률로 적퇴치시 아이템 2배 드롭");
		case POINT_POTION_BONUS:	return LC_TEXT("물약 사용시 %d%% 성능 증가");
		case POINT_KILL_HP_RECOVERY:	return LC_TEXT("%d%% 확률로 적퇴치시 생명력 회복");
		case POINT_ATT_GRADE_BONUS:	return LC_TEXT("공격력 +%d");
		case POINT_DEF_GRADE_BONUS:	return LC_TEXT("방어력 +%d");
		case POINT_MAGIC_ATT_GRADE:	return LC_TEXT("마법 공격력 +%d");
		case POINT_MAGIC_DEF_GRADE:	return LC_TEXT("마법 방어력 +%d");
		case POINT_MAX_STAMINA:	return LC_TEXT("최대 지구력 +%d");
		case POINT_ATTBONUS_WARRIOR:	return LC_TEXT("무사에게 강함 +%d%%");
		case POINT_ATTBONUS_ASSASSIN:	return LC_TEXT("자객에게 강함 +%d%%");
		case POINT_ATTBONUS_SURA:		return LC_TEXT("수라에게 강함 +%d%%");
		case POINT_ATTBONUS_SHAMAN:		return LC_TEXT("무당에게 강함 +%d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_ATTBONUS_WOLFMAN:	return LC_TEXT("무당에게 강함 +%d%%");
#endif
		case POINT_ATTBONUS_MONSTER:	return LC_TEXT("몬스터에게 강함 +%d%%");
		case POINT_MALL_ATTBONUS:		return LC_TEXT("공격력 +%d%%");
		case POINT_MALL_DEFBONUS:		return LC_TEXT("방어력 +%d%%");
		case POINT_MALL_EXPBONUS:		return LC_TEXT("경험치 %d%%");
		case POINT_MALL_ITEMBONUS:		return LC_TEXT("아이템 드롭율 %d배"); // @fixme180 float to int
		case POINT_MALL_GOLDBONUS:		return LC_TEXT("돈 드롭율 %d배"); // @fixme180 float to int
		case POINT_MAX_HP_PCT:			return LC_TEXT("최대 생명력 +%d%%");
		case POINT_MAX_SP_PCT:			return LC_TEXT("최대 정신력 +%d%%");
		case POINT_SKILL_DAMAGE_BONUS:	return LC_TEXT("스킬 데미지 %d%%");
		case POINT_NORMAL_HIT_DAMAGE_BONUS:	return LC_TEXT("평타 데미지 %d%%");
		case POINT_SKILL_DEFEND_BONUS:		return LC_TEXT("스킬 데미지 저항 %d%%");
		case POINT_NORMAL_HIT_DEFEND_BONUS:	return LC_TEXT("평타 데미지 저항 %d%%");
		case POINT_RESIST_WARRIOR:	return LC_TEXT("무사공격에 %d%% 저항");
		case POINT_RESIST_ASSASSIN:	return LC_TEXT("자객공격에 %d%% 저항");
		case POINT_RESIST_SURA:		return LC_TEXT("수라공격에 %d%% 저항");
		case POINT_RESIST_SHAMAN:	return LC_TEXT("무당공격에 %d%% 저항");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_WOLFMAN:	return LC_TEXT("무당공격에 %d%% 저항");
#endif
		default:					return "UNK_ID %d%%"; // @fixme180
	}
}

static bool FN_hair_affect_string(LPCHARACTER ch, char *buf, size_t bufsiz)
{
	if (NULL == ch || NULL == buf)
		return false;

	CAffect* aff = NULL;
	time_t expire = 0;
	struct tm ltm;
	int	year, mon, day;
	int	offset = 0;

	aff = ch->FindAffect(AFFECT_HAIR);

	if (NULL == aff)
		return false;

	expire = ch->GetQuestFlag("hair.limit_time");

	if (expire < get_global_time())
		return false;

	// set apply string
	offset = snprintf(buf, bufsiz, FN_point_string(aff->bApplyOn), aff->lApplyValue);

	if (offset < 0 || offset >= (int) bufsiz)
		offset = bufsiz - 1;

	localtime_r(&expire, &ltm);

	year	= ltm.tm_year + 1900;
	mon		= ltm.tm_mon + 1;
	day		= ltm.tm_mday;

	snprintf(buf + offset, bufsiz - offset, LC_TEXT(" (만료일 : %d년 %d월 %d일)"), year, mon, day);

	return true;
}

ACMD(do_costume)
{
	char buf[1024]; // @warme015
	const size_t bufferSize = sizeof(buf);

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	CItem* pBody = ch->GetWear(WEAR_COSTUME_BODY);
	CItem* pHair = ch->GetWear(WEAR_COSTUME_HAIR);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	CItem* pMount = ch->GetWear(WEAR_COSTUME_MOUNT);
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	CItem* pAcce = ch->GetWear(WEAR_COSTUME_ACCE);
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	CItem* pWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
#endif
#ifdef ENABLE_WINGS_NEW
	CItem* pWing = ch->GetWear(WEAR_COSTUME_WING);
#endif

	ch->ChatPacket(CHAT_TYPE_INFO, "COSTUME status:");

	if (pHair)
	{
		const char* itemName = pHair->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  HAIR : %s", itemName);

		for (int i = 0; i < pHair->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pHair->GetAttribute(i);
			if (0 < attr.bType)
			{
				snprintf(buf, bufferSize, FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pHair->IsEquipped() && arg1[0] == 'h')
			ch->UnequipItem(pHair);
	}

	if (pBody)
	{
		const char* itemName = pBody->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  BODY : %s", itemName);

		if (pBody->IsEquipped() && arg1[0] == 'b')
			ch->UnequipItem(pBody);
	}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (pMount)
	{
		const char* itemName = pMount->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  MOUNT : %s", itemName);

		if (pMount->IsEquipped() && arg1[0] == 'm')
			ch->UnequipItem(pMount);
	}
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	if (pAcce)
	{
		const char* itemName = pAcce->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  ACCE : %s", itemName);

		if (pAcce->IsEquipped() && arg1[0] == 'a')
			ch->UnequipItem(pAcce);
	}
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	if (pWeapon)
	{
		const char* itemName = pWeapon->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  WEAPON : %s", itemName);

		if (pWeapon->IsEquipped() && arg1[0] == 'w')
			ch->UnequipItem(pWeapon);
	}
#endif

#ifdef ENABLE_WINGS_NEW
	if (pWing)
	{
		const char* itemName = pWing->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  WING : %s", itemName);

		if (pWing->IsEquipped() && arg1[0] == 'wing')
			ch->UnequipItem(pWing);
	}
#endif

}

ACMD(do_hair)
{
	char buf[256];

	if (false == FN_hair_affect_string(ch, buf, sizeof(buf)))
		return;

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

ACMD(do_inventory)
{
	int	index = 0;
	int	count		= 1;

	char arg1[256];
	char arg2[256];

	LPITEM	item;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: inventory <start_index> <count>");
		return;
	}

	if (!*arg2)
	{
		index = 0;
		str_to_number(count, arg1);
	}
	else
	{
		str_to_number(index, arg1); index = MIN(index, INVENTORY_MAX_NUM);
		str_to_number(count, arg2); count = MIN(count, INVENTORY_MAX_NUM);
	}

	for (int i = 0; i < count; ++i)
	{
		if (index >= INVENTORY_MAX_NUM)
			break;

		item = ch->GetInventoryItem(index);

		ch->ChatPacket(CHAT_TYPE_INFO, "inventory [%d] = %s",
						index, item ? item->GetName() : "<NONE>");
		++index;
	}
}

//gift notify quest command
ACMD(do_gift)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift");
}

/* ACMD(do_cube)
{
	if (!ch->CanDoCube())
		return;

	sys_log(1, "CUBE COMMAND <%s>: %s", ch->GetName(), argument);
	int cube_index = 0, inven_index = 0;
#ifdef ENABLE_SPECIAL_STORAGE
	int inven_type = 0;
	
	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
#else
	const char *line;

	char arg1[256], arg2[256], arg3[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));
#endif

	if (0 == arg1[0])
	{
		// print usage
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: cube open");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube close");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube add <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube delete <cube_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube list");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube cancel");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube make [all]");
		return;
	}

	const std::string& strArg1 = std::string(arg1);

	// r_info (request information)

	//					    (Server -> Client) /cube r_list npcVNUM resultCOUNT 123,1/125,1/128,1/130,5

	//					   (Server -> Client) /cube m_info startIndex count 125,1|126,2|127,2|123,5&555,5&555,4/120000@125,1|126,2|127,2|123,5&555,5&555,4/120000

	if (strArg1 == "r_info")
	{
		if (0 == arg2[0])
			Cube_request_result_list(ch);
		else
		{
			if (isdigit(*arg2))
			{
				int listIndex = 0, requestCount = 1;
				str_to_number(listIndex, arg2);

				if (0 != arg3[0] && isdigit(*arg3))
					str_to_number(requestCount, arg3);

				Cube_request_material_info(ch, listIndex, requestCount);
			}
		}

		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		case 'c':	// close
			Cube_close(ch);
			break;

		case 'l':	// list
			Cube_show_list(ch);
			break;

		case 'a':	// add cue_index inven_index
			{
				if (0 == arg2[0] || !isdigit(*arg2) ||
					0 == arg3[0] || !isdigit(*arg3))
					return;

				str_to_number(cube_index, arg2);
				str_to_number(inven_index, arg3);
#ifdef ENABLE_SPECIAL_STORAGE
				str_to_number(inven_type, arg4);
				Cube_add_item (ch, cube_index, inven_index, inven_type);
#else
				Cube_add_item (ch, cube_index, inven_index);
#endif
			}
			break;

		case 'd':	// delete
			{
				if (0 == arg2[0] || !isdigit(*arg2))
					return;

				str_to_number(cube_index, arg2);
				Cube_delete_item (ch, cube_index);
			}
			break;

		case 'm':	// make
			if (0 != arg2[0])
			{
				while (true == Cube_make(ch))
					sys_log(1, "cube make success");
			}
			else
				Cube_make(ch);
			break;

		default:
			return;
	}
} */

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
ACMD(do_cube)
{

	const char* line;
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
	{
		return;
	}

	switch (LOWER(arg1[0]))
	{
	case 'o':	// open
		Cube_open(ch);
		break;

	default:
		return;
	}
}
#endif

ACMD(do_in_game_mall)
{
	if (LC_IsEurope() == true)
	{
		char country_code[3];

		switch (LC_GetLocalType())
		{
			case LC_GERMANY:	country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_FRANCE:		country_code[0] = 'f'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_ITALY:		country_code[0] = 'i'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_SPAIN:		country_code[0] = 'e'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_UK:			country_code[0] = 'e'; country_code[1] = 'n'; country_code[2] = '\0'; break;
			case LC_TURKEY:		country_code[0] = 't'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_POLAND:		country_code[0] = 'p'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_PORTUGAL:	country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_GREEK:		country_code[0] = 'g'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_RUSSIA:		country_code[0] = 'r'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_DENMARK:	country_code[0] = 'd'; country_code[1] = 'k'; country_code[2] = '\0'; break;
			case LC_BULGARIA:	country_code[0] = 'b'; country_code[1] = 'g'; country_code[2] = '\0'; break;
			case LC_CROATIA:	country_code[0] = 'h'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_MEXICO:		country_code[0] = 'm'; country_code[1] = 'x'; country_code[2] = '\0'; break;
			case LC_ARABIA:		country_code[0] = 'a'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_CZECH:		country_code[0] = 'c'; country_code[1] = 'z'; country_code[2] = '\0'; break;
			case LC_ROMANIA:	country_code[0] = 'r'; country_code[1] = 'o'; country_code[2] = '\0'; break;
			case LC_HUNGARY:	country_code[0] = 'h'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_NETHERLANDS: country_code[0] = 'n'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_USA:		country_code[0] = 'u'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_CANADA:	country_code[0] = 'c'; country_code[1] = 'a'; country_code[2] = '\0'; break;
			default:
				if (test_server == true)
				{
					country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0';
				}
				break;
		}

		char buf[512+1];
		char sas[33];
		MD5_CTX ctx;
		const char sas_key[] = "GF9001";

		snprintf(buf, sizeof(buf), "%u%u%s", ch->GetPlayerID(), ch->GetAID(), sas_key);

		MD5Init(&ctx);
		MD5Update(&ctx, (const unsigned char *) buf, strlen(buf));
#ifdef __FreeBSD__
		MD5End(&ctx, sas);
#else
		static const char hex[] = "0123456789abcdef";
		unsigned char digest[16];
		MD5Final(digest, &ctx);
		int i;
		for (i = 0; i < 16; ++i) {
			sas[i+i] = hex[digest[i] >> 4];
			sas[i+i+1] = hex[digest[i] & 0x0f];
		}
		sas[i+i] = '\0';
#endif

		snprintf(buf, sizeof(buf), "mall http://%s/ishop?pid=%u&c=%s&sid=%d&sas=%s",
				g_strWebMallURL.c_str(), ch->GetPlayerID(), country_code, g_server_id, sas);

		ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
	}
}

ACMD(do_dice)
{
	char arg1[256], arg2[256];
	int start = 1, end = 100;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (*arg1 && *arg2)
	{
		start = atoi(arg1);
		end = atoi(arg2);
	}
	else if (*arg1 && !*arg2)
	{
		start = 1;
		end = atoi(arg1);
	}

	end = MAX(start, end);
	start = MIN(start, end);

	int n = number(start, end);

#ifdef ENABLE_DICE_SYSTEM
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_DICE_INFO, LC_TEXT("%s님이 주사위를 굴려 %d가 나왔습니다. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_DICE_INFO, LC_TEXT("당신이 주사위를 굴려 %d가 나왔습니다. (%d-%d)"), n, start, end);
#else
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_INFO, LC_TEXT("%s님이 주사위를 굴려 %d가 나왔습니다. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("당신이 주사위를 굴려 %d가 나왔습니다. (%d-%d)"), n, start, end);
#endif
}

#ifdef ENABLE_NEWSTUFF
ACMD(do_click_safebox)
{
	if ((ch->GetGMLevel() <= GM_PLAYER) && (ch->GetDungeon() || ch->GetWarMap()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open the safebox in dungeon or at war."));
		return;
	}

	ch->SetSafeboxOpenPosition();
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
}
ACMD(do_force_logout)
{
	LPDESC pDesc=DESC_MANAGER::instance().FindByCharacterName(ch->GetName());
	if (!pDesc)
		return;
	pDesc->DelayedDisconnect(0);
}
#endif

ACMD(do_click_mall)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
}

ACMD(do_ride)
{
    sys_log(1, "[DO_RIDE] start");
    if (ch->IsDead() || ch->IsStun())
	return;

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (ch->IsPolymorphed() == true){
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("donusmuskenbinegebinemezsin."));
		return;
	}
	if(ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;
		
		if (!mountSystem && !mount)
			return;
		
		if(mount->GetValue(1) != 0)
			mobVnum = mount->GetValue(1);
		
		if (ch->GetMountVnum())
		{
			if(mountSystem->CountSummoned() == 0)
			{
				mountSystem->Unmount(mobVnum);
			}
		}
		else
		{
			if(mountSystem->CountSummoned() == 1)
			{
				mountSystem->Mount(mobVnum, mount);
			}
		}
		
		return;
	}
#endif

    {
	if (ch->IsHorseRiding())
	{
	    sys_log(1, "[DO_RIDE] stop riding");
	    ch->StopRiding();
	    return;
	}

	if (ch->GetMountVnum())
	{
	    sys_log(1, "[DO_RIDE] unmount");
	    do_unmount(ch, NULL, 0, 0);
	    return;
	}
    }

    {
	if (ch->GetHorse() != NULL)
	{
	    sys_log(1, "[DO_RIDE] start riding");
	    ch->StartRiding();
	    return;
	}

	for (BYTE i=0; i<INVENTORY_MAX_NUM; ++i)
	{
	    LPITEM item = ch->GetInventoryItem(i);
	    if (NULL == item)
			continue;

		if (item->IsRideItem())
		{
			if (
				NULL==ch->GetWear(WEAR_UNIQUE1)
				|| NULL==ch->GetWear(WEAR_UNIQUE2)
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				|| NULL == ch->GetWear(WEAR_COSTUME_MOUNT)
#endif
			)
			{
				sys_log(1, "[DO_RIDE] USE UNIQUE ITEM");
				//ch->EquipItem(item);
				ch->UseItem(TItemPos (INVENTORY, i));
				return;
			}
		}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
			if (item->IsMountItem())
			{
				if (NULL==ch->GetWear(WEAR_COSTUME_MOUNT))
				{
					sys_log(1,  "[DO_RIDE] USE UNIQUE ITEM");
					//ch->EquipItem(item);
					ch->UseItem(TItemPos (INVENTORY, i));
					return;
				}
			}
#endif

	    switch (item->GetVnum())
	    {
		case 71114:
		case 71116:
		case 71118:
		case 71120:
		    sys_log(1, "[DO_RIDE] USE QUEST ITEM");
		    ch->UseItem(TItemPos (INVENTORY, i));
		    return;
	    }

		if( (item->GetVnum() > 52000) && (item->GetVnum() < 52091) )	{
			sys_log(1, "[DO_RIDE] USE QUEST ITEM");
			ch->UseItem(TItemPos (INVENTORY, i));
		    return;
		}
	}
    }

    ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 먼저 소환해주세요."));
}

#ifdef ENABLE_MOVE_CHANNEL
ACMD(DoChangeChannel)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	WORD channel = 0;
	str_to_number(channel, arg1);
	if (!channel)
		return;

	ch->ChangeChannel(channel);
}
#endif

#ifdef ENABLE_SPECIAL_STORAGE
ACMD(do_transfer_inv_storage)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD dwPos = atoi(arg1);

	if (dwPos >= 0 && dwPos <= INVENTORY_MAX_NUM)
	{
		LPITEM item = ch->GetInventoryItem(dwPos);

		if (item && item->IsSpecialStorageItem())
		{
			int iEmptyPos = ch->GetEmptySpecialStorageSlot(item);

			if (iEmptyPos != -1)
				ch->MoveItem(TItemPos(INVENTORY, item->GetCell()), TItemPos(item->GetSpecialWindowType(), iEmptyPos), item->GetCount());
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("|TThere isn't enough space in the special storage.|t"));
		}
	}
}

ACMD(do_transfer_storage_inv)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	DWORD dwPos = atoi(arg1);

	BYTE window_type = atoi(arg2);

	if (dwPos >= 0 && dwPos <= INVENTORY_MAX_NUM && window_type >= UPGRADE_INVENTORY && window_type <= BONUS_INVENTORY)
	{
		LPITEM item = ch->GetSpecialStorageItem(dwPos, window_type);

		if (item && item->IsSpecialStorageItem())
		{
			int iEmptyPos = ch->GetEmptyInventory(item->GetSize());

			if (iEmptyPos != -1)
				ch->MoveItem(TItemPos(item->GetSpecialWindowType(), item->GetCell()), TItemPos(INVENTORY, iEmptyPos), item->GetCount());
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("|TThere isn't enough space in the inventory.|t"));
		}
	}
}
#endif

#ifdef ENABLE_10_STATU
ACMD(do_stat_val)
{
	char	arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	int val = 0;
	str_to_number(val, arg2);

	if (!*arg1 || val <= 0)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 능력을 올릴 수 없습니다."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	if (val > ch->GetPoint(POINT_STAT))
		val = ch->GetPoint(POINT_STAT);

	if (ch->GetRealPoint(idx) + val > MAX_STAT)
		val = MAX_STAT - ch->GetRealPoint(idx);

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + val);
	ch->SetPoint(idx, ch->GetPoint(idx) + val);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
		ch->PointChange(POINT_MAX_HP, 0);
	else if (idx == POINT_HT)
		ch->PointChange(POINT_MAX_SP, 0);

	ch->PointChange(POINT_STAT, -val);
	ch->ComputePoints();
}
#endif

#ifdef ENABLE_SPECIAL_STORAGE
ACMD(do_sort_inventory_items)
{
	if (!ch)
		return;

	if (quest::CQuestManager::instance().GetEventFlag("duzenle_kapat") == 1) // /e duzenle_kapat 1 
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Sistem> Sistem deaktif."));
		return;
	}

	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsAcceOpened(true) || ch->IsAcceOpened(false)
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
			|| ch->IsAcceOpened()
#endif
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PLEASE_BEFORE_CLOSE_WINDOW_AND_USE_THIS_FUNCTION"));
		return;
	}

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("WARP_HAS_CANCELLED"));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}

	if (!ch->CanWarp())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Bunu yapmak icin tum pencereleri kapatip tekrar deneyin.");
		return;
	}

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);

		if (!item)
			continue;

		if (item->isLocked())
			continue;

		if (item->GetCount() == g_bItemCountLimit)
			continue;

		int pos = ch->GetEmptyInventory(item->GetSize());

		if (pos >= 0 && pos < item->GetCell())
		{
			item->RemoveFromCharacter();
			item->AddToCharacter(ch, TItemPos(INVENTORY, pos));
		}

		if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			for (int j = i; j < INVENTORY_MAX_NUM; ++j)
			{
				LPITEM item2 = ch->GetInventoryItem(j);

				if (!item2)
					continue;

				if (item2->isLocked())
					continue;

				if (item2->GetVnum() == item->GetVnum())
				{
					bool bStopSockets = false;

					for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
					{
						if (item2->GetSocket(k) != item->GetSocket(k))
						{
							bStopSockets = true;
							break;
						}
					}

					if (bStopSockets)
						continue;

#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
					DWORD bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
#else
					BYTE bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
#endif

					item->SetCount(item->GetCount() + bAddCount);
					item2->SetCount(item2->GetCount() - bAddCount);

					continue;
				}
			}
		}
	}
}

ACMD(do_sort_special_storage)
{
    if (!ch)
        return;

    if (quest::CQuestManager::instance().GetEventFlag("duzenle_kapat") == 1)
    {
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Sistem> Sistem deaktif."));
        return;
    }

    if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || 
        ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsAcceOpened(true) || ch->IsAcceOpened(false))
    {
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PLEASE_BEFORE_CLOSE_WINDOW_AND_USE_THIS_FUNCTION"));
        return;
    }

    if (ch->m_pkTimedEvent)
    {
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("WARP_HAS_CANCELLED"));
        event_cancel(&ch->m_pkTimedEvent);
        return;
    }

    if (!ch->CanWarp())
    {
        ch->ChatPacket(CHAT_TYPE_INFO, "Bunu yapmak icin tum pencereleri kapatip tekrar deneyin.");
        return;
    }

    for (int m = 0; m < 5; ++m)
    {
        for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
        {
            LPITEM item = nullptr;

            switch (m)
            {
                case 0:
                    item = ch->GetUpgradeInventoryItem(i);
                    break;
                case 1:
                    item = ch->GetBookInventoryItem(i);
                    break;
                case 2:
                    item = ch->GetStoneInventoryItem(i);
                    break;
                case 3:
                    item = ch->GetGeneralInventoryItem(i);
                    break;
                case 4:
                    item = ch->GetBonusInventoryItem(i);
                    break;
                default:
                    continue;
            }

            if (!item || item->isLocked() || item->GetCount() >= g_bItemCountLimit)
                continue;

            if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
            {
                for (int j = i + 1; j < SPECIAL_INVENTORY_MAX_NUM; ++j)
                {
                    LPITEM item2 = nullptr;

                    switch (m)
                    {
                        case 0:
                            item2 = ch->GetUpgradeInventoryItem(j);
                            break;
                        case 1:
                            item2 = ch->GetBookInventoryItem(j);
                            break;
                        case 2:
                            item2 = ch->GetStoneInventoryItem(j);
                            break;
                        case 3:
                            item2 = ch->GetGeneralInventoryItem(j);
                            break;
                        case 4:
                            item2 = ch->GetBonusInventoryItem(j);
                            break;
                        default:
                            continue;
                    }

                    if (!item2 || item2->isLocked() || item2->GetVnum() != item->GetVnum())
                        continue;

                    bool bStopSockets = false;
                    for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
                    {
                        if (item2->GetSocket(k) != item->GetSocket(k))
                        {
                            bStopSockets = true;
                            break;
                        }
                    }

                    if (bStopSockets)
                        continue;

#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
                    DWORD bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
#else
                    BYTE bAddCount = MIN(g_bItemCountLimit - item->GetCount(), item2->GetCount());
#endif

                    if (bAddCount > 0)
                    {
                        item->SetCount(item->GetCount() + bAddCount);
                        item2->SetCount(item2->GetCount() - bAddCount);
                    }
                }
            }
        }
    }

    ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Envanteriniz d�zenlendi!"));
}

#endif

#ifdef ENABLE_WHEEL_OF_FORTUNE
ACMD(do_wheel_of_fortune)
{
	std::vector<std::string> vecArgs;
	split_argument(argument, vecArgs);
	if (vecArgs.size() < 2) { return; }

	else if (vecArgs[1] == "spin")
	{
		if (ch->GetProtectTime("WheelWorking") != 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Wheel already is working!");
			return;
		}
		else if (quest::CQuestManager::instance().GetEventFlag("whell_event") != 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Event is not active!");
			return;
		}

		if (vecArgs.size() < 3) { return; }

		DWORD dwWhellIdx = 0;
		if (!str_to_number(dwWhellIdx, vecArgs[2].c_str()))
			return;

		if (ch->GetExchange())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You can't do it with exchange!");
			return;
		}
		//ch->SetExchangeTime();

		const std::map<DWORD, CWhellItemGroup*>* whellDataMap = ITEM_MANAGER::Instance().GetWhellItemsData();
		const auto it = whellDataMap->find(dwWhellIdx);
		if (it == whellDataMap->end())
		{
			sys_err("unknown wheel request! player: %s whellidx: %u", ch->GetName(), dwWhellIdx);
			return;
		}

		if (dwWhellIdx == 0)//prremium cark itemi
		{

			if (ch->CountSpecifyItem(900009) <= 0) 
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli Malzeme Yok !"));
				return;
			}

			ch->RemoveSpecifyItem(900009, 1);

		}

		else if (dwWhellIdx == 19)
		{
			if (ch->CountSpecifyItem(900011) <= 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli Malzeme Yok !"));
				return;
			}
			ch->RemoveSpecifyItem(900011, 1);
		}
		else if (dwWhellIdx == 29)
		{
			if (ch->CountSpecifyItem(900012) <= 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli Malzeme Yok !"));
				return;
			}
			ch->RemoveSpecifyItem(900012, 1);
		}
		else if (dwWhellIdx == 39)
		{
			if (ch->CountSpecifyItem(900010) <= 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli Malzeme Yok !"));
				return;
			}
			ch->RemoveSpecifyItem(900010, 1);
		}
		else if (dwWhellIdx == 49)
		{
			if (ch->CountSpecifyItem(900013) <= 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli Malzeme Yok !"));
				return;
			}
			ch->RemoveSpecifyItem(900013, 1);
		}
		else
		{
			if (dwWhellIdx < 10 || (dwWhellIdx != 19 && dwWhellIdx != 29 && dwWhellIdx != 39 && dwWhellIdx != 49))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Invalid wheel index!");
				return;
			}

			if (ch->CountSpecifyItem(dwWhellIdx) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "You don't have enough item for spin!");
				return;
			}
			ch->RemoveSpecifyItem(dwWhellIdx, 1);
		}

		std::vector<std::pair<DWORD, int>> m_send_items{};
		const auto itemVector = it->second->GetVector();
		std::string cmd_wheel("");
		while (true)
		{
			const auto randomItem = it->second->GetOne();
			if (std::find(m_send_items.begin(), m_send_items.end(), std::make_pair(randomItem.dwItemVnum, randomItem.iCount)) != m_send_items.end())
				continue;
			
			if (randomItem.iRarePct >= number(1, 100))
			{
				m_send_items.emplace_back(randomItem.dwItemVnum, randomItem.iCount);
				if(cmd_wheel.length())
					cmd_wheel += "#";
				
				char tmpBuf[64];
				snprintf(tmpBuf, sizeof(tmpBuf), "%u|%d", randomItem.dwItemVnum, randomItem.iCount);
				cmd_wheel += tmpBuf;
			}
			if (m_send_items.size() >= 10)
				break;
		}

		const BYTE bLuckyIdx = number(0, 9);
		ch->SetProtectTime("WheelIndex", dwWhellIdx);
		ch->SetProtectTime("WheelLuckyIndex", bLuckyIdx);
		ch->SetProtectTime("WheelLuckyItemVnum", m_send_items[bLuckyIdx].first);
		ch->SetProtectTime("WheelLuckyItemCount", m_send_items[bLuckyIdx].second);
		ch->SetProtectTime("WheelWorking", 1);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "SetWhellData %u %s", bLuckyIdx, cmd_wheel.c_str());
	}
	else if (vecArgs[1] == "loaditem")
	{
		if (vecArgs.size() < 3) { return; }

		DWORD dwWhellIdx = 0;
		if (!str_to_number(dwWhellIdx, vecArgs[2].c_str()))
			return;

		const std::map<DWORD, CWhellItemGroup*>* whellDataMap = ITEM_MANAGER::Instance().GetWhellItemsData();
		const auto it = whellDataMap->find(dwWhellIdx);
		if (it == whellDataMap->end())
		{
			sys_err("unknown wheel request! player: %s whellidx: %u", ch->GetName(), dwWhellIdx);
			return;
		}

		const auto itemVector = it->second->GetVector();
		std::string cmd_wheel("");

		for (auto itItem = itemVector->begin(); itItem != itemVector->end(); ++itItem)
		{
			if (cmd_wheel.length())
				cmd_wheel += "#";
			
			char tmpBuf[64];
			snprintf(tmpBuf, sizeof(tmpBuf), "%u|%d", itItem->dwItemVnum, itItem->iCount);
			cmd_wheel += tmpBuf;

			if (cmd_wheel.length() > 200)
			{
				ch->ChatPacket(CHAT_TYPE_COMMAND, "SetLoadWhellData %u %s", dwWhellIdx, cmd_wheel.c_str());
				cmd_wheel.clear();
			}
		}

		if (cmd_wheel.length())
			ch->ChatPacket(CHAT_TYPE_COMMAND, "SetLoadWhellData %u %s", dwWhellIdx, cmd_wheel.c_str());

	}
	else if (vecArgs[1] == "done")
	{
		if (ch->GetProtectTime("WheelWorking") != 0)
		{
			const DWORD dwSelectedItemIdx = ch->GetProtectTime("WheelLuckyItemVnum"), dwSelectedItemCount = ch->GetProtectTime("WheelLuckyItemCount");
			ch->AutoGiveItem(dwSelectedItemIdx, dwSelectedItemCount);

			ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowWhellReward %u %u", dwSelectedItemIdx, dwSelectedItemCount);
			ch->SetProtectTime("WheelIndex", 0);
			ch->SetProtectTime("WheelLuckyIndex", 0);
			ch->SetProtectTime("WheelLuckyItemVnum", 0);
			ch->SetProtectTime("WheelLuckyItemCount", 0);
			ch->SetProtectTime("WheelWorking", 0);
		}
	}
	else if (vecArgs[1] == "event")
	{
		if (!ch->IsGM())
			return;
		if (vecArgs.size() < 3) { return; }

		if (vecArgs[2] == "active")
		{
			BroadcastNotice("Wheel event is started.");
			quest::CQuestManager::instance().RequestSetEventFlag("whell_event", 1);
		}
		else if (vecArgs[2] == "deactive")
		{
			BroadcastNotice("Wheel event is done.");
			quest::CQuestManager::instance().RequestSetEventFlag("whell_event", 0);
		}
	}
}

#endif



// vaffanculo a chi t'e morto martysama
