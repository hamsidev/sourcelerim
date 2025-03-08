#ifndef __INC_TABLES_H__
#define __INC_TABLES_H__

#include "length.h"
#include "item_length.h"
#include "CommonDefines.h"

typedef	DWORD IDENT;

enum
{
	HEADER_GD_LOGIN				= 1,
	HEADER_GD_LOGOUT			= 2,

	HEADER_GD_PLAYER_LOAD		= 3,
	HEADER_GD_PLAYER_SAVE		= 4,
	HEADER_GD_PLAYER_CREATE		= 5,
	HEADER_GD_PLAYER_DELETE		= 6,

	HEADER_GD_LOGIN_KEY			= 7,
	// 8 empty
	HEADER_GD_BOOT				= 9,
	HEADER_GD_PLAYER_COUNT		= 10,
	HEADER_GD_QUEST_SAVE		= 11,
	HEADER_GD_SAFEBOX_LOAD		= 12,
	HEADER_GD_SAFEBOX_SAVE		= 13,
	HEADER_GD_SAFEBOX_CHANGE_SIZE	= 14,
	HEADER_GD_EMPIRE_SELECT		= 15,

	HEADER_GD_SAFEBOX_CHANGE_PASSWORD		= 16,
	HEADER_GD_SAFEBOX_CHANGE_PASSWORD_SECOND	= 17, // Not really a packet, used internal
	HEADER_GD_DIRECT_ENTER		= 18,

	HEADER_GD_GUILD_SKILL_UPDATE	= 19,
	HEADER_GD_GUILD_EXP_UPDATE		= 20,
	HEADER_GD_GUILD_ADD_MEMBER		= 21,
	HEADER_GD_GUILD_REMOVE_MEMBER	= 22,
	HEADER_GD_GUILD_CHANGE_GRADE	= 23,
	HEADER_GD_GUILD_CHANGE_MEMBER_DATA	= 24,
	HEADER_GD_GUILD_DISBAND		= 25,
	HEADER_GD_GUILD_WAR			= 26,
	HEADER_GD_GUILD_WAR_SCORE		= 27,
	HEADER_GD_GUILD_CREATE		= 28,

	HEADER_GD_ITEM_SAVE			= 30,
	HEADER_GD_ITEM_DESTROY		= 31,

	HEADER_GD_ADD_AFFECT		= 32,
	HEADER_GD_REMOVE_AFFECT		= 33,

	HEADER_GD_ITEM_FLUSH		= 35,

	HEADER_GD_PARTY_CREATE		= 36,
	HEADER_GD_PARTY_DELETE		= 37,
	HEADER_GD_PARTY_ADD			= 38,
	HEADER_GD_PARTY_REMOVE		= 39,
	HEADER_GD_PARTY_STATE_CHANGE	= 40,
	HEADER_GD_PARTY_HEAL_USE		= 41,

	HEADER_GD_RELOAD_PROTO		= 43,

	HEADER_GD_CHANGE_NAME		= 44,

	HEADER_GD_GUILD_CHANGE_LADDER_POINT	= 46,
	HEADER_GD_GUILD_USE_SKILL		= 47,

	HEADER_GD_REQUEST_EMPIRE_PRIV	= 48,
	HEADER_GD_REQUEST_GUILD_PRIV	= 49,

	HEADER_GD_MONEY_LOG				= 50,

	HEADER_GD_GUILD_DEPOSIT_MONEY				= 51,
	HEADER_GD_GUILD_WITHDRAW_MONEY				= 52,
	HEADER_GD_GUILD_WITHDRAW_MONEY_GIVE_REPLY	= 53,

	HEADER_GD_REQUEST_CHARACTER_PRIV	= 54,

	HEADER_GD_SET_EVENT_FLAG			= 55,

	HEADER_GD_PARTY_SET_MEMBER_LEVEL	= 56,

	HEADER_GD_GUILD_WAR_BET		= 57,

	HEADER_GD_CREATE_OBJECT		= 60,
	HEADER_GD_DELETE_OBJECT		= 61,
	HEADER_GD_UPDATE_LAND		= 62,

	HEADER_GD_MARRIAGE_ADD		= 70,
	HEADER_GD_MARRIAGE_UPDATE	= 71,
	HEADER_GD_MARRIAGE_REMOVE	= 72,

	HEADER_GD_WEDDING_REQUEST	= 73,
	HEADER_GD_WEDDING_READY		= 74,
	HEADER_GD_WEDDING_END		= 75,
#ifdef ENABLE_RONARK_SYSTEM
	HEADER_GD_GUILD_RONARK_KING	= 98,
#endif
	HEADER_GD_AUTH_LOGIN		= 100,
	HEADER_GD_LOGIN_BY_KEY		= 101,
	HEADER_GD_BILLING_EXPIRE	= 104,
	HEADER_GD_BILLING_CHECK		= 106,
	HEADER_GD_MALL_LOAD			= 107,

	HEADER_GD_MYSHOP_PRICELIST_UPDATE	= 108,
	HEADER_GD_MYSHOP_PRICELIST_REQ		= 109,

	HEADER_GD_BLOCK_CHAT				= 110,

	HEADER_GD_RELOAD_ADMIN			= 115,
	HEADER_GD_BREAK_MARRIAGE		= 116,
	HEADER_GD_ELECT_MONARCH			= 117,
	HEADER_GD_CANDIDACY				= 118,
	HEADER_GD_ADD_MONARCH_MONEY		= 119,
	HEADER_GD_TAKE_MONARCH_MONEY	= 120,
	HEADER_GD_COME_TO_VOTE			= 121,
	HEADER_GD_RMCANDIDACY			= 122,
	HEADER_GD_SETMONARCH			= 123,
	HEADER_GD_RMMONARCH				= 124,
	HEADER_GD_DEC_MONARCH_MONEY		= 125,

	HEADER_GD_CHANGE_MONARCH_LORD	= 126,

	HEADER_GD_REQ_CHANGE_GUILD_MASTER	= 129,

	HEADER_GD_REQ_SPARE_ITEM_ID_RANGE	= 130,

	HEADER_GD_UPDATE_HORSE_NAME		= 131,
	HEADER_GD_REQ_HORSE_NAME		= 132,

	HEADER_GD_DC					= 133,

	HEADER_GD_VALID_LOGOUT			= 134,

	HEADER_GD_REQUEST_CHARGE_CASH	= 137,

	HEADER_GD_DELETE_AWARDID		= 138,	// delete gift notify icon

	HEADER_GD_UPDATE_CHANNELSTATUS	= 139,
	HEADER_GD_REQUEST_CHANNELSTATUS	= 140,

	HEADER_GD_SETUP			= 0xff,

	///////////////////////////////////////////////
	HEADER_DG_NOTICE			= 1,

	HEADER_DG_LOGIN_SUCCESS			= 30,
	HEADER_DG_LOGIN_NOT_EXIST		= 31,
	HEADER_DG_LOGIN_WRONG_PASSWD	= 33,
	HEADER_DG_LOGIN_ALREADY			= 34,

	HEADER_DG_PLAYER_LOAD_SUCCESS	= 35,
	HEADER_DG_PLAYER_LOAD_FAILED	= 36,
	HEADER_DG_PLAYER_CREATE_SUCCESS	= 37,
	HEADER_DG_PLAYER_CREATE_ALREADY	= 38,
	HEADER_DG_PLAYER_CREATE_FAILED	= 39,
	HEADER_DG_PLAYER_DELETE_SUCCESS	= 40,
	HEADER_DG_PLAYER_DELETE_FAILED	= 41,

	HEADER_DG_ITEM_LOAD			= 42,

	HEADER_DG_BOOT				= 43,
	HEADER_DG_QUEST_LOAD		= 44,

	HEADER_DG_SAFEBOX_LOAD					= 45,
	HEADER_DG_SAFEBOX_CHANGE_SIZE			= 46,
	HEADER_DG_SAFEBOX_WRONG_PASSWORD		= 47,
	HEADER_DG_SAFEBOX_CHANGE_PASSWORD_ANSWER = 48,

	HEADER_DG_EMPIRE_SELECT		= 49,

	HEADER_DG_AFFECT_LOAD		= 50,
	HEADER_DG_MALL_LOAD			= 51,

	HEADER_DG_DIRECT_ENTER		= 55,

	HEADER_DG_GUILD_SKILL_UPDATE	= 56,
	HEADER_DG_GUILD_SKILL_RECHARGE	= 57,
	HEADER_DG_GUILD_EXP_UPDATE		= 58,

	HEADER_DG_PARTY_CREATE		= 59,
	HEADER_DG_PARTY_DELETE		= 60,
	HEADER_DG_PARTY_ADD			= 61,
	HEADER_DG_PARTY_REMOVE		= 62,
	HEADER_DG_PARTY_STATE_CHANGE	= 63,
	HEADER_DG_PARTY_HEAL_USE		= 64,
	HEADER_DG_PARTY_SET_MEMBER_LEVEL	= 65,

	HEADER_DG_TIME			= 90,
	HEADER_DG_ITEM_ID_RANGE		= 91,

	HEADER_DG_GUILD_ADD_MEMBER		= 92,
	HEADER_DG_GUILD_REMOVE_MEMBER	= 93,
	HEADER_DG_GUILD_CHANGE_GRADE	= 94,
	HEADER_DG_GUILD_CHANGE_MEMBER_DATA	= 95,
	HEADER_DG_GUILD_DISBAND		= 96,
	HEADER_DG_GUILD_WAR			= 97,
	HEADER_DG_GUILD_WAR_SCORE		= 98,
	HEADER_DG_GUILD_TIME_UPDATE		= 99,
	HEADER_DG_GUILD_LOAD		= 100,
	HEADER_DG_GUILD_LADDER		= 101,
	HEADER_DG_GUILD_SKILL_USABLE_CHANGE	= 102,
	HEADER_DG_GUILD_MONEY_CHANGE	= 103,
	HEADER_DG_GUILD_WITHDRAW_MONEY_GIVE	= 104,

	HEADER_DG_SET_EVENT_FLAG		= 105,

	HEADER_DG_GUILD_WAR_RESERVE_ADD	= 106,
	HEADER_DG_GUILD_WAR_RESERVE_DEL	= 107,
	HEADER_DG_GUILD_WAR_BET		= 108,
#ifdef ENABLE_RONARK_SYSTEM
	HEADER_DG_GUILD_RONARK_KING		= 110,
#endif
	HEADER_DG_RELOAD_PROTO		= 120,
	HEADER_DG_CHANGE_NAME		= 121,

	HEADER_DG_AUTH_LOGIN		= 122,

	HEADER_DG_CHANGE_EMPIRE_PRIV	= 124,
	HEADER_DG_CHANGE_GUILD_PRIV		= 125,

	HEADER_DG_MONEY_LOG			= 126,

	HEADER_DG_CHANGE_CHARACTER_PRIV	= 127,

	HEADER_DG_BILLING_REPAIR		= 128,
	HEADER_DG_BILLING_EXPIRE		= 129,
	HEADER_DG_BILLING_LOGIN		= 130,
	HEADER_DG_BILLING_CHECK		= 132,

	HEADER_DG_CREATE_OBJECT		= 140,
	HEADER_DG_DELETE_OBJECT		= 141,
	HEADER_DG_UPDATE_LAND		= 142,

	HEADER_DG_MARRIAGE_ADD		= 150,
	HEADER_DG_MARRIAGE_UPDATE		= 151,
	HEADER_DG_MARRIAGE_REMOVE		= 152,

	HEADER_DG_WEDDING_REQUEST		= 153,
	HEADER_DG_WEDDING_READY		= 154,
	HEADER_DG_WEDDING_START		= 155,
	HEADER_DG_WEDDING_END		= 156,

	HEADER_DG_MYSHOP_PRICELIST_RES	= 157,
	HEADER_DG_RELOAD_ADMIN = 158,
	HEADER_DG_BREAK_MARRIAGE = 159,
	HEADER_DG_ELECT_MONARCH			= 160,
	HEADER_DG_CANDIDACY				= 161,
	HEADER_DG_ADD_MONARCH_MONEY		= 162,
	HEADER_DG_TAKE_MONARCH_MONEY	= 163,
	HEADER_DG_COME_TO_VOTE			= 164,
	HEADER_DG_RMCANDIDACY			= 165,
	HEADER_DG_SETMONARCH			= 166,
	HEADER_DG_RMMONARCH			= 167,
	HEADER_DG_DEC_MONARCH_MONEY = 168,

	HEADER_DG_CHANGE_MONARCH_LORD_ACK = 169,
	HEADER_DG_UPDATE_MONARCH_INFO	= 170,

	HEADER_DG_ACK_CHANGE_GUILD_MASTER = 173,

	HEADER_DG_ACK_SPARE_ITEM_ID_RANGE = 174,

	HEADER_DG_UPDATE_HORSE_NAME 	= 175,
	HEADER_DG_ACK_HORSE_NAME		= 176,

	HEADER_DG_NEED_LOGIN_LOG		= 177,

	HEADER_DG_RESULT_CHARGE_CASH	= 179,
	HEADER_DG_ITEMAWARD_INFORMER	= 180,	//gift notify
	HEADER_DG_RESPOND_CHANNELSTATUS	= 181,

	HEADER_DG_MAP_LOCATIONS			= 0xfe,
	HEADER_DG_P2P					= 0xff,
};

/* ----------------------------------------------
 * table
 * ----------------------------------------------
 */

/* game Server -> DB Server */
#pragma pack(1)
enum ERequestChargeType
{
	ERequestCharge_Cash = 0,
	ERequestCharge_Mileage,
};

typedef struct SRequestChargeCash
{
	DWORD		dwAID;		// id(primary key) - Account Table
	DWORD		dwAmount;
	ERequestChargeType	eChargeType;
} TRequestChargeCash;

typedef struct SSimplePlayer
{
	DWORD		dwID;
	char		szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE		byJob;
	BYTE		byLevel;
	DWORD		dwPlayMinutes;
	BYTE		byST, byHT, byDX, byIQ;
	DWORD		wMainPart; // @fixme502
	BYTE		bChangeName;
	DWORD		wHairPart; // @fixme502
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	DWORD		wAccePart;
#endif
#ifdef ENABLE_WINGS_NEW
	DWORD		wWingPart;
#endif
	BYTE		bDummy[4];
	long		x, y;
	long		lAddr;
	WORD		wPort;
	BYTE		skill_group;
} TSimplePlayer;

typedef struct SAccountTable
{
	DWORD		id;
	char		login[LOGIN_MAX_LEN + 1];
	char		passwd[PASSWD_MAX_LEN + 1];
	char		social_id[SOCIAL_ID_MAX_LEN + 1];
	char		status[ACCOUNT_STATUS_MAX_LEN + 1];
	BYTE		bEmpire;
	TSimplePlayer	players[PLAYER_PER_ACCOUNT];
} TAccountTable;

typedef struct SPacketDGCreateSuccess
{
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketDGCreateSuccess;

typedef struct TPlayerItemAttribute
{
	BYTE	bType;
	short	sValue;
} TPlayerItemAttribute;

typedef struct SPlayerItem
{
	DWORD	id;
	BYTE	window;
	WORD	pos;
	DWORD	count;

	DWORD	vnum;
	long	alSockets[ITEM_SOCKET_MAX_NUM];

	TPlayerItemAttribute    aAttr[ITEM_ATTRIBUTE_MAX_NUM];

	DWORD	owner;
} TPlayerItem;

typedef struct SQuickslot
{
	BYTE	type;
	BYTE	pos;
} TQuickslot;

typedef struct SPlayerSkill
{
	BYTE	bMasterType;
	BYTE	bLevel;
	time_t	tNextRead;
} TPlayerSkill;

struct	THorseInfo
{
	BYTE	bLevel;
	BYTE	bRiding;
	short	sStamina;
	short	sHealth;
	DWORD	dwHorseHealthDropTime;
};

typedef struct SPlayerTable
{
	DWORD	id;

	char	name[CHARACTER_NAME_MAX_LEN + 1];
	char	ip[IP_ADDRESS_LENGTH + 1];

	WORD	job;
	BYTE	voice;

	BYTE	level;
	BYTE	level_step;
	short	st, ht, dx, iq;

	DWORD	exp;
	long long	gold;

	BYTE	dir;
	INT		x, y, z;
	INT		lMapIndex;

	long	lExitX, lExitY;
	long	lExitMapIndex;

	// @fixme400
	int		hp;
	int		sp;

	short	sRandomHP;
	short	sRandomSP;

	int         playtime;

	short	stat_point;
	short	skill_point;
	short	sub_skill_point;
	short	horse_skill_point;

	TPlayerSkill skills[SKILL_MAX_NUM];

	TQuickslot  quickslot[QUICKSLOT_MAX_NUM];

	BYTE	part_base;
	DWORD	parts[PART_MAX_NUM]; // @fixme502

	short	stamina;

	BYTE	skill_group;
	long	lAlignment;

	short	stat_reset_count;

	THorseInfo	horse;

	DWORD	logoff_interval;

	int		aiPremiumTimes[PREMIUM_MAX_NUM];
#ifdef ENABLE_PVP_RANKING
	int rank_Kill;
	int rank_Dead;
#endif
} TPlayerTable;

typedef struct SMobSkillLevel
{
	DWORD	dwVnum;
	BYTE	bLevel;
} TMobSkillLevel;

typedef struct SEntityTable
{
	DWORD dwVnum;
} TEntityTable;

typedef struct SMobTable : public SEntityTable
{
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	char	szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

	BYTE	bType;			// Monster, NPC
	BYTE	bRank;			// PAWN, KNIGHT, KING
	BYTE	bBattleType;		// MELEE, etc..
	BYTE	bLevel;			// Level
	BYTE	bSize;

	DWORD	dwGoldMin;
	DWORD	dwGoldMax;
	DWORD	dwExp;
#ifdef on
	int64_t	dwMaxHP;
#else
	DWORD	dwMaxHP;
#endif
	BYTE	bRegenCycle;
	BYTE	bRegenPercent;
	WORD	wDef;

	DWORD	dwAIFlag;
	DWORD	dwRaceFlag;
	DWORD	dwImmuneFlag;

	BYTE	bStr, bDex, bCon, bInt;
	DWORD	dwDamageRange[2];

	short	sAttackSpeed;
	short	sMovingSpeed;
	BYTE	bAggresiveHPPct;
	WORD	wAggressiveSight;
	WORD	wAttackRange;

	char	cEnchants[MOB_ENCHANTS_MAX_NUM];
	char	cResists[MOB_RESISTS_MAX_NUM];

	DWORD	dwResurrectionVnum;
	DWORD	dwDropItemVnum;

	BYTE	bMountCapacity;
	BYTE	bOnClickType;

	BYTE	bEmpire;
	char	szFolder[64 + 1];

	float	fDamMultiply;

	DWORD	dwSummonVnum;
	DWORD	dwDrainSP;
	DWORD	dwMobColor;
	DWORD	dwPolymorphItemVnum;

	TMobSkillLevel Skills[MOB_SKILL_MAX_NUM];

	BYTE	bBerserkPoint;
	BYTE	bStoneSkinPoint;
	BYTE	bGodSpeedPoint;
	BYTE	bDeathBlowPoint;
	BYTE	bRevivePoint;
} TMobTable;

typedef struct SSkillTable
{
	DWORD	dwVnum;
	char	szName[32 + 1];
	BYTE	bType;
	BYTE	bMaxLevel;
	DWORD	dwSplashRange;

	char	szPointOn[64];
	char	szPointPoly[100 + 1];
	char	szSPCostPoly[100 + 1];
	char	szDurationPoly[100 + 1];
	char	szDurationSPCostPoly[100 + 1];
	char	szCooldownPoly[100 + 1];
	char	szMasterBonusPoly[100 + 1];
	//char	szAttackGradePoly[100 + 1];
	char	szGrandMasterAddSPCostPoly[100 + 1];
	DWORD	dwFlag;
	DWORD	dwAffectFlag;

	// Data for secondary skill
	char 	szPointOn2[64];
	char 	szPointPoly2[100 + 1];
	char 	szDurationPoly2[100 + 1];
	DWORD 	dwAffectFlag2;

	// Data for grand master point
	char 	szPointOn3[64];
	char 	szPointPoly3[100 + 1];
	char 	szDurationPoly3[100 + 1];

	BYTE	bLevelStep;
	BYTE	bLevelLimit;
	DWORD	preSkillVnum;
	BYTE	preSkillLevel;

	long	lMaxHit;
	char	szSplashAroundDamageAdjustPoly[100 + 1];

	BYTE	bSkillAttrType;

	DWORD	dwTargetRange;
} TSkillTable;

typedef struct SShopItemTable
{
	DWORD		vnum;
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	DWORD		count;
#else
	BYTE		count;
#endif

    TItemPos	pos;
	long long	price;
	BYTE		display_pos;
} TShopItemTable;

typedef struct SShopTable
{
	DWORD		dwVnum;
	DWORD		dwNPCVnum;

#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	DWORD		byItemCount;
#else
	BYTE		byItemCount;
#endif
	TShopItemTable	items[SHOP_HOST_ITEM_MAX_NUM];
} TShopTable;

#define QUEST_NAME_MAX_LEN	32
#define QUEST_STATE_MAX_LEN	64

typedef struct SQuestTable
{
	DWORD		dwPID;
	char		szName[QUEST_NAME_MAX_LEN + 1];
	char		szState[QUEST_STATE_MAX_LEN + 1];
	long		lValue;
} TQuestTable;

typedef struct SItemLimit
{
	BYTE	bType;
	long	lValue;
} TItemLimit;

typedef struct SItemApply
{
	BYTE	bType;
	long	lValue;
} TItemApply;

typedef struct SItemTable : public SEntityTable
{
	DWORD		dwVnumRange;
	char        szName[ITEM_NAME_MAX_LEN + 1];
	char	szLocaleName[ITEM_NAME_MAX_LEN + 1];
	BYTE	bType;
	BYTE	bSubType;

	BYTE        bWeight;
	BYTE	bSize;

	DWORD	dwAntiFlags;
	DWORD	dwFlags;
	DWORD	dwWearFlags;
	DWORD	dwImmuneFlag;

	long long	dwGold;
	long long	dwShopBuyPrice;

	TItemLimit	aLimits[ITEM_LIMIT_MAX_NUM];
	TItemApply	aApplies[ITEM_APPLY_MAX_NUM];
	long        alValues[ITEM_VALUES_MAX_NUM];
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	DWORD	dwRefinedVnum;
	WORD	wRefineSet;
	BYTE	bAlterToMagicItemPct;
	BYTE	bSpecular;
	BYTE	bGainSocketPct;

	short int	sAddonType;

	char		cLimitRealTimeFirstUseIndex;
	char		cLimitTimerBasedOnWearIndex;

#ifdef SANDIK_AC
	BYTE	GetSize()		{ return bSize; }
	BYTE	GetType()		{ return bType; }
	BYTE	GetSubType()	{ return bSubType; }

	DWORD	GetVnum()		{ return dwVnumRange; }
	bool	IsStackable()	{ return (dwFlags & ITEM_FLAG_STACKABLE) ? true : false; }

	bool IsUpgradeItem()
	{
		switch (GetVnum())
		{
			case 30003:
			case 30004:
			case 30005:
			case 30006:
			case 30007:
			case 30008:
			case 30009:
			case 30010:
			case 30011:
			case 30014:
			case 30015:
			case 30016:
			case 30017:
			case 30018:
			case 30019:
			case 30021:
			case 30022:
			case 30023:
			case 30025:
			case 30027:
			case 30028:
			case 30030:
			case 30031:
			case 30032:
			case 30033:
			case 30034:
			case 30035:
			case 30037:
			case 30038:
			case 30039:
			case 30040:
			case 30041:
			case 30042:
			case 30045:
			case 30046:
			case 30047:
			case 30048:
			case 30049:
			case 30050:
			case 30051:
			case 30052:
			case 30053:
			case 30055:
			case 30056:
			case 30057:
			case 30058:
			case 30059:
			case 30060:
			case 30061:
			case 30067:
			case 30069:
			case 30070:
			case 30071:
			case 30072:
			case 30073:
			case 30074:
			case 30075:
			case 30076:
			case 30077:
			case 30078:
			case 30079:
			case 30080:
			case 30081:
			case 30082:
			case 30083:
			case 30084:
			case 30085:
			case 30086:
			case 30087:
			case 30088:
			case 30089:
			case 30090:
			case 30091:
			case 30092:
			case 30192:
			case 30193:
			case 30194:
			case 30195:
			case 30196:
			case 30197:
			case 30198:
			case 30199:
			case 30500:
			case 30501:
			case 30502:
			case 30503:
			case 30504:
			case 30505:
			case 30506:
			case 30507:
			case 30508:
			case 30509:
			case 30510:
			case 30511:
			case 30512:
			case 30513:
			case 30514:
			case 30515:
			case 30516:
			case 30517:
			case 30518:
			case 30519:
			case 30520:
			case 30521:
			case 30522:
			case 30523:
			case 30524:
			case 30525:
			case 30600:
			case 30601:
			case 30602:
			case 30603:
			case 30604:
			case 30605:
			case 30606:
			case 30607:
			case 30608:
			case 30609:
			case 30610:
			case 30611:
			case 30612:
			case 30614:
			case 30615:
			case 30616:
			case 30617:
			case 30618:
			case 30619:
			case 30620:
			case 30621:
			case 30622:
			case 30623:
			case 30624:
			case 30625:
			case 30626:
			case 30627:
			case 30628:
			case 30629:
			case 70251:
			case 70252:
			case 70253:
			case 70254:
			case 71123:
			case 71129:
			case 78741:
			case 78742:
			case 78743:
			case 78744:
			case 78745:
			case 78746:
			case 27992:
			case 27993:
			case 27994:
			case 78712:
			case 78713:
			case 78714:
			case 78716:
			case 78723:
			case 78725:
			case 78726:
			case 78727:
			case 78728:
			case 78729:
			case 78730:
			case 11120:
			case 11121:
			case 11122:
			case 11123:
			case 11124:
			case 11125:
			case 31108:
			case 31109:
			case 31110:
			case 31111:
			case 31112:
			case 31113:
			case 31114:
			case 31115:
			case 31116:
			case 31117:
			case 31118:
			case 31119:
			case 31120:
			case 31121:
			case 31122:
			case 31123:
			case 31124:
			case 31125:
			case 31126:
			case 31127:
			case 31128:
			case 31129:
			case 31130:
			case 31131:
			case 31132:
			case 31133:
			case 31134:
			case 31135:
			case 31136:
			case 31137:
			case 31138:
			case 31144:
			case 31145:
			case 31146:
			case 31147:
			case 31148:
			case 31149:
			case 31196:
			case 31197:
			case 31198:
			case 31199:
			case 31200:
			case 31201:
			case 31202:
			case 31203:
			case 31204:
			case 31205:
			case 31206:
			case 31207:
			case 31208:
			case 31209:
			case 31210:
			case 31211:
			case 31212:
			case 31213:
			case 78724:
				return true;
		}
		
		return false;
	}

	bool IsBook()
	{
		switch (GetVnum())
		{
			case 31001:
			case 31002:
			case 31003:
			case 31004:
			case 31005:
			case 31006:
			case 31007:
			case 31008:
			case 31009:
			case 31010:
			case 31011:
			case 31012:
			case 31013:
			case 31015:
			case 31016:
			case 31017:
			case 31018:
			case 31019:
			case 31020:
			case 31022:
			case 31023:
			case 31024:
			case 31025:
			case 31026:
			case 31027:
			case 31028:
			case 31031:
			case 31032:
			case 31033:
			case 31034:
			case 31035:
			case 31036:
			case 31037:
			case 31038:
			case 31039:
			case 31040:
			case 31041:
			case 31042:
			case 31043:
			case 31044:
			case 31045:
			case 31046:
			case 31047:
			case 31048:
			case 31049:
			case 31050:
			case 31051:
			case 31053:
			case 31054:
			case 31055:
			case 31056:
			case 31058:
			case 31059:
			case 31060:
			case 31061:
			case 31062:
			case 31063:
			case 31064:
			case 31065:
			case 31066:
			case 31067:
			case 31069:
			case 31070:
			case 31071:
			case 31072:
			case 31073:
			case 31074:
			case 31075:
			case 31076:
			case 31077:
			case 31078:
			case 31079:
			case 31080:
			case 31081:
			case 31082:
			case 31083:
			case 31084:
			case 31085:
			case 31086:
			case 31088:
			case 31089:
			case 31090:
			case 31091:
			case 31092:
			case 31093:
			case 31094:
			case 31095:
			case 31096:
			case 31097:
			case 31098:
			case 31099:
			case 31100:
			case 35000:
			case 35001:
			case 50039:
			case 50040:
			case 70016:
			case 70017:
			case 70018:
			case 70019:
			case 50028:
			case 50029:
			case 50030:
			case 55101:
			case 55102:
			case 55103:
			case 55104:
			case 55105:
			case 55106:
			case 55107:
			case 55108:
			case 55109:
			case 55110:
			case 55200:
			case 55201:
			case 55202:
			case 55203:
			case 55204:
			case 55205:
			case 55206:
			case 55207:
			case 55208:
			case 55209:
			case 55210:
			case 55211:
			case 55212:
			case 55213:
			case 55214:
			case 55215:
			case 51001:
			case 50111:
			case 50300:
			case 33029:
			case 33030:
			case 85000:
			case 50902:
			case 50903:
			case 50904:
			case 50516:
			case 50517:
			case 50518:
			case 50519:
			case 50520:
			case 50521:
			case 50522:
			case 50523:
			case 50524:
			case 50560:
			case 50561:
			case 50562:
			case 50563:
			case 50564:
			case 50565:
			case 50566:
			case 50567:
			case 50568:
			case 90014:
			case 50037:
			case 27990:
			case 50038:
			case 30630:
			case 27799:
				return true;
		}
		
		return false;
	}

	bool IsStone()
	{
		return (GetType() == ITEM_METIN && GetSubType() == METIN_NORMAL);
	}

	bool IsChestItemVnum(DWORD vnum)
	{
		switch (vnum)
		{
			case 30300:
			case 50287:
			case 50288:
			case 50289:
			case 50290:
			case 50926:
			case 50927:
				return true;
		}

		return false;
	}

	bool IsGeneralItem()
	{
		if (GetType() == ITEM_GIFTBOX)
			return true;

		if (IsChestItemVnum(GetVnum()))
			return true;

		return false;
	}

	bool IsDragonSoul()
	{
		return (GetType() == ITEM_DS);
	}
#endif

} TItemTable;

struct TItemAttrTable
{
	TItemAttrTable() :
		dwApplyIndex(0),
		dwProb(0)
	{
		szApply[0] = 0;
		memset(&lValues, 0, sizeof(lValues));
		memset(&bMaxLevelBySet, 0, sizeof(bMaxLevelBySet));
	}

	char    szApply[APPLY_NAME_MAX_LEN + 1];
	DWORD   dwApplyIndex;
	DWORD   dwProb;
	long    lValues[ITEM_ATTRIBUTE_MAX_LEVEL];
	BYTE    bMaxLevelBySet[ATTRIBUTE_SET_MAX_NUM];
};

typedef struct SConnectTable
{
	char	login[LOGIN_MAX_LEN + 1];
	IDENT	ident;
} TConnectTable;

typedef struct SLoginPacket
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TLoginPacket;

typedef struct SPlayerLoadPacket
{
	DWORD	account_id;
	DWORD	player_id;
	BYTE	account_index;
} TPlayerLoadPacket;

typedef struct SPlayerCreatePacket
{
	char		login[LOGIN_MAX_LEN + 1];
	char		passwd[PASSWD_MAX_LEN + 1];
	DWORD		account_id;
	BYTE		account_index;
	TPlayerTable	player_table;
} TPlayerCreatePacket;

typedef struct SPlayerDeletePacket
{
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	player_id;
	BYTE	account_index;
	//char	name[CHARACTER_NAME_MAX_LEN + 1];
	char	private_code[8];
} TPlayerDeletePacket;

typedef struct SLogoutPacket
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TLogoutPacket;

typedef struct SPlayerCountPacket
{
	DWORD	dwCount;
} TPlayerCountPacket;

#define SAFEBOX_MAX_NUM			135
#define SAFEBOX_PASSWORD_MAX_LEN	6

typedef struct SSafeboxTable
{
	DWORD	dwID;
	BYTE	bSize;
	DWORD	dwGold;
	WORD	wItemCount;
} TSafeboxTable;

typedef struct SSafeboxChangeSizePacket
{
	DWORD	dwID;
	BYTE	bSize;
} TSafeboxChangeSizePacket;

typedef struct SSafeboxLoadPacket
{
	DWORD	dwID;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
} TSafeboxLoadPacket;

typedef struct SSafeboxChangePasswordPacket
{
	DWORD	dwID;
	char	szOldPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
	char	szNewPassword[SAFEBOX_PASSWORD_MAX_LEN + 1];
} TSafeboxChangePasswordPacket;

typedef struct SSafeboxChangePasswordPacketAnswer
{
	BYTE	flag;
} TSafeboxChangePasswordPacketAnswer;

typedef struct SEmpireSelectPacket
{
	DWORD	dwAccountID;
	BYTE	bEmpire;
} TEmpireSelectPacket;

typedef struct SPacketGDSetup
{
	char	szPublicIP[16];	// Public IP which listen to users
	BYTE	bChannel;
	WORD	wListenPort;
	WORD	wP2PPort;
	long	alMaps[MAP_ALLOW_LIMIT];
	DWORD	dwLoginCount;
	BYTE	bAuthServer;
} TPacketGDSetup;

typedef struct SPacketDGMapLocations
{
	BYTE	bCount;
} TPacketDGMapLocations;

typedef struct SMapLocation
{
	long	alMaps[MAP_ALLOW_LIMIT];
	char	szHost[MAX_HOST_LENGTH + 1];
	WORD	wPort;
#ifdef ENABLE_MOVE_CHANNEL
	BYTE	channel;
#endif
} TMapLocation;

typedef struct SPacketDGP2P
{
	char	szHost[MAX_HOST_LENGTH + 1];
	WORD	wPort;
	BYTE	bChannel;
} TPacketDGP2P;

typedef struct SPacketGDDirectEnter
{
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	BYTE	index;
} TPacketGDDirectEnter;

typedef struct SPacketDGDirectEnter
{
	TAccountTable accountTable;
	TPlayerTable playerTable;
} TPacketDGDirectEnter;

typedef struct SPacketGuildSkillUpdate
{
	DWORD guild_id;
	int amount;
	BYTE skill_levels[12];
	BYTE skill_point;
	BYTE save;
} TPacketGuildSkillUpdate;

typedef struct SPacketGuildExpUpdate
{
	DWORD guild_id;
	int amount;
} TPacketGuildExpUpdate;

typedef struct SPacketGuildChangeMemberData
{
	DWORD guild_id;
	DWORD pid;
	DWORD offer;
	BYTE level;
	BYTE grade;
} TPacketGuildChangeMemberData;

typedef struct SPacketDGLoginAlready
{
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketDGLoginAlready;

typedef struct TPacketAffectElement
{
	DWORD	dwType;
	BYTE	bApplyOn;
	long	lApplyValue;
	DWORD	dwFlag;
	long	lDuration;
	long	lSPCost;
} TPacketAffectElement;

typedef struct SPacketGDAddAffect
{
	DWORD			dwPID;
	TPacketAffectElement	elem;
} TPacketGDAddAffect;

typedef struct SPacketGDRemoveAffect
{
	DWORD	dwPID;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGDRemoveAffect;

typedef struct SPacketPartyCreate
{
	DWORD	dwLeaderPID;
} TPacketPartyCreate;

typedef struct SPacketPartyDelete
{
	DWORD	dwLeaderPID;
} TPacketPartyDelete;

typedef struct SPacketPartyAdd
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bState;
} TPacketPartyAdd;

typedef struct SPacketPartyRemove
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
} TPacketPartyRemove;

typedef struct SPacketPartyStateChange
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bRole;
	BYTE	bFlag;
} TPacketPartyStateChange;

typedef struct SPacketPartySetMemberLevel
{
	DWORD	dwLeaderPID;
	DWORD	dwPID;
	BYTE	bLevel;
} TPacketPartySetMemberLevel;

typedef struct SPacketGDBoot
{
    DWORD	dwItemIDRange[2];
	char	szIP[16];
} TPacketGDBoot;

typedef struct SPacketGuild
{
	DWORD	dwGuild;
	DWORD	dwInfo;
} TPacketGuild;

typedef struct SPacketGDGuildAddMember
{
	DWORD	dwPID;
	DWORD	dwGuild;
	BYTE	bGrade;
} TPacketGDGuildAddMember;

typedef struct SPacketDGGuildMember
{
	DWORD	dwPID;
	DWORD	dwGuild;
	BYTE	bGrade;
	BYTE	isGeneral;
	BYTE	bJob;
	BYTE	bLevel;
	DWORD	dwOffer;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketDGGuildMember;

typedef struct SPacketGuildWar
{
	BYTE	bType;
	BYTE	bWar;
	DWORD	dwGuildFrom;
	DWORD	dwGuildTo;
	long	lWarPrice;
	long	lInitialScore;
} TPacketGuildWar;

typedef struct SPacketGuildWarScore
{
	DWORD dwGuildGainPoint;
	DWORD dwGuildOpponent;
	long lScore;
	long lBetScore;
} TPacketGuildWarScore;

typedef struct SRefineMaterial
{
	DWORD vnum;
	int count;
} TRefineMaterial;

typedef struct SRefineTable
{
	//DWORD src_vnum;
	//DWORD result_vnum;
	DWORD id;
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	DWORD material_count;
#else
	BYTE material_count;
#endif
	long long cost;
	int prob;
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TRefineTable;

typedef struct SBanwordTable
{
	char szWord[BANWORD_MAX_LEN + 1];
} TBanwordTable;

typedef struct SPacketGDChangeName
{
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGDChangeName;

typedef struct SPacketDGChangeName
{
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketDGChangeName;

typedef struct SPacketGuildLadder
{
	DWORD dwGuild;
	long lLadderPoint;
	long lWin;
	long lDraw;
	long lLoss;
} TPacketGuildLadder;

typedef struct SPacketGuildLadderPoint
{
	DWORD dwGuild;
	long lChange;
} TPacketGuildLadderPoint;

typedef struct SPacketGuildUseSkill
{
	DWORD dwGuild;
	DWORD dwSkillVnum;
	DWORD dwCooltime;
} TPacketGuildUseSkill;

typedef struct SPacketGuildSkillUsableChange
{
	DWORD dwGuild;
	DWORD dwSkillVnum;
	BYTE bUsable;
} TPacketGuildSkillUsableChange;

typedef struct SPacketGDLoginKey
{
	DWORD dwAccountID;
	DWORD dwLoginKey;
} TPacketGDLoginKey;

typedef struct SPacketGDAuthLogin
{
	DWORD	dwID;
	DWORD	dwLoginKey;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szSocialID[SOCIAL_ID_MAX_LEN + 1];
	DWORD	adwClientKey[4];
	BYTE	bBillType;
	DWORD	dwBillID;
	int		iPremiumTimes[PREMIUM_MAX_NUM];
} TPacketGDAuthLogin;

typedef struct SPacketGDLoginByKey
{
	char	szLogin[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
	char	szIP[MAX_HOST_LENGTH + 1];
} TPacketGDLoginByKey;

typedef struct SPacketGiveGuildPriv
{
	BYTE type;
	int value;
	DWORD guild_id;
	time_t duration_sec;
} TPacketGiveGuildPriv;
typedef struct SPacketGiveEmpirePriv
{
	BYTE type;
	int value;
	BYTE empire;
	time_t duration_sec;
} TPacketGiveEmpirePriv;
typedef struct SPacketGiveCharacterPriv
{
	BYTE type;
	int value;
	DWORD pid;
} TPacketGiveCharacterPriv;
typedef struct SPacketRemoveGuildPriv
{
	BYTE type;
	DWORD guild_id;
} TPacketRemoveGuildPriv;
typedef struct SPacketRemoveEmpirePriv
{
	BYTE type;
	BYTE empire;
} TPacketRemoveEmpirePriv;

typedef struct SPacketDGChangeCharacterPriv
{
	BYTE type;
	int value;
	DWORD pid;
	BYTE bLog;
} TPacketDGChangeCharacterPriv;

typedef struct SPacketDGChangeGuildPriv
{
	BYTE type;
	int value;
	DWORD guild_id;
	BYTE bLog;
	time_t end_time_sec;
} TPacketDGChangeGuildPriv;

typedef struct SPacketDGChangeEmpirePriv
{
	BYTE type;
	int value;
	BYTE empire;
	BYTE bLog;
	time_t end_time_sec;
} TPacketDGChangeEmpirePriv;

typedef struct SPacketMoneyLog
{
	BYTE type;
	DWORD vnum;
	long long	gold;
} TPacketMoneyLog;

typedef struct SPacketGDGuildMoney
{
	DWORD dwGuild;
	INT iGold;
} TPacketGDGuildMoney;

typedef struct SPacketDGGuildMoneyChange
{
	DWORD dwGuild;
	INT iTotalGold;
} TPacketDGGuildMoneyChange;

typedef struct SPacketDGGuildMoneyWithdraw
{
	DWORD dwGuild;
	INT iChangeGold;
} TPacketDGGuildMoneyWithdraw;

typedef struct SPacketGDGuildMoneyWithdrawGiveReply
{
	DWORD dwGuild;
	INT iChangeGold;
	BYTE bGiveSuccess;
} TPacketGDGuildMoneyWithdrawGiveReply;

typedef struct SPacketSetEventFlag
{
	char	szFlagName[EVENT_FLAG_NAME_MAX_LEN + 1];
	long	lValue;
} TPacketSetEventFlag;

typedef struct SPacketBillingLogin
{
	DWORD	dwLoginKey;
	BYTE	bLogin;
} TPacketBillingLogin;

typedef struct SPacketBillingRepair
{
	DWORD	dwLoginKey;
	char	szLogin[LOGIN_MAX_LEN + 1];
	char	szHost[MAX_HOST_LENGTH + 1];
} TPacketBillingRepair;

typedef struct SPacketBillingExpire
{
	char	szLogin[LOGIN_MAX_LEN + 1];
	BYTE	bBillType;
	DWORD	dwRemainSeconds;
} TPacketBillingExpire;

typedef struct SPacketLoginOnSetup
{
	DWORD   dwID;
	char    szLogin[LOGIN_MAX_LEN + 1];
	char    szSocialID[SOCIAL_ID_MAX_LEN + 1];
	char    szHost[MAX_HOST_LENGTH + 1];
	DWORD   dwLoginKey;
	DWORD   adwClientKey[4];
} TPacketLoginOnSetup;

typedef struct SPacketGDCreateObject
{
	DWORD	dwVnum;
	DWORD	dwLandID;
	INT		lMapIndex;
	INT	 	x, y;
	float	xRot;
	float	yRot;
	float	zRot;
} TPacketGDCreateObject;

typedef struct SGuildReserve
{
	DWORD       dwID;
	DWORD       dwGuildFrom;
	DWORD       dwGuildTo;
	DWORD       dwTime;
	BYTE        bType;
	long        lWarPrice;
	long        lInitialScore;
	bool        bStarted;
	DWORD	dwBetFrom;
	DWORD	dwBetTo;
	long	lPowerFrom;
	long	lPowerTo;
	long	lHandicap;
} TGuildWarReserve;

typedef struct SPacketGDGuildWarBet
{
	DWORD	dwWarID;
	char	szLogin[LOGIN_MAX_LEN + 1];
	DWORD	dwGold;
	DWORD	dwGuild;
} TPacketGDGuildWarBet;

// Marriage

typedef struct SPacketMarriageAdd
{
	DWORD dwPID1;
	DWORD dwPID2;
	time_t tMarryTime;
	char szName1[CHARACTER_NAME_MAX_LEN + 1];
	char szName2[CHARACTER_NAME_MAX_LEN + 1];
} TPacketMarriageAdd;

typedef struct SPacketMarriageUpdate
{
	DWORD dwPID1;
	DWORD dwPID2;
	INT  iLovePoint;
	BYTE  byMarried;
} TPacketMarriageUpdate;

typedef struct SPacketMarriageRemove
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketMarriageRemove;

typedef struct SPacketWeddingRequest
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingRequest;

typedef struct SPacketWeddingReady
{
	DWORD dwPID1;
	DWORD dwPID2;
	DWORD dwMapIndex;
} TPacketWeddingReady;

typedef struct SPacketWeddingStart
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingStart;

typedef struct SPacketWeddingEnd
{
	DWORD dwPID1;
	DWORD dwPID2;
} TPacketWeddingEnd;

typedef struct SPacketMyshopPricelistHeader
{
	DWORD	dwOwnerID;
	BYTE	byCount;
} TPacketMyshopPricelistHeader;

typedef struct SItemPriceInfo
{
	DWORD	dwVnum;
	long long	dwPrice;
} TItemPriceInfo;

typedef struct SItemPriceListTable
{
	DWORD	dwOwnerID;
	BYTE	byCount;

	TItemPriceInfo	aPriceInfo[SHOP_PRICELIST_MAX_NUM];
} TItemPriceListTable;

typedef struct SPacketBlockChat
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lDuration;
} TPacketBlockChat;

//ADMIN_MANAGER
typedef struct TAdminInfo
{
	int m_ID;
	char m_szAccount[32];
	char m_szName[32];
	char m_szContactIP[16];
	char m_szServerIP[16];
	int m_Authority;
} tAdminInfo;
//END_ADMIN_MANAGER

//BOOT_LOCALIZATION
struct tLocale
{
	char szValue[32];
	char szKey[32];
};
//BOOT_LOCALIZATION

//RELOAD_ADMIN
typedef struct SPacketReloadAdmin
{
	char szIP[16];
} TPacketReloadAdmin;
//END_RELOAD_ADMIN

typedef struct TMonarchInfo
{
	DWORD pid[4];
	int64_t money[4];
	char name[4][32];
	char date[4][32];
} MonarchInfo;

typedef struct TMonarchElectionInfo
{
	DWORD pid;
	DWORD selectedpid;
	char date[32];
} MonarchElectionInfo;

typedef struct tMonarchCandidacy
{
	DWORD pid;
	char name[32];
	char date[32];
} MonarchCandidacy;

typedef struct tChangeMonarchLord
{
	BYTE bEmpire;
	DWORD dwPID;
} TPacketChangeMonarchLord;

typedef struct tChangeMonarchLordACK
{
	BYTE bEmpire;
	DWORD dwPID;
	char szName[32];
	char szDate[32];
} TPacketChangeMonarchLordACK;

typedef struct tChangeGuildMaster
{
	DWORD dwGuildID;
	DWORD idFrom;
	DWORD idTo;
} TPacketChangeGuildMaster;

typedef struct tItemIDRange
{
	DWORD dwMin;
	DWORD dwMax;
	DWORD dwUsableItemIDMin;
} TItemIDRangeTable;

typedef struct tUpdateHorseName
{
	DWORD dwPlayerID;
	char szHorseName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketUpdateHorseName;

typedef struct tDC
{
	char	login[LOGIN_MAX_LEN + 1];
} TPacketDC;

typedef struct tNeedLoginLogInfo
{
	DWORD dwPlayerID;
} TPacketNeedLoginLogInfo;

typedef struct tItemAwardInformer
{
	char	login[LOGIN_MAX_LEN + 1];
	char	command[20];
	unsigned int vnum;
} TPacketItemAwardInfromer;

typedef struct tDeleteAwardID
{
	DWORD dwID;
} TPacketDeleteAwardID;

typedef struct SChannelStatus
{
	WORD nPort; // backward change for client @fixme024
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_PVP_RANKING
typedef struct SPvPRankInfo
{
	int	iDead;
	int	VID;
} TPvPRankInfo;
#endif
#ifdef ENABLE_RONARK_SYSTEM
typedef struct SPacketGDRonarkKing
{
	DWORD dwGuild;
	bool ronark_king;
} TPacketGDRonarkKing;

typedef struct SPacketDGGuildRonarkKing
{
	DWORD dwGuild;
	bool ronark_king;
} TPacketDGGuildRonarkKing;
#endif
#pragma pack()
#endif
// vaffanculo a chi t'e morto martysama
