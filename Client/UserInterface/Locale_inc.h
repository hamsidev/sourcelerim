#pragma once

//////////////////////////////////////////////////////////////////////////
// ### Default Ymir Macros ###
#define LOCALE_SERVICE_EUROPE
#define ENABLE_COSTUME_SYSTEM
#define ENABLE_ENERGY_SYSTEM
#define ENABLE_DRAGON_SOUL_SYSTEM
#define ENABLE_NEW_EQUIPMENT_SYSTEM
// ### Default Ymir Macros ###
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ### New From LocaleInc ###
#define ENABLE_PACK_GET_CHECK
// #define ENABLE_CANSEEHIDDENTHING_FOR_GM
#define ENABLE_PROTOSTRUCT_AUTODETECT
#define ENABLE_PLAYER_PER_ACCOUNT5
#define ENABLE_LEVEL_IN_TRADE
#define ENABLE_DICE_SYSTEM
#define ENABLE_EXTEND_INVEN_SYSTEM
#define ENABLE_LVL115_ARMOR_EFFECT
#define ENABLE_SLOT_WINDOW_EX
#define ENABLE_TEXT_LEVEL_REFRESH
#define ENABLE_USE_COSTUME_ATTR
#define ENABLE_DISCORD_RPC
#define ENABLE_PET_SYSTEM_EX
#define ENABLE_LOCALE_EX
#define ENABLE_NO_DSS_QUALIFICATION
//#define ENABLE_NO_SELL_PRICE_DIVIDED_BY_5
#define ENABLE_PENDANT_SYSTEM
#define ENABLE_GLOVE_SYSTEM
#define ENABLE_MOVE_CHANNEL
#define ENABLE_QUIVER_SYSTEM
#define ENABLE_RACE_HEIGHT
#define ENABLE_ELEMENTAL_TARGET
#define ENABLE_INGAME_CONSOLE
#define ENABLE_4TH_AFF_SKILL_DESC
#define ENABLE_LOCALE_COMMON
#define ENABLE_GUILD_TOKEN_AUTH

#define ENABLE_NEW_EVENT_STRUCT
#ifdef ENABLE_NEW_EVENT_STRUCT
#define USE_NEW_EVENT_TEXT_AUTO_Y
#endif

#define WJ_SHOW_MOB_INFO
#ifdef WJ_SHOW_MOB_INFO
#define ENABLE_SHOW_MOBAIFLAG
#define ENABLE_SHOW_MOBLEVEL
#define WJ_SHOW_MOB_INFO_EX
#endif
// ### New From LocaleInc ###
//////////////////////////////////////////////////////////////////////////
#define DAMAGE_ON
#define NOKTA_HASAR // Noktalı Hasar Sistemi
#ifdef NOKTA_HASAR // Noktalı Hasar Sistemi
#define M_K_T HASAR
#endif
#define ENABLE_ITEM_COUNT_LIMIT_SYSTEM
#define ENABLE_SPECIAL_STORAGE
#define SANDIK_AC
#define ENABLE_EXTRA_SOCKET_SYSTEM
#define ENABLE_10_STATU
#define ENABLE_BONUS_STONE
#define ENABLE_BONUS_BOSS
#define ENABLE_PET_SLOT
#define ENABLE_VIEW_TARGET_PLAYER_HP	// Oyuncu Yüzdeli HP görme sistemi
#define ENABLE_VIEW_TARGET_DECIMAL_HP	// MOB Yüzdeli HP görme sistemi
#define ENABLE_TARGET_INFORMATION_SYSTEM
#define ENABLE_CUBE_RENEWAL_WORLDARD
#define NEW_PVM_ATTRIBUTE
#define ENABLE_FT_GAME_SYSTEM
#define ENABLE_SHOW_CHEST_DROP_SYSTEM
#define __BL__DETAILS_UI__
#define ENABLE_ALIGN_RENEWAL
#define ENABLE_CPP_PSM
#define ENABLE_SHINING_SYSTEM
#define ENABLE_MODEL_RENDER_TARGET
#define ENABLE_WINGS_NEW
#define SOCKET_SLOTS
////
#define ENABLE_PVP_RANKING	 // Savas Alani
#ifdef ENABLE_PVP_RANKING	 // Savas Alani
	#define ENABLE_PVP_RANKING_WITH_EMPIRE	 // Savas Alani
#endif	 // Savas Alani
#define ITEM_SHOP
#ifdef ITEM_SHOP
	#define SHOP_CHAR_MAX_NUM 24
	#define SHOP_CAT_MAX_NUM 256
#endif
#define ENABLE_WHEEL_OF_FORTUNE
#define ENABLE_RONARK_SYSTEM


/* FİXES */
#define ENABLE_MESSENGER_REMOVE_FIX
#define ENABLE_FIX_MOBS_LAG
#if defined(ENABLE_FIX_MOBS_LAG)
	#define FIX_MOBS_LAG_FIX
#endif
// #define PACKET_GONDER_KNK
/* FİXES */


//////////////////////////////////////////////////////////////////////////
// ### From GameLib ###
#define ENABLE_WOLFMAN_CHARACTER
#ifdef ENABLE_WOLFMAN_CHARACTER
// #define DISABLE_WOLFMAN_ON_CREATE
#endif
// #define ENABLE_MAGIC_REDUCTION_SYSTEM
#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_WEAPON_COSTUME_SYSTEM
// ### From GameLib ###
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ### New System Defines - Extended Version ###

// if is define ENABLE_ACCE_COSTUME_SYSTEM the players can use shoulder sash
#define ENABLE_ACCE_COSTUME_SYSTEM
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
// #define USE_ACCE_ABSORB_WITH_NO_NEGATIVE_BONUS
#endif

// if you want use SetMouseWheelScrollEvent or you want use mouse wheel to move the scrollbar
#define ENABLE_MOUSEWHEEL_EVENT

//if you want to see highlighted a new item when dropped or when exchanged
#define ENABLE_HIGHLIGHT_NEW_ITEM

// it shows emojis in the textlines
#define ENABLE_EMOJI_SYSTEM

// effects while hidden won't show up
#define __ENABLE_STEALTH_FIX__

// circle dots in minimap instead of squares
#define ENABLE_MINIMAP_WHITEMARK_CIRCLE
#define ENABLE_MINIMAP_TELEPORT_CLICK // click on minimap as gm to warp directly

// for debug: print received packets
// #define ENABLE_PRINT_RECV_PACKET_DEBUG

// ### New System Defines - Extended Version ###
//////////////////////////////////////////////////////////////////////////
// vaffanculo a chi t'e morto martysama
