#include "stdafx.h"
#include "questlua.h"
#include "questmanager.h"
#include "desc_client.h"
#include "char.h"
#include "char_manager.h"
#include "utils.h"
#include "guild.h"
#include "guild_manager.h"
#include "ronark_map.h"

namespace quest
{
	ALUA(ronark_start)
	{
		int iStatus = (int)lua_tonumber(L, 1);
		int iMap = (int)lua_tonumber(L, 2);
		CRonarkMapManager::instance().SetRonarkStatus(iStatus, iMap);
		return 0;
	}

	ALUA(ronark_get_status)
	{
		lua_pushnumber(L, CRonarkMapManager::instance().GetRonarkStatus());
		return 1;
	}

	ALUA(ronark_set_member_limit)
	{
		int iMember = (int)lua_tonumber(L, 1);
		CRonarkMapManager::instance().SetMemberLimitCount(iMember);
		return 0;
	}

	ALUA(ronark_get_member_limit)
	{
		lua_pushnumber(L, CRonarkMapManager::instance().GetMemberLimitCount());
		return 1;
	}

	ALUA(ronark_get_total_member_count)
	{
		lua_pushnumber(L, CRonarkMapManager::instance().GetTotalMemberCount());
		return 1;
	}

	ALUA(ronark_destroy)
	{
		CRonarkMapManager::instance().ResetEvent();
		return 0;
	}

	ALUA(ronark_get_winner_guild)
	{
		lua_pushnumber(L, CRonarkMapManager::instance().GetWinnerGuild());
		return 1;
	}

	void RegisterRonarkFunctionTable()
	{
		luaL_reg ronark_functions[] =
		{
			{	"start",	ronark_start	},
			{	"get_status",	ronark_get_status	},
			{	"set_member_limit",	ronark_set_member_limit	},
			{	"get_member_limit",	ronark_get_member_limit	},
			{	"get_total_member_count",	ronark_get_total_member_count	},
			{	"get_winner_guild",			ronark_get_winner_guild			},
			{	"destroy",	ronark_destroy	},
			{	NULL,		NULL		}
		};

		CQuestManager::instance().AddLuaFunctionTable("ronark", ronark_functions);
	}
}