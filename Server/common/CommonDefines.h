#ifndef __INC_METIN2_COMMON_DEFINES_H__
#define __INC_METIN2_COMMON_DEFINES_H__
#pragma once
//////////////////////////////////////////////////////////////////////////
// ### Standard Features ###
#define _IMPROVED_PACKET_ENCRYPTION_
#ifdef _IMPROVED_PACKET_ENCRYPTION_
#define USE_IMPROVED_PACKET_DECRYPTED_BUFFER
#endif
#define __UDP_BLOCK__
//#define ENABLE_QUEST_CATEGORY

// ### END Standard Features ###
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ### New Features ###
#define ENABLE_NO_MOUNT_CHECK
#define ENABLE_D_NJGUILD
#define ENABLE_FULL_NOTICE
#define ENABLE_NEWSTUFF
#define ENABLE_PORT_SECURITY
#define ENABLE_BELT_INVENTORY_EX
#define ENABLE_CMD_WARP_IN_DUNGEON
// #define ENABLE_ITEM_ATTR_COSTUME
// #define ENABLE_SEQUENCE_SYSTEM
#define ENABLE_PLAYER_PER_ACCOUNT5
#define ENABLE_DICE_SYSTEM
#define ENABLE_EXTEND_INVEN_SYSTEM
#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_WEAPON_COSTUME_SYSTEM
#define ENABLE_QUEST_DIE_EVENT
#define ENABLE_QUEST_BOOT_EVENT
#define ENABLE_QUEST_DND_EVENT
#define ENABLE_PET_SYSTEM_EX
#define ENABLE_SKILL_FLAG_PARTY
#define ENABLE_NO_DSS_QUALIFICATION
// #define ENABLE_NO_SELL_PRICE_DIVIDED_BY_5
#define ENABLE_CHECK_SELL_PRICE
#define ENABLE_GOTO_LAG_FIX
#define ENABLE_MOUNT_COSTUME_EX_SYSTEM
#define ENABLE_PENDANT_SYSTEM
#define ENABLE_GLOVE_SYSTEM
#define ENABLE_MOVE_CHANNEL
#define ENABLE_QUIVER_SYSTEM
#define ENABLE_REDUCED_ENTITY_VIEW
#define ENABLE_GUILD_TOKEN_AUTH
#define ENABLE_DB_SQL_LOG

#define __PET_SYSTEM__
// #ifdef __PET_SYSTEM__
// #define USE_ACTIVE_PET_SEAL_EFFECT
// #define PET_SEAL_ACTIVE_SOCKET_IDX 2
// #define USE_PET_SEAL_ON_LOGIN
// #endif

// enum eCommonDefines {
	// MAP_ALLOW_LIMIT = 32, // 32 default
// };

#define ENABLE_RONARK_SYSTEM	// Ronark Sistemi
enum eCommonDefines {
	MAP_ALLOW_LIMIT = 32, // 32 default
	GetGoldMultipler = 1,
#ifdef ENABLE_RONARK_SYSTEM
	RONARK_BOSS = 6192,
	RONARK_REWARD_MOB = 20392,
#endif
};

#define on
#define ENABLE_ITEM_COUNT_LIMIT_SYSTEM
#define ENABLE_SPECIAL_STORAGE
#define SANDIK_AC
#define ENABLE_EXTRA_SOCKET_SYSTEM
#define ENABLE_STONE_ADD
#define ENABLE_10_STATU
#define ENABLE_BONUS_STONE
#define ENABLE_BONUS_BOSS
#define ENABLE_TARGET_INFORMATION_SYSTEM	// Mob Drop İnfo Sistemi
#define __VIEW_TARGET_PLAYER_HP__			// Oyuncu Yüzdeli HP Sistemi
#define __VIEW_TARGET_DECIMAL_HP__			// Mob Yüzdeli HP Sistemi
#define ENABLE_CUBE_RENEWAL_WORLDARD			// Official Cube Sistemi
#define ENABLE_CUBE_ATTR_SOCKET					// Official Cube Sistemi Fixi
#define ORT_BEC_EFSUN_SYSTEM
#define NEW_PVM_ATTRIBUTE
#define ENABLE_FT_GAME_SYSTEM
#define ENABLE_SHOW_CHEST_DROP_SYSTEM
#define ENABLE_SORT_INVENTORY
#define ENABLE_ALIGN_RENEWAL
#define ENABLE_WINGS_NEW
#define SOCKET_SLOTS
/////
#define ENABLE_PVP_RANKING		// Arena Alani
#ifdef ENABLE_PVP_RANKING		// Arena Alani
#define ENABLE_PVP_RANKING_WITH_EMPIRE		// Arena Alani
#endif		// Arena Alani
#define ITEM_SHOP
#ifdef ITEM_SHOP
	#define SHOP_CHAR_MAX_NUM 24
	#define SHOP_CAT_MAX_NUM 256
#endif
#define ENABLE_WHEEL_OF_FORTUNE

/*--------------------------------------------FIXES-----------------------------------*/
#define ENABLE_FIX_DESTROY_GUILD_IF_WAR_IS_ACTIVE
#define ENABLE_CAMP_FIRE_FIX
#define ENABLE_ANTI_CMD_FLOOD
#define ENABLE_SQL_INJECT_CONTROL_ON_QUERY
#define ENABLE_PARTY_DUNGEON_FIX
#define ENABLE_PARTY_EXP_FIX
#define ENABLE_BOOKS_STACKFIX
#define ENABLE_MESSENGER_REMOVE_FIX
#define ENABLE_COMPUTE_POINT_FIX
#define ENABLE_LAST_ATTACK_TIME_FIX
#define ENABLE_STR_NEW_NAME_FIX
#define ENABLE_ITEM_AWARD_FIX
#define ENABLE_POINT_EXP_FIX
#define ENABLE_FIX //Yapilan Butun fixler
#define __IMPROVED_HANDSHAKE_PROCESS__
#define ENABLE_FAST_EQUIP_FIX // Hızlı İtem Tak Çıkar Fix
#define ENABLE_YMIR_AFFECT_FIX
#define __REGEN_REWORK__
#define ENABLE_CRASH_CORE_ARROW_FIX
#define ENABLE_CH_CRASH_CORE_FIX
#define ENABLE_STAT_COPY_BUG_FIX
#define ENABLE_ADDSTONE_FAILURE

/*--------------------------------------------FIXES-----------------------------------*/

#define ENABLE_WOLFMAN_CHARACTER
#ifdef ENABLE_WOLFMAN_CHARACTER
// #define DISABLE_WOLFMAN_ON_CREATE
#define USE_MOB_BLEEDING_AS_POISON
#define USE_MOB_CLAW_AS_DAGGER
// #define USE_ITEM_BLEEDING_AS_POISON
// #define USE_ITEM_CLAW_AS_DAGGER
#define USE_WOLFMAN_STONES
#define USE_WOLFMAN_BOOKS
#endif

// #define ENABLE_MAGIC_REDUCTION_SYSTEM
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
// #define USE_MAGIC_REDUCTION_STONES
#endif

// ### END New Features ###
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ### Ex Features ###
#define DISABLE_STOP_RIDING_WHEN_DIE //	if DISABLE_TOP_RIDING_WHEN_DIE is defined, the player doesn't lose the horse after dying
#define ENABLE_ACCE_COSTUME_SYSTEM //fixed version
// #define USE_ACCE_ABSORB_WITH_NO_NEGATIVE_BONUS //enable only positive bonus in acce absorb
#define ENABLE_HIGHLIGHT_NEW_ITEM //if you want to see highlighted a new item when dropped or when exchanged
#define ENABLE_KILL_EVENT_FIX //if you want to fix the 0 exp problem about the when kill lua event (recommended)
// #define ENABLE_SYSLOG_PACKET_SENT // debug purposes

#define ENABLE_EXTEND_ITEM_AWARD //slight adjustement
#ifdef ENABLE_EXTEND_ITEM_AWARD
	// #define USE_ITEM_AWARD_CHECK_ATTRIBUTES //it prevents bonuses higher than item_attr lvl1-lvl5 min-max range limit
#endif

// ### END Ex Features ###
//////////////////////////////////////////////////////////////////////////

#endif
// vaffanculo a chi t'e morto martysama
