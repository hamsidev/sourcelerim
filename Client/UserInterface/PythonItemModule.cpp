#include "StdAfx.h"
#include "PythonItem.h"

#include "../gamelib/ItemManager.h"
#include "../gamelib/GameLibDefines.h"
#include "InstanceBase.h"
#include "AbstractApplication.h"

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;

PyObject * itemSetUseSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iUseSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iUseSound))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.SetUseSoundFileName(iUseSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemSetDropSoundFileName(PyObject * poSelf, PyObject * poArgs)
{
	int iDropSound;
	if (!PyTuple_GetInteger(poArgs, 0, &iDropSound))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonItem& rkItem=CPythonItem::Instance();
	rkItem.SetDropSoundFileName(iDropSound, szFileName);
	return Py_BuildNone();
}

PyObject * itemSelectItem(PyObject * poSelf, PyObject * poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BadArgument();

	if (!CItemManager::Instance().SelectItemData(iIndex))
	{
		TraceError("Cannot find item by %d", iIndex);
		CItemManager::Instance().SelectItemData(60001);
	}

	return Py_BuildNone();
}

PyObject * itemGetItemName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetName());
}

PyObject * itemGetItemDescription(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetDescription());
}

PyObject * itemGetItemSummary(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("s", pItemData->GetSummary());
}

PyObject * itemGetIconImage(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

//	if (CItemData::ITEM_TYPE_SKILLBOOK == pItemData->GetType())
//	{
//		char szItemName[64+1];
//		_snprintf(szItemName, "d:/ymir work/ui/items/etc/book_%02d.sub", );
//		CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szItemName);
//	}

	return Py_BuildValue("i", pItemData->GetIconImage());
}

PyObject * itemGetIconImageFileName(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildValue("s", "Noname");

	return Py_BuildValue("s", pImage->GetFileName());
}

PyObject * itemGetItemSize(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("(ii)", 1, pItemData->GetSize());
}

PyObject * itemGetItemType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetType());
}

PyObject * itemGetItemSubType(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->GetSubType());
}

PyObject * itemGetIBuyItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Py_BuildException("no selected item data");

	return PyLong_FromLongLong(pItemData->GetIBuyItemPrice());
}

PyObject * itemGetISellItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

	if (!pItemData)
		return Py_BuildException("no selected item data");

	return PyLong_FromLongLong(pItemData->GetISellItemPrice());
}

PyObject * itemIsAntiFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsAntiFlag(iFlag));
}

PyObject * itemIsFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(iFlag));
}

PyObject * itemIsWearableFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsWearableFlag(iFlag));
}

PyObject * itemIs1GoldItem(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("no selected item data");

	return Py_BuildValue("i", pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD));
}

PyObject * itemGetLimit(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemLimit ItemLimit;
	if (!pItemData->GetLimit(iValueIndex, &ItemLimit))
		return Py_BuildException();

	return Py_BuildValue("ii", ItemLimit.bType, ItemLimit.lValue);
}

PyObject * itemGetAffect(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CItemData::TItemApply ItemApply;
	if (!pItemData->GetApply(iValueIndex, &ItemApply))
		return Py_BuildException();

	if ((CItemData::APPLY_ATT_SPEED == ItemApply.bType) && (CItemData::ITEM_TYPE_WEAPON == pItemData->GetType()) && (CItemData::WEAPON_TWO_HANDED == pItemData->GetSubType()))
	{
		ItemApply.lValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
	}

	return Py_BuildValue("ii", ItemApply.bType, ItemApply.lValue);
}

PyObject * itemGetValue(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetValue(iValueIndex));
}

PyObject * itemGetSocket(PyObject * poSelf, PyObject * poArgs)
{
	int iValueIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iValueIndex))
		return Py_BadArgument();

	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->GetSocket(iValueIndex));
}

PyObject * itemGetIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	CGraphicSubImage * pImage = pItemData->GetIconImage();
	if (!pImage)
		return Py_BuildException("Cannot get icon image by %d", pItemData->GetIndex());

	CGraphicImageInstance * pImageInstance = CGraphicImageInstance::New();
	pImageInstance->SetImagePointer(pImage);

	return Py_BuildValue("i", pImageInstance);
}

PyObject * itemDeleteIconInstance(PyObject * poSelf, PyObject * poArgs)
{
	int iHandle;
	if (!PyTuple_GetInteger(poArgs, 0, &iHandle))
		return Py_BadArgument();

	CGraphicImageInstance::Delete((CGraphicImageInstance *) iHandle);

	return Py_BuildNone();
}

PyObject * itemIsEquipmentVID(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Not yet select item data");

	return Py_BuildValue("i", pItemData->IsEquipment());
}

PyObject* itemGetUseType(PyObject * poSelf, PyObject * poArgs)
{
	int iItemVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemVID))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemVID);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	return Py_BuildValue("s", pItemData->GetUseTypeString());
}

PyObject * itemIsRefineScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (pItemData->GetType() != CItemData::ITEM_TYPE_USE)
		return Py_BuildValue("i", FALSE);

	switch (pItemData->GetSubType())
	{
		case CItemData::USE_TUNING:
			return Py_BuildValue("i", TRUE);
			break;
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsDetachScroll(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	int iType = pItemData->GetType();
	int iSubType = pItemData->GetSubType();
	if (iType == CItemData::ITEM_TYPE_USE)
	if (iSubType == CItemData::USE_DETACHMENT)
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemCanAddToQuickSlotItem(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_USE == pItemData->GetType() || CItemData::ITEM_TYPE_QUEST == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsKey(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_TREASURE_KEY == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemIsMetin(PyObject * poSelf, PyObject * poArgs)
{
	int iItemIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemIndex))
		return Py_BadArgument();

	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find select item data");

	if (CItemData::ITEM_TYPE_METIN == pItemData->GetType())
	{
		return Py_BuildValue("i", TRUE);
	}

	return Py_BuildValue("i", FALSE);
}

PyObject * itemRender(PyObject * poSelf, PyObject * poArgs)
{
	CPythonItem::Instance().Render();
	return Py_BuildNone();
}

PyObject * itemUpdate(PyObject * poSelf, PyObject * poArgs)
{
	IAbstractApplication& rkApp=IAbstractApplication::GetSingleton();

	POINT ptMouse;
	rkApp.GetMousePosition(&ptMouse);

	CPythonItem::Instance().Update(ptMouse);
	return Py_BuildNone();
}

PyObject * itemCreateItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();
	int iVirtualNumber;
	if (!PyTuple_GetInteger(poArgs, 1, &iVirtualNumber))
		return Py_BadArgument();

	float x;
	if (!PyTuple_GetFloat(poArgs, 2, &x))
		return Py_BadArgument();
	float y;
	if (!PyTuple_GetFloat(poArgs, 3, &y))
		return Py_BadArgument();
	float z;
	if (!PyTuple_GetFloat(poArgs, 4, &z))
		return Py_BadArgument();

	bool bDrop = true;
	PyTuple_GetBoolean(poArgs, 5, &bDrop);

	CPythonItem::Instance().CreateItem(iVirtualID, iVirtualNumber, x, y, z, bDrop);

	return Py_BuildNone();
}

PyObject * itemDeleteItem(PyObject * poSelf, PyObject * poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BadArgument();

	CPythonItem::Instance().DeleteItem(iVirtualID);
	return Py_BuildNone();
}

PyObject * itemPick(PyObject * poSelf, PyObject * poArgs)
{
	DWORD dwItemID;
	if (CPythonItem::Instance().GetPickedItemID(&dwItemID))
		return Py_BuildValue("i", dwItemID);
	else
		return Py_BuildValue("i", -1);
}

#if defined(__BL__DETAILS_UI__)
#include "Packet.h"

static BYTE __GetApplyPointType(BYTE bApplyType)
{
	switch (bApplyType)
	{
		// Generated from constants.cpp(aApplyInfo)
		case CItemData::EApplyTypes::APPLY_NONE:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_MAX_HP:	return EPointTypes::POINT_MAX_HP;
		case CItemData::EApplyTypes::APPLY_MAX_SP:	return EPointTypes::POINT_MAX_SP;
		case CItemData::EApplyTypes::APPLY_CON:	return EPointTypes::POINT_HT;
		case CItemData::EApplyTypes::APPLY_INT:	return EPointTypes::POINT_IQ;
		case CItemData::EApplyTypes::APPLY_STR:	return EPointTypes::POINT_ST;
		case CItemData::EApplyTypes::APPLY_DEX:	return EPointTypes::POINT_DX;
		case CItemData::EApplyTypes::APPLY_ATT_SPEED:	return EPointTypes::POINT_ATT_SPEED;
		case CItemData::EApplyTypes::APPLY_MOV_SPEED:	return EPointTypes::POINT_MOV_SPEED;
		case CItemData::EApplyTypes::APPLY_CAST_SPEED:	return EPointTypes::POINT_CASTING_SPEED;
		case CItemData::EApplyTypes::APPLY_HP_REGEN:	return EPointTypes::POINT_HP_REGEN;
		case CItemData::EApplyTypes::APPLY_SP_REGEN:	return EPointTypes::POINT_SP_REGEN;
		case CItemData::EApplyTypes::APPLY_POISON_PCT:	return EPointTypes::POINT_POISON_PCT;
		case CItemData::EApplyTypes::APPLY_STUN_PCT:	return EPointTypes::POINT_STUN_PCT;
		case CItemData::EApplyTypes::APPLY_SLOW_PCT:	return EPointTypes::POINT_SLOW_PCT;
		case CItemData::EApplyTypes::APPLY_CRITICAL_PCT:	return EPointTypes::POINT_CRITICAL_PCT;
		case CItemData::EApplyTypes::APPLY_PENETRATE_PCT:	return EPointTypes::POINT_PENETRATE_PCT;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_HUMAN:	return EPointTypes::POINT_ATTBONUS_HUMAN;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_ANIMAL:	return EPointTypes::POINT_ATTBONUS_ANIMAL;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_ORC:	return EPointTypes::POINT_ATTBONUS_ORC;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_MILGYO:	return EPointTypes::POINT_ATTBONUS_MILGYO;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_UNDEAD:	return EPointTypes::POINT_ATTBONUS_UNDEAD;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_DEVIL:	return EPointTypes::POINT_ATTBONUS_DEVIL;
		case CItemData::EApplyTypes::APPLY_STEAL_HP:	return EPointTypes::POINT_STEAL_HP;
		case CItemData::EApplyTypes::APPLY_STEAL_SP:	return EPointTypes::POINT_STEAL_SP;
		case CItemData::EApplyTypes::APPLY_MANA_BURN_PCT:	return EPointTypes::POINT_MANA_BURN_PCT;
		case CItemData::EApplyTypes::APPLY_DAMAGE_SP_RECOVER:	return EPointTypes::POINT_DAMAGE_SP_RECOVER;
		case CItemData::EApplyTypes::APPLY_BLOCK:	return EPointTypes::POINT_BLOCK;
		case CItemData::EApplyTypes::APPLY_DODGE:	return EPointTypes::POINT_DODGE;
		case CItemData::EApplyTypes::APPLY_RESIST_SWORD:	return EPointTypes::POINT_RESIST_SWORD;
		case CItemData::EApplyTypes::APPLY_RESIST_TWOHAND:	return EPointTypes::POINT_RESIST_TWOHAND;
		case CItemData::EApplyTypes::APPLY_RESIST_DAGGER:	return EPointTypes::POINT_RESIST_DAGGER;
		case CItemData::EApplyTypes::APPLY_RESIST_BELL:	return EPointTypes::POINT_RESIST_BELL;
		case CItemData::EApplyTypes::APPLY_RESIST_FAN:	return EPointTypes::POINT_RESIST_FAN;
		case CItemData::EApplyTypes::APPLY_RESIST_BOW:	return EPointTypes::POINT_RESIST_BOW;
		case CItemData::EApplyTypes::APPLY_RESIST_FIRE:	return EPointTypes::POINT_RESIST_FIRE;
		case CItemData::EApplyTypes::APPLY_RESIST_ELEC:	return EPointTypes::POINT_RESIST_ELEC;
		case CItemData::EApplyTypes::APPLY_RESIST_MAGIC:	return EPointTypes::POINT_RESIST_MAGIC;
		case CItemData::EApplyTypes::APPLY_RESIST_WIND:	return EPointTypes::POINT_RESIST_WIND;
		case CItemData::EApplyTypes::APPLY_REFLECT_MELEE:	return EPointTypes::POINT_REFLECT_MELEE;
		case CItemData::EApplyTypes::APPLY_REFLECT_CURSE:	return EPointTypes::POINT_REFLECT_CURSE;
		case CItemData::EApplyTypes::APPLY_POISON_REDUCE:	return EPointTypes::POINT_POISON_REDUCE;
		case CItemData::EApplyTypes::APPLY_KILL_SP_RECOVER:	return EPointTypes::POINT_KILL_SP_RECOVER;
		case CItemData::EApplyTypes::APPLY_EXP_DOUBLE_BONUS:	return EPointTypes::POINT_EXP_DOUBLE_BONUS;
		case CItemData::EApplyTypes::APPLY_GOLD_DOUBLE_BONUS:	return EPointTypes::POINT_GOLD_DOUBLE_BONUS;
		case CItemData::EApplyTypes::APPLY_ITEM_DROP_BONUS:	return EPointTypes::POINT_ITEM_DROP_BONUS;
		case CItemData::EApplyTypes::APPLY_POTION_BONUS:	return EPointTypes::POINT_POTION_BONUS;
		case CItemData::EApplyTypes::APPLY_KILL_HP_RECOVER:	return EPointTypes::POINT_KILL_HP_RECOVER;
		case CItemData::EApplyTypes::APPLY_IMMUNE_STUN:	return EPointTypes::POINT_IMMUNE_STUN;
		case CItemData::EApplyTypes::APPLY_IMMUNE_SLOW:	return EPointTypes::POINT_IMMUNE_SLOW;
		case CItemData::EApplyTypes::APPLY_IMMUNE_FALL:	return EPointTypes::POINT_IMMUNE_FALL;
		case CItemData::EApplyTypes::APPLY_SKILL:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_BOW_DISTANCE:	return EPointTypes::POINT_BOW_DISTANCE;
		case CItemData::EApplyTypes::APPLY_ATT_GRADE_BONUS:	return EPointTypes::POINT_ATT_GRADE_BONUS;
		case CItemData::EApplyTypes::APPLY_DEF_GRADE_BONUS:	return EPointTypes::POINT_DEF_GRADE_BONUS;
		case CItemData::EApplyTypes::APPLY_MAGIC_ATT_GRADE:	return EPointTypes::POINT_MAGIC_ATT_GRADE_BONUS;
		case CItemData::EApplyTypes::APPLY_MAGIC_DEF_GRADE:	return EPointTypes::POINT_MAGIC_DEF_GRADE_BONUS;
		case CItemData::EApplyTypes::APPLY_CURSE_PCT:	return EPointTypes::POINT_CURSE_PCT;
		case CItemData::EApplyTypes::APPLY_MAX_STAMINA:	return EPointTypes::POINT_MAX_STAMINA;
		case CItemData::EApplyTypes::APPLY_ATT_BONUS_TO_WARRIOR:	return EPointTypes::POINT_ATTBONUS_WARRIOR;
		case CItemData::EApplyTypes::APPLY_ATT_BONUS_TO_ASSASSIN:	return EPointTypes::POINT_ATTBONUS_ASSASSIN;
		case CItemData::EApplyTypes::APPLY_ATT_BONUS_TO_SURA:	return EPointTypes::POINT_ATTBONUS_SURA;
		case CItemData::EApplyTypes::APPLY_ATT_BONUS_TO_SHAMAN:	return EPointTypes::POINT_ATTBONUS_SHAMAN;
		case CItemData::EApplyTypes::APPLY_ATT_BONUS_TO_MONSTER:	return EPointTypes::POINT_ATTBONUS_MONSTER;
		case CItemData::EApplyTypes::APPLY_MALL_ATTBONUS:	return EPointTypes::POINT_ATT_BONUS;
		case CItemData::EApplyTypes::APPLY_MALL_DEFBONUS:	return EPointTypes::POINT_MALL_DEFBONUS;
		case CItemData::EApplyTypes::APPLY_MALL_EXPBONUS:	return EPointTypes::POINT_MALL_EXPBONUS;
		case CItemData::EApplyTypes::APPLY_MALL_ITEMBONUS:	return EPointTypes::POINT_MALL_ITEMBONUS;
		case CItemData::EApplyTypes::APPLY_MALL_GOLDBONUS:	return EPointTypes::POINT_MALL_GOLDBONUS;
		case CItemData::EApplyTypes::APPLY_MAX_HP_PCT:	return EPointTypes::POINT_MAX_HP_PCT;
		case CItemData::EApplyTypes::APPLY_MAX_SP_PCT:	return EPointTypes::POINT_MAX_SP_PCT;
		case CItemData::EApplyTypes::APPLY_SKILL_DAMAGE_BONUS:	return EPointTypes::POINT_SKILL_DAMAGE_BONUS;
		case CItemData::EApplyTypes::APPLY_NORMAL_HIT_DAMAGE_BONUS:	return EPointTypes::POINT_NORMAL_HIT_DAMAGE_BONUS;
		case CItemData::EApplyTypes::APPLY_SKILL_DEFEND_BONUS:	return EPointTypes::POINT_SKILL_DEFEND_BONUS;
		case CItemData::EApplyTypes::APPLY_NORMAL_HIT_DEFEND_BONUS:	return EPointTypes::POINT_NORMAL_HIT_DEFEND_BONUS;
		case CItemData::EApplyTypes::APPLY_EXTRACT_HP_PCT:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_RESIST_WARRIOR:	return EPointTypes::POINT_RESIST_WARRIOR;
		case CItemData::EApplyTypes::APPLY_RESIST_ASSASSIN:	return EPointTypes::POINT_RESIST_ASSASSIN;
		case CItemData::EApplyTypes::APPLY_RESIST_SURA:	return EPointTypes::POINT_RESIST_SURA;
		case CItemData::EApplyTypes::APPLY_RESIST_SHAMAN:	return EPointTypes::POINT_RESIST_SHAMAN;
		case CItemData::EApplyTypes::APPLY_ENERGY:	return EPointTypes::POINT_ENERGY;
		case CItemData::EApplyTypes::APPLY_DEF_GRADE:	return EPointTypes::POINT_DEF_GRADE;

		case CItemData::EApplyTypes::APPLY_COSTUME_ATTR_BONUS:	return EPointTypes::POINT_COSTUME_ATTR_BONUS;
		case CItemData::EApplyTypes::APPLY_MAGIC_ATTBONUS_PER:	return EPointTypes::POINT_MAGIC_ATT_BONUS_PER;
		case CItemData::EApplyTypes::APPLY_MELEE_MAGIC_ATTBONUS_PER:	return EPointTypes::POINT_MELEE_MAGIC_ATT_BONUS_PER;
		case CItemData::EApplyTypes::APPLY_RESIST_ICE:	return EPointTypes::POINT_RESIST_ICE;
		case CItemData::EApplyTypes::APPLY_RESIST_EARTH:	return EPointTypes::POINT_RESIST_EARTH;
		case CItemData::EApplyTypes::APPLY_RESIST_DARK:	return EPointTypes::POINT_RESIST_DARK;
		case CItemData::EApplyTypes::APPLY_ANTI_CRITICAL_PCT:	return EPointTypes::POINT_RESIST_CRITICAL;
		case CItemData::EApplyTypes::APPLY_ANTI_PENETRATE_PCT:	return EPointTypes::POINT_RESIST_PENETRATE; 

		case CItemData::EApplyTypes::APPLY_ATTBONUS_STONE:	return EPointTypes::POINT_ATTBONUS_STONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_BOSS:	return EPointTypes::POINT_ATTBONUS_BOSS;

#ifdef ENABLE_PENDANT_SYSTEM
	case CItemData::EApplyTypes::APPLY_ENCHANT_FIRE:				return EPointTypes::POINT_ENCHANT_FIRE;
	case CItemData::EApplyTypes::APPLY_ENCHANT_ICE:					return EPointTypes::POINT_ENCHANT_ICE;
	case CItemData::EApplyTypes::APPLY_ENCHANT_EARTH:				return EPointTypes::POINT_ENCHANT_EARTH;
	case CItemData::EApplyTypes::APPLY_ENCHANT_DARK:				return EPointTypes::POINT_ENCHANT_DARK;
	case CItemData::EApplyTypes::APPLY_ENCHANT_WIND:				return EPointTypes::POINT_ENCHANT_WIND;
	case CItemData::EApplyTypes::APPLY_ENCHANT_ELECT:				return EPointTypes::POINT_ENCHANT_ELECT;
	case CItemData::EApplyTypes::APPLY_RESIST_HUMAN:				return EPointTypes::POINT_RESIST_HUMAN;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_SWORD:				return EPointTypes::POINT_ATTBONUS_SWORD;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_TWOHAND:			return EPointTypes::POINT_ATTBONUS_TWOHAND;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_DAGGER:				return EPointTypes::POINT_ATTBONUS_DAGGER;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_BELL:				return EPointTypes::POINT_ATTBONUS_BELL;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_FAN:				return EPointTypes::POINT_ATTBONUS_FAN;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_BOW:				return EPointTypes::POINT_ATTBONUS_BOW;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_CLAW:				return EPointTypes::POINT_ATTBONUS_CLAW;
	case CItemData::EApplyTypes::APPLY_ATTBONUS_CZ:					return EPointTypes::POINT_ATTBONUS_CZ;
#endif

#if 0
		case CItemData::EApplyTypes::APPLY_RESIST_HUMAN:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_CZ:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_DESERT:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_INSECT:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_RESIST_MAGIC_REDUCTION:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_ELECT:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_ICE:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_DARK:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_FIRE:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_WIND:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ENCHANT_EARTH:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_WOLFMAN:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_RESIST_WOLFMAN:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_RESIST_CLAW:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_SWORD:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_TWOHAND:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_DAGGER:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_BELL:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_FAN:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_BOW:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_ATTBONUS_CLAW:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_RESIST_MOUNT_FALL:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_BLEEDING_PCT:	return EPointTypes::POINT_NONE;
		case CItemData::EApplyTypes::APPLY_BLEEDING_REDUCE:	return EPointTypes::POINT_NONE;
#endif
	}

	return POINT_MAX_NUM;
}

PyObject* itemGetApplyPoint(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bApplyType;
	if (!PyTuple_GetByte(poArgs, 0, &bApplyType))
		return Py_BuildException();

	const BYTE bApplyPointType{ __GetApplyPointType(bApplyType) };
	if (bApplyPointType == POINT_MAX_NUM)
		TraceError("itemGetApplyPoint - Cannot find Apply Point Type(Apply Type: %d)", bApplyType);
	
	return Py_BuildValue("i", bApplyPointType);
}
#endif

PyObject* itemLoadItemTable(PyObject* poSelf, PyObject* poArgs)
{
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BadArgument();

	CItemManager::Instance().LoadItemTable(szFileName);
	return Py_BuildNone();
}

void initItem()
{
	static PyMethodDef s_methods[] =
	{
		{ "SetUseSoundFileName",			itemSetUseSoundFileName,				METH_VARARGS },
		{ "SetDropSoundFileName",			itemSetDropSoundFileName,				METH_VARARGS },
		{ "SelectItem",						itemSelectItem,							METH_VARARGS },

		{ "GetItemName",					itemGetItemName,						METH_VARARGS },
		{ "GetItemDescription",				itemGetItemDescription,					METH_VARARGS },
		{ "GetItemSummary",					itemGetItemSummary,						METH_VARARGS },
		{ "GetIconImage",					itemGetIconImage,						METH_VARARGS },
		{ "GetIconImageFileName",			itemGetIconImageFileName,				METH_VARARGS },
		{ "GetItemSize",					itemGetItemSize,						METH_VARARGS },
		{ "GetItemType",					itemGetItemType,						METH_VARARGS },
		{ "GetItemSubType",					itemGetItemSubType,						METH_VARARGS },
		{ "GetIBuyItemPrice",				itemGetIBuyItemPrice,					METH_VARARGS },
		{ "GetISellItemPrice",				itemGetISellItemPrice,					METH_VARARGS },
		{ "IsAntiFlag",						itemIsAntiFlag,							METH_VARARGS },
		{ "IsFlag",							itemIsFlag,								METH_VARARGS },
		{ "IsWearableFlag",					itemIsWearableFlag,						METH_VARARGS },
		{ "Is1GoldItem",					itemIs1GoldItem,						METH_VARARGS },
		{ "GetLimit",						itemGetLimit,							METH_VARARGS },
		{ "GetAffect",						itemGetAffect,							METH_VARARGS },
		{ "GetValue",						itemGetValue,							METH_VARARGS },
		{ "GetSocket",						itemGetSocket,							METH_VARARGS },
		{ "GetIconInstance",				itemGetIconInstance,					METH_VARARGS },
		{ "GetUseType",						itemGetUseType,							METH_VARARGS },
		{ "DeleteIconInstance",				itemDeleteIconInstance,					METH_VARARGS },
		{ "IsEquipmentVID",					itemIsEquipmentVID,						METH_VARARGS },
		{ "IsRefineScroll",					itemIsRefineScroll,						METH_VARARGS },
		{ "IsDetachScroll",					itemIsDetachScroll,						METH_VARARGS },
		{ "IsKey",							itemIsKey,								METH_VARARGS },
		{ "IsMetin",						itemIsMetin,							METH_VARARGS },
		{ "CanAddToQuickSlotItem",			itemCanAddToQuickSlotItem,				METH_VARARGS },

		{ "Update",							itemUpdate,								METH_VARARGS },
		{ "Render",							itemRender,								METH_VARARGS },
		{ "CreateItem",						itemCreateItem,							METH_VARARGS },
		{ "DeleteItem",						itemDeleteItem,							METH_VARARGS },
		{ "Pick",							itemPick,								METH_VARARGS },

		{ "LoadItemTable",					itemLoadItemTable,						METH_VARARGS },

#if defined(__BL__DETAILS_UI__)
		{ "GetApplyPoint",					itemGetApplyPoint,						METH_VARARGS },
#endif

#ifdef ENABLE_SEALBIND_SYSTEM
		{ "IsSealScroll",					itemIsSealScroll,						METH_VARARGS },
		{ "GetDefaultSealDate",				itemGetDefaultSealDate,					METH_VARARGS },
		{ "GetUnlimitedSealDate",			itemGetUnlimitedSealDate,				METH_VARARGS },
#endif

		{ NULL,								NULL,									NULL		 },
	};

	PyObject * poModule = Py_InitModule("item", s_methods);

	PyModule_AddIntConstant(poModule, "USESOUND_ACCESSORY",			CPythonItem::USESOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "USESOUND_ARMOR",				CPythonItem::USESOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "USESOUND_BOW",				CPythonItem::USESOUND_BOW);
	PyModule_AddIntConstant(poModule, "USESOUND_DEFAULT",			CPythonItem::USESOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "USESOUND_WEAPON",			CPythonItem::USESOUND_WEAPON);
	PyModule_AddIntConstant(poModule, "USESOUND_POTION",			CPythonItem::USESOUND_POTION);
	PyModule_AddIntConstant(poModule, "USESOUND_PORTAL",			CPythonItem::USESOUND_PORTAL);

	PyModule_AddIntConstant(poModule, "DROPSOUND_ACCESSORY",		CPythonItem::DROPSOUND_ACCESSORY);
	PyModule_AddIntConstant(poModule, "DROPSOUND_ARMOR",			CPythonItem::DROPSOUND_ARMOR);
	PyModule_AddIntConstant(poModule, "DROPSOUND_BOW",				CPythonItem::DROPSOUND_BOW);
	PyModule_AddIntConstant(poModule, "DROPSOUND_DEFAULT",			CPythonItem::DROPSOUND_DEFAULT);
	PyModule_AddIntConstant(poModule, "DROPSOUND_WEAPON",			CPythonItem::DROPSOUND_WEAPON);

	PyModule_AddIntConstant(poModule, "EQUIPMENT_COUNT",			c_Equipment_Count);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_HEAD",				c_Equipment_Head);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BODY",				c_Equipment_Body);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WEAPON",			c_Equipment_Weapon);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WRIST",			c_Equipment_Wrist);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SHOES",			c_Equipment_Shoes);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_NECK",				c_Equipment_Neck);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_EAR",				c_Equipment_Ear);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE1",			c_Equipment_Unique1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_UNIQUE2",			c_Equipment_Unique2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_ARROW",			c_Equipment_Arrow);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SHIELD",			c_Equipment_Shield);
#ifdef ENABLE_WINGS_NEW
	PyModule_AddIntConstant(poModule, "EQUIPMENT_WING",				c_Equipment_Wing);
#endif
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_BELT",				c_Equipment_Belt);
#endif
#ifdef ENABLE_PENDANT_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PENDANT",			c_Equipment_Pendant);
#endif
#ifdef ENABLE_GLOVE_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_GLOVE",			c_Equipment_Glove);
#endif
#ifdef SOCKET_SLOTS
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET1",				c_Equipment_Socket1);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET2",				c_Equipment_Socket2);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET3",				c_Equipment_Socket3);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET4",				c_Equipment_Socket4);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET5",				c_Equipment_Socket5);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET6",				c_Equipment_Socket6);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET7",				c_Equipment_Socket7);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET8",				c_Equipment_Socket8);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET9",				c_Equipment_Socket9);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET10",				c_Equipment_Socket10);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET11",				c_Equipment_Socket11);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET12",				c_Equipment_Socket12);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET13",				c_Equipment_Socket13);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET14",				c_Equipment_Socket14);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET15",				c_Equipment_Socket15);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET16",				c_Equipment_Socket16);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET17",				c_Equipment_Socket17);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET18",				c_Equipment_Socket18);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET19",				c_Equipment_Socket19);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET20",				c_Equipment_Socket20);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET21",				c_Equipment_Socket21);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET22",				c_Equipment_Socket22);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET23",				c_Equipment_Socket23);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET24",				c_Equipment_Socket24);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SOCKET25",				c_Equipment_Socket25);
#endif

	PyModule_AddIntConstant(poModule, "ITEM_TYPE_NONE",				CItemData::ITEM_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_WEAPON",			CItemData::ITEM_TYPE_WEAPON);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ARMOR",			CItemData::ITEM_TYPE_ARMOR);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_USE",				CItemData::ITEM_TYPE_USE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_AUTOUSE",			CItemData::ITEM_TYPE_AUTOUSE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_MATERIAL",			CItemData::ITEM_TYPE_MATERIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL",			CItemData::ITEM_TYPE_SPECIAL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TOOL",				CItemData::ITEM_TYPE_TOOL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_LOTTERY",			CItemData::ITEM_TYPE_LOTTERY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ELK",				CItemData::ITEM_TYPE_ELK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_METIN",			CItemData::ITEM_TYPE_METIN);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CONTAINER",		CItemData::ITEM_TYPE_CONTAINER);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_FISH",				CItemData::ITEM_TYPE_FISH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_ROD",				CItemData::ITEM_TYPE_ROD);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RESOURCE",			CItemData::ITEM_TYPE_RESOURCE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_CAMPFIRE",			CItemData::ITEM_TYPE_CAMPFIRE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_UNIQUE",			CItemData::ITEM_TYPE_UNIQUE);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLBOOK",		CItemData::ITEM_TYPE_SKILLBOOK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_QUEST",			CItemData::ITEM_TYPE_QUEST);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_POLYMORPH",		CItemData::ITEM_TYPE_POLYMORPH);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_BOX",		CItemData::ITEM_TYPE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TREASURE_KEY",		CItemData::ITEM_TYPE_TREASURE_KEY);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SKILLFORGET",		CItemData::ITEM_TYPE_SKILLFORGET);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GIFTBOX",			CItemData::ITEM_TYPE_GIFTBOX);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PICK",				CItemData::ITEM_TYPE_PICK);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_HAIR",				CItemData::ITEM_TYPE_HAIR);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_TOTEM",			CItemData::ITEM_TYPE_TOTEM);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BLEND",			CItemData::ITEM_TYPE_BLEND);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_COSTUME",			CItemData::ITEM_TYPE_COSTUME);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_DS",				CItemData::ITEM_TYPE_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SPECIAL_DS",		CItemData::ITEM_TYPE_SPECIAL_DS);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_EXTRACT",			CItemData::ITEM_TYPE_EXTRACT);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SECONDARY_COIN",	CItemData::ITEM_TYPE_SECONDARY_COIN);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RING",				CItemData::ITEM_TYPE_RING);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_BELT",				CItemData::ITEM_TYPE_BELT);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PET",				CItemData::ITEM_TYPE_PET);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_MEDIUM",			CItemData::ITEM_TYPE_MEDIUM);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_GACHA",			CItemData::ITEM_TYPE_GACHA);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOUL",				CItemData::ITEM_TYPE_SOUL);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PASSIVE",			CItemData::ITEM_TYPE_PASSIVE);

#ifdef ENABLE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_COSTUME",			CItemData::ITEM_TYPE_COSTUME);

	// Item Sub Type
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_BODY",			CItemData::COSTUME_BODY);
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_HAIR",			CItemData::COSTUME_HAIR);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_MOUNT",			CItemData::COSTUME_MOUNT);
#endif
#ifdef ENABLE_WINGS_NEW
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_WING", CItemData::COSTUME_WING);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_WING", c_Costume_Slot_Wing);
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_ACCE",			CItemData::COSTUME_ACCE);
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_WEAPON",		CItemData::COSTUME_WEAPON);
#endif

	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_START",			c_Costume_Slot_Start);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_COUNT",			c_Costume_Slot_Count);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_BODY",			c_Costume_Slot_Body);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_HAIR",			c_Costume_Slot_Hair);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_MOUNT",			c_Costume_Slot_Mount);
#endif
#ifdef ENABLE_PET_SLOT
	PyModule_AddIntConstant(poModule, "COSTUME_TYPE_PET",			CItemData::COSTUME_PET);
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_PET",			c_Costume_Slot_Pet);
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_ACCE",			c_Costume_Slot_Acce);
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_WEAPON",		c_Costume_Slot_Weapon);
#endif
	PyModule_AddIntConstant(poModule, "COSTUME_SLOT_END",			c_Costume_Slot_End);
#endif

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_START",			c_Belt_Inventory_Slot_Start);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_COUNT",			c_Belt_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "BELT_INVENTORY_SLOT_END",			c_Belt_Inventory_Slot_End);
#endif
#ifdef SOCKET_SLOTS
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET1",				CItemData::ITEM_TYPE_SOCKET1);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET2",				CItemData::ITEM_TYPE_SOCKET2);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET3",				CItemData::ITEM_TYPE_SOCKET3);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET4",				CItemData::ITEM_TYPE_SOCKET4);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET5",				CItemData::ITEM_TYPE_SOCKET5);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET6",				CItemData::ITEM_TYPE_SOCKET6);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET7",				CItemData::ITEM_TYPE_SOCKET7);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET8",				CItemData::ITEM_TYPE_SOCKET8);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET9",				CItemData::ITEM_TYPE_SOCKET9);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET10",				CItemData::ITEM_TYPE_SOCKET10);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET11",				CItemData::ITEM_TYPE_SOCKET11);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET12",				CItemData::ITEM_TYPE_SOCKET12);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET13",				CItemData::ITEM_TYPE_SOCKET13);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET14",				CItemData::ITEM_TYPE_SOCKET14);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET15",				CItemData::ITEM_TYPE_SOCKET15);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET16",				CItemData::ITEM_TYPE_SOCKET16);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET17",				CItemData::ITEM_TYPE_SOCKET17);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET18",				CItemData::ITEM_TYPE_SOCKET18);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET19",				CItemData::ITEM_TYPE_SOCKET19);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET20",				CItemData::ITEM_TYPE_SOCKET20);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET21",				CItemData::ITEM_TYPE_SOCKET21);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET22",				CItemData::ITEM_TYPE_SOCKET22);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET23",				CItemData::ITEM_TYPE_SOCKET23);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET24",				CItemData::ITEM_TYPE_SOCKET24);
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_SOCKET25",				CItemData::ITEM_TYPE_SOCKET25);
#endif

	PyModule_AddIntConstant(poModule, "WEAPON_SWORD",				CItemData::WEAPON_SWORD);
	PyModule_AddIntConstant(poModule, "WEAPON_DAGGER",				CItemData::WEAPON_DAGGER);
	PyModule_AddIntConstant(poModule, "WEAPON_BOW",					CItemData::WEAPON_BOW);
	PyModule_AddIntConstant(poModule, "WEAPON_TWO_HANDED",			CItemData::WEAPON_TWO_HANDED);
	PyModule_AddIntConstant(poModule, "WEAPON_BELL",				CItemData::WEAPON_BELL);
	PyModule_AddIntConstant(poModule, "WEAPON_FAN",					CItemData::WEAPON_FAN);
	PyModule_AddIntConstant(poModule, "WEAPON_ARROW",				CItemData::WEAPON_ARROW);
	PyModule_AddIntConstant(poModule, "WEAPON_MOUNT_SPEAR",			CItemData::WEAPON_MOUNT_SPEAR);
#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "WEAPON_CLAW",				CItemData::WEAPON_CLAW);
#endif
#ifdef ENABLE_QUIVER_SYSTEM
	PyModule_AddIntConstant(poModule, "WEAPON_QUIVER",				CItemData::WEAPON_QUIVER);
#endif
#ifdef __UNIMPLEMENTED__
	PyModule_AddIntConstant(poModule, "WEAPON_BOUQUET",				CItemData::WEAPON_BOUQUET);
#endif
	PyModule_AddIntConstant(poModule, "WEAPON_NUM_TYPES",			CItemData::WEAPON_NUM_TYPES);

	PyModule_AddIntConstant(poModule, "USE_POTION",					CItemData::USE_POTION);
	PyModule_AddIntConstant(poModule, "USE_TALISMAN",				CItemData::USE_TALISMAN);
	PyModule_AddIntConstant(poModule, "USE_TUNING",					CItemData::USE_TUNING);
	PyModule_AddIntConstant(poModule, "USE_MOVE",					CItemData::USE_MOVE);
	PyModule_AddIntConstant(poModule, "USE_TREASURE_BOX",			CItemData::USE_TREASURE_BOX);
	PyModule_AddIntConstant(poModule, "USE_MONEYBAG",				CItemData::USE_MONEYBAG);
	PyModule_AddIntConstant(poModule, "USE_BAIT",					CItemData::USE_BAIT);
	PyModule_AddIntConstant(poModule, "USE_ABILITY_UP",				CItemData::USE_ABILITY_UP);
	PyModule_AddIntConstant(poModule, "USE_AFFECT",					CItemData::USE_AFFECT);
	PyModule_AddIntConstant(poModule, "USE_CREATE_STONE",			CItemData::USE_CREATE_STONE);
	PyModule_AddIntConstant(poModule, "USE_SPECIAL",				CItemData::USE_SPECIAL);
	PyModule_AddIntConstant(poModule, "USE_POTION_NODELAY",			CItemData::USE_POTION_NODELAY);
	PyModule_AddIntConstant(poModule, "USE_CLEAR",					CItemData::USE_CLEAR);
	PyModule_AddIntConstant(poModule, "USE_INVISIBILITY",			CItemData::USE_INVISIBILITY);
	PyModule_AddIntConstant(poModule, "USE_DETACHMENT",				CItemData::USE_DETACHMENT);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_PER",		CItemData::USE_TIME_CHARGE_PER);
	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_FIX",		CItemData::USE_TIME_CHARGE_FIX);

	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_NORMAL",	CItemData::MATERIAL_DS_REFINE_NORMAL);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_BLESSED",	CItemData::MATERIAL_DS_REFINE_BLESSED);
	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_HOLLY",	CItemData::MATERIAL_DS_REFINE_HOLLY);

	PyModule_AddIntConstant(poModule, "METIN_NORMAL",				CItemData::METIN_NORMAL);
	PyModule_AddIntConstant(poModule, "METIN_GOLD",					CItemData::METIN_GOLD);

	PyModule_AddIntConstant(poModule, "LIMIT_NONE",					CItemData::LIMIT_NONE);
	PyModule_AddIntConstant(poModule, "LIMIT_LEVEL",				CItemData::LIMIT_LEVEL);
	PyModule_AddIntConstant(poModule, "LIMIT_STR",					CItemData::LIMIT_STR);
	PyModule_AddIntConstant(poModule, "LIMIT_DEX",					CItemData::LIMIT_DEX);
	PyModule_AddIntConstant(poModule, "LIMIT_INT",					CItemData::LIMIT_INT);
	PyModule_AddIntConstant(poModule, "LIMIT_CON",					CItemData::LIMIT_CON);
	PyModule_AddIntConstant(poModule, "LIMIT_PCBANG",				CItemData::LIMIT_PCBANG);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME",			CItemData::LIMIT_REAL_TIME);
	PyModule_AddIntConstant(poModule, "LIMIT_REAL_TIME_START_FIRST_USE",	CItemData::LIMIT_REAL_TIME_START_FIRST_USE);
	PyModule_AddIntConstant(poModule, "LIMIT_TIMER_BASED_ON_WEAR",	CItemData::LIMIT_TIMER_BASED_ON_WEAR);
	PyModule_AddIntConstant(poModule, "LIMIT_NEWWORLD_LEVEL",		CItemData::LIMIT_NEWWORLD_LEVEL);
	PyModule_AddIntConstant(poModule, "LIMIT_TYPE_MAX_NUM",			CItemData::LIMIT_MAX_NUM);
	PyModule_AddIntConstant(poModule, "LIMIT_MAX_NUM",				CItemData::ITEM_LIMIT_MAX_NUM);

	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_FEMALE",		CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MALE",			CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WARRIOR",		CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ASSASSIN",		CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SURA",			CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SHAMAN",		CItemData::ITEM_ANTIFLAG_SHAMAN);
#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_WOLFMAN",		CItemData::ITEM_ANTIFLAG_WOLFMAN);
#endif
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GET",			CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_DROP",			CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SELL",			CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_A",		CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_B",		CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_EMPIRE_R",		CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAVE",			CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_GIVE",			CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_PKDROP",		CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_STACK",		CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MYSHOP",		CItemData::ITEM_ANTIFLAG_MYSHOP);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_SAFEBOX",		CItemData::ITEM_ANTIFLAG_SAFEBOX);

	PyModule_AddIntConstant(poModule, "ITEM_FLAG_REFINEABLE", CItemData::ITEM_FLAG_REFINEABLE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_SAVE", CItemData::ITEM_FLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_STACKABLE", CItemData::ITEM_FLAG_STACKABLE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_COUNT_PER_1GOLD", CItemData::ITEM_FLAG_COUNT_PER_1GOLD);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_SLOW_QUERY", CItemData::ITEM_FLAG_SLOW_QUERY);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_RARE", CItemData::ITEM_FLAG_RARE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_UNIQUE", CItemData::ITEM_FLAG_UNIQUE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_MAKECOUNT", CItemData::ITEM_FLAG_MAKECOUNT);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_IRREMOVABLE", CItemData::ITEM_FLAG_IRREMOVABLE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_CONFIRM_WHEN_USE", CItemData::ITEM_FLAG_CONFIRM_WHEN_USE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_QUEST_USE", CItemData::ITEM_FLAG_QUEST_USE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_QUEST_USE_MULTIPLE", CItemData::ITEM_FLAG_QUEST_USE_MULTIPLE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_QUEST_GIVE", CItemData::ITEM_FLAG_QUEST_GIVE);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_LOG", CItemData::ITEM_FLAG_LOG);
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_APPLICABLE", CItemData::ITEM_FLAG_APPLICABLE);

	PyModule_AddIntConstant(poModule, "ANTIFLAG_FEMALE",			CItemData::ITEM_ANTIFLAG_FEMALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MALE",				CItemData::ITEM_ANTIFLAG_MALE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WARRIOR",			CItemData::ITEM_ANTIFLAG_WARRIOR);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_ASSASSIN",			CItemData::ITEM_ANTIFLAG_ASSASSIN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SURA",				CItemData::ITEM_ANTIFLAG_SURA);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SHAMAN",			CItemData::ITEM_ANTIFLAG_SHAMAN);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GET",				CItemData::ITEM_ANTIFLAG_GET);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_DROP",				CItemData::ITEM_ANTIFLAG_DROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SELL",				CItemData::ITEM_ANTIFLAG_SELL);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_A",			CItemData::ITEM_ANTIFLAG_EMPIRE_A);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_B",			CItemData::ITEM_ANTIFLAG_EMPIRE_B);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_EMPIRE_R",			CItemData::ITEM_ANTIFLAG_EMPIRE_R);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAVE",				CItemData::ITEM_ANTIFLAG_SAVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_GIVE",				CItemData::ITEM_ANTIFLAG_GIVE);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_PKDROP",			CItemData::ITEM_ANTIFLAG_PKDROP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_STACK",				CItemData::ITEM_ANTIFLAG_STACK);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_MYSHOP",			CItemData::ITEM_ANTIFLAG_MYSHOP);
	PyModule_AddIntConstant(poModule, "ANTIFLAG_SAFEBOX",			CItemData::ITEM_ANTIFLAG_SAFEBOX);
#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "ANTIFLAG_WOLFMAN",			CItemData::ITEM_ANTIFLAG_WOLFMAN);
#endif
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_CHANGELOOK",	CItemData::ITEM_ANTIFLAG_CHANGELOOK);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_REINFORCE",	CItemData::ITEM_ANTIFLAG_REINFORCE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ENCHANT",		CItemData::ITEM_ANTIFLAG_ENCHANT);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ENERGY",		CItemData::ITEM_ANTIFLAG_ENERGY);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_PETFEED",		CItemData::ITEM_ANTIFLAG_PETFEED);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_APPLY",		CItemData::ITEM_ANTIFLAG_APPLY);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_ACCE",			CItemData::ITEM_ANTIFLAG_ACCE);
	PyModule_AddIntConstant(poModule, "ITEM_ANTIFLAG_MAIL",			CItemData::ITEM_ANTIFLAG_MAIL);

	PyModule_AddIntConstant(poModule, "WEARABLE_BODY",				CItemData::WEARABLE_BODY);
	PyModule_AddIntConstant(poModule, "WEARABLE_HEAD",				CItemData::WEARABLE_HEAD);
	PyModule_AddIntConstant(poModule, "WEARABLE_FOOTS",				CItemData::WEARABLE_FOOTS);
	PyModule_AddIntConstant(poModule, "WEARABLE_WRIST",				CItemData::WEARABLE_WRIST);
	PyModule_AddIntConstant(poModule, "WEARABLE_WEAPON",			CItemData::WEARABLE_WEAPON);
	PyModule_AddIntConstant(poModule, "WEARABLE_NECK",				CItemData::WEARABLE_NECK);
	PyModule_AddIntConstant(poModule, "WEARABLE_EAR",				CItemData::WEARABLE_EAR);
	PyModule_AddIntConstant(poModule, "WEARABLE_UNIQUE",			CItemData::WEARABLE_UNIQUE);
	PyModule_AddIntConstant(poModule, "WEARABLE_SHIELD",			CItemData::WEARABLE_SHIELD);
	PyModule_AddIntConstant(poModule, "WEARABLE_ARROW",				CItemData::WEARABLE_ARROW);
	PyModule_AddIntConstant(poModule, "WEARABLE_HAIR",				CItemData::WEARABLE_HAIR);
	PyModule_AddIntConstant(poModule, "WEARABLE_ABILITY",			CItemData::WEARABLE_ABILITY);
#ifdef ENABLE_PENDANT_SYSTEM
	PyModule_AddIntConstant(poModule, "WEARABLE_PENDANT",			CItemData::WEARABLE_PENDANT);
#endif
#ifdef ENABLE_GLOVE_SYSTEM
	PyModule_AddIntConstant(poModule, "WEARABLE_GLOVE",				CItemData::WEARABLE_GLOVE);
#endif

	PyModule_AddIntConstant(poModule, "ARMOR_BODY",					CItemData::ARMOR_BODY);
	PyModule_AddIntConstant(poModule, "ARMOR_HEAD",					CItemData::ARMOR_HEAD);
	PyModule_AddIntConstant(poModule, "ARMOR_SHIELD",				CItemData::ARMOR_SHIELD);
	PyModule_AddIntConstant(poModule, "ARMOR_WRIST",				CItemData::ARMOR_WRIST);
	PyModule_AddIntConstant(poModule, "ARMOR_FOOTS",				CItemData::ARMOR_FOOTS);
	PyModule_AddIntConstant(poModule, "ARMOR_NECK",					CItemData::ARMOR_NECK);
	PyModule_AddIntConstant(poModule, "ARMOR_EAR",					CItemData::ARMOR_EAR);
#ifdef ENABLE_PENDANT_SYSTEM
	PyModule_AddIntConstant(poModule, "ARMOR_PENDANT",				CItemData::ARMOR_PENDANT);
#endif
#ifdef ENABLE_GLOVE_SYSTEM
	PyModule_AddIntConstant(poModule, "ARMOR_GLOVE",				CItemData::ARMOR_GLOVE);
#endif

	PyModule_AddIntConstant(poModule, "ITEM_APPLY_MAX_NUM",			CItemData::ITEM_APPLY_MAX_NUM);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_MAX_NUM",		CItemData::ITEM_SOCKET_MAX_NUM);

	PyModule_AddIntConstant(poModule, "APPLY_NONE",					CItemData::APPLY_NONE);
	PyModule_AddIntConstant(poModule, "APPLY_STR",					CItemData::APPLY_STR);
	PyModule_AddIntConstant(poModule, "APPLY_DEX",					CItemData::APPLY_DEX);
	PyModule_AddIntConstant(poModule, "APPLY_CON",					CItemData::APPLY_CON);
	PyModule_AddIntConstant(poModule, "APPLY_INT",					CItemData::APPLY_INT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP",				CItemData::APPLY_MAX_HP);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP",				CItemData::APPLY_MAX_SP);
	PyModule_AddIntConstant(poModule, "APPLY_HP_REGEN",				CItemData::APPLY_HP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_SP_REGEN",				CItemData::APPLY_SP_REGEN);
	PyModule_AddIntConstant(poModule, "APPLY_DEF_GRADE_BONUS",		CItemData::APPLY_DEF_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_GRADE_BONUS",		CItemData::APPLY_ATT_GRADE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ATT_SPEED",			CItemData::APPLY_ATT_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MOV_SPEED",			CItemData::APPLY_MOV_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_CAST_SPEED",			CItemData::APPLY_CAST_SPEED);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATT_GRADE",		CItemData::APPLY_MAGIC_ATT_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_DEF_GRADE",		CItemData::APPLY_MAGIC_DEF_GRADE);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL",				CItemData::APPLY_SKILL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ANIMAL",		CItemData::APPLY_ATTBONUS_ANIMAL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD",		CItemData::APPLY_ATTBONUS_UNDEAD);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL", 		CItemData::APPLY_ATTBONUS_DEVIL);
    PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_HUMAN",		CItemData::APPLY_ATTBONUS_HUMAN);
    PyModule_AddIntConstant(poModule, "APPLY_BOW_DISTANCE", 		CItemData::APPLY_BOW_DISTANCE);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_BOW", 			CItemData::APPLY_RESIST_BOW);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_FIRE", 			CItemData::APPLY_RESIST_FIRE);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_ELEC", 			CItemData::APPLY_RESIST_ELEC);
    PyModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC", 		CItemData::APPLY_RESIST_MAGIC);
    PyModule_AddIntConstant(poModule, "APPLY_POISON_PCT",			CItemData::APPLY_POISON_PCT);
    PyModule_AddIntConstant(poModule, "APPLY_SLOW_PCT", 			CItemData::APPLY_SLOW_PCT);
    PyModule_AddIntConstant(poModule, "APPLY_STUN_PCT", 			CItemData::APPLY_STUN_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_CRITICAL_PCT",			CItemData::APPLY_CRITICAL_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_PENETRATE_PCT",		CItemData::APPLY_PENETRATE_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ORC",			CItemData::APPLY_ATTBONUS_ORC);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MILGYO",		CItemData::APPLY_ATTBONUS_MILGYO);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_UNDEAD",		CItemData::APPLY_ATTBONUS_UNDEAD);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DEVIL",		CItemData::APPLY_ATTBONUS_DEVIL);
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_HP",				CItemData::APPLY_STEAL_HP);
	PyModule_AddIntConstant(poModule, "APPLY_STEAL_SP",				CItemData::APPLY_STEAL_SP);
	PyModule_AddIntConstant(poModule, "APPLY_MANA_BURN_PCT",		CItemData::APPLY_MANA_BURN_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_DAMAGE_SP_RECOVER",	CItemData::APPLY_DAMAGE_SP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_BLOCK",				CItemData::APPLY_BLOCK);
	PyModule_AddIntConstant(poModule, "APPLY_DODGE",				CItemData::APPLY_DODGE);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SWORD",			CItemData::APPLY_RESIST_SWORD);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_TWOHAND",		CItemData::APPLY_RESIST_TWOHAND);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DAGGER",		CItemData::APPLY_RESIST_DAGGER);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_BELL",			CItemData::APPLY_RESIST_BELL);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_FAN",			CItemData::APPLY_RESIST_FAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WIND",			CItemData::APPLY_RESIST_WIND);
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_MELEE",		CItemData::APPLY_REFLECT_MELEE);
	PyModule_AddIntConstant(poModule, "APPLY_REFLECT_CURSE",		CItemData::APPLY_REFLECT_CURSE);
	PyModule_AddIntConstant(poModule, "APPLY_POISON_REDUCE",		CItemData::APPLY_POISON_REDUCE);
	PyModule_AddIntConstant(poModule, "APPLY_KILL_SP_RECOVER",		CItemData::APPLY_KILL_SP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_EXP_DOUBLE_BONUS",		CItemData::APPLY_EXP_DOUBLE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_GOLD_DOUBLE_BONUS",	CItemData::APPLY_GOLD_DOUBLE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_ITEM_DROP_BONUS",		CItemData::APPLY_ITEM_DROP_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_POTION_BONUS",			CItemData::APPLY_POTION_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_KILL_HP_RECOVER",		CItemData::APPLY_KILL_HP_RECOVER);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_STUN",			CItemData::APPLY_IMMUNE_STUN);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_SLOW",			CItemData::APPLY_IMMUNE_SLOW);
	PyModule_AddIntConstant(poModule, "APPLY_IMMUNE_FALL",			CItemData::APPLY_IMMUNE_FALL);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_STAMINA",			CItemData::APPLY_MAX_STAMINA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WARRIOR",		CItemData::APPLY_ATT_BONUS_TO_WARRIOR);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_ASSASSIN",	CItemData::APPLY_ATT_BONUS_TO_ASSASSIN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SURA",		CItemData::APPLY_ATT_BONUS_TO_SURA);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SHAMAN",		CItemData::APPLY_ATT_BONUS_TO_SHAMAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_MONSTER",		CItemData::APPLY_ATT_BONUS_TO_MONSTER);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ATTBONUS",		CItemData::APPLY_MALL_ATTBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_DEFBONUS",		CItemData::APPLY_MALL_DEFBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_EXPBONUS",		CItemData::APPLY_MALL_EXPBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_ITEMBONUS",		CItemData::APPLY_MALL_ITEMBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MALL_GOLDBONUS",		CItemData::APPLY_MALL_GOLDBONUS);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_HP_PCT",			CItemData::APPLY_MAX_HP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_MAX_SP_PCT",			CItemData::APPLY_MAX_SP_PCT);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DAMAGE_BONUS",		CItemData::APPLY_SKILL_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DAMAGE_BONUS",	CItemData::APPLY_NORMAL_HIT_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_SKILL_DEFEND_BONUS",		CItemData::APPLY_SKILL_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_NORMAL_HIT_DEFEND_BONUS",	CItemData::APPLY_NORMAL_HIT_DEFEND_BONUS);

	PyModule_AddIntConstant(poModule, "APPLY_PC_BANG_EXP_BONUS",	CItemData::APPLY_PC_BANG_EXP_BONUS);
	PyModule_AddIntConstant(poModule, "APPLY_PC_BANG_DROP_BONUS",	CItemData::APPLY_PC_BANG_DROP_BONUS);

	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WARRIOR",	CItemData::APPLY_RESIST_WARRIOR );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ASSASSIN",	CItemData::APPLY_RESIST_ASSASSIN );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SURA",		CItemData::APPLY_RESIST_SURA );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_SHAMAN",	CItemData::APPLY_RESIST_SHAMAN );
	PyModule_AddIntConstant(poModule, "APPLY_ENERGY",	CItemData::APPLY_ENERGY );
	PyModule_AddIntConstant(poModule, "APPLY_COSTUME_ATTR_BONUS",	CItemData::APPLY_COSTUME_ATTR_BONUS );

	PyModule_AddIntConstant(poModule, "APPLY_MAGIC_ATTBONUS_PER",	CItemData::APPLY_MAGIC_ATTBONUS_PER );
	PyModule_AddIntConstant(poModule, "APPLY_MELEE_MAGIC_ATTBONUS_PER",	CItemData::APPLY_MELEE_MAGIC_ATTBONUS_PER );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_ICE",	CItemData::APPLY_RESIST_ICE );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_EARTH",	CItemData::APPLY_RESIST_EARTH );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_DARK",	CItemData::APPLY_RESIST_DARK );
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_CRITICAL_PCT",	CItemData::APPLY_ANTI_CRITICAL_PCT );
	PyModule_AddIntConstant(poModule, "APPLY_ANTI_PENETRATE_PCT",	CItemData::APPLY_ANTI_PENETRATE_PCT );
#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "APPLY_BLEEDING_PCT",	CItemData::APPLY_BLEEDING_PCT );
	PyModule_AddIntConstant(poModule, "APPLY_BLEEDING_REDUCE",	CItemData::APPLY_BLEEDING_REDUCE );
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_WOLFMAN",	CItemData::APPLY_ATT_BONUS_TO_WOLFMAN );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_WOLFMAN",	CItemData::APPLY_RESIST_WOLFMAN );
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_CLAW",	CItemData::APPLY_RESIST_CLAW );
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "APPLY_ACCEDRAIN_RATE",			CItemData::APPLY_ACCEDRAIN_RATE);
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MAGIC_REDUCTION",	CItemData::APPLY_RESIST_MAGIC_REDUCTION);
#endif

	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_ELECT", CItemData::APPLY_ENCHANT_ELECT);
	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_FIRE", CItemData::APPLY_ENCHANT_FIRE);
	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_ICE", CItemData::APPLY_ENCHANT_ICE);
	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_WIND", CItemData::APPLY_ENCHANT_WIND);
	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_EARTH", CItemData::APPLY_ENCHANT_EARTH);
	PyModule_AddIntConstant(poModule, "APPLY_ENCHANT_DARK", CItemData::APPLY_ENCHANT_DARK);

	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_CZ", CItemData::APPLY_ATTBONUS_CZ);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_INSECT", CItemData::APPLY_ATTBONUS_INSECT);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DESERT", CItemData::APPLY_ATTBONUS_DESERT);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_SWORD", CItemData::APPLY_ATTBONUS_SWORD);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_TWOHAND", CItemData::APPLY_ATTBONUS_TWOHAND);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_DAGGER", CItemData::APPLY_ATTBONUS_DAGGER);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BELL", CItemData::APPLY_ATTBONUS_BELL);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_FAN", CItemData::APPLY_ATTBONUS_FAN);
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BOW", CItemData::APPLY_ATTBONUS_BOW);
#ifdef ENABLE_WOLFMAN_CHARACTER
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_CLAW", CItemData::APPLY_ATTBONUS_CLAW);
#endif
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_HUMAN", CItemData::APPLY_RESIST_HUMAN);
	PyModule_AddIntConstant(poModule, "APPLY_RESIST_MOUNT_FALL", CItemData::APPLY_RESIST_MOUNT_FALL);
#ifdef ENABLE_BONUS_STONE
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_STONE", CItemData::APPLY_ATTBONUS_STONE);
#endif
#ifdef ENABLE_BONUS_BOSS
	PyModule_AddIntConstant(poModule, "APPLY_ATTBONUS_BOSS", CItemData::APPLY_ATTBONUS_BOSS);
#endif
#ifdef ENABLE_SEALBIND_SYSTEM
	PyModule_AddIntConstant(poModule, "E_SEAL_DATE_DEFAULT_TIMESTAMP",		CItemData::SEAL_DATE_DEFAULT_TIMESTAMP);
	PyModule_AddIntConstant(poModule, "E_SEAL_DATE_UNLIMITED_TIMESTAMP",	CItemData::SEAL_DATE_UNLIMITED_TIMESTAMP);
#endif

#ifdef ENABLE_PET_SYSTEM_EX
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PET", CItemData::ITEM_TYPE_PET);
	PyModule_AddIntConstant(poModule, "PET_EGG", CItemData::PET_EGG);
	PyModule_AddIntConstant(poModule, "PET_UPBRINGING", CItemData::PET_UPBRINGING);
	PyModule_AddIntConstant(poModule, "PET_BAG", CItemData::PET_BAG);
	PyModule_AddIntConstant(poModule, "PET_FEEDSTUFF", CItemData::PET_FEEDSTUFF);
	PyModule_AddIntConstant(poModule, "PET_SKILL", CItemData::PET_SKILL);
	PyModule_AddIntConstant(poModule, "PET_SKILL_DEL_BOOK", CItemData::PET_SKILL_DEL_BOOK);
	PyModule_AddIntConstant(poModule, "PET_NAME_CHANGE", CItemData::PET_NAME_CHANGE);
	PyModule_AddIntConstant(poModule, "PET_EXPFOOD", CItemData::PET_EXPFOOD);
	PyModule_AddIntConstant(poModule, "PET_SKILL_ALL_DEL_BOOK", CItemData::PET_SKILL_ALL_DEL_BOOK);
	PyModule_AddIntConstant(poModule, "PET_EXPFOOD_PER", CItemData::PET_EXPFOOD_PER);
	PyModule_AddIntConstant(poModule, "PET_ATTR_DETERMINE", CItemData::PET_ATTR_DETERMINE);
	PyModule_AddIntConstant(poModule, "PET_ATTR_CHANGE", CItemData::PET_ATTR_CHANGE);
	PyModule_AddIntConstant(poModule, "PET_PAY", CItemData::PET_PAY);
	PyModule_AddIntConstant(poModule, "PET_PRIMIUM_FEEDSTUFF", CItemData::PET_PRIMIUM_FEEDSTUFF);
#endif

#ifdef ENABLE_SPECIAL_STORAGE
	PyModule_AddIntConstant(poModule, "ITEM_FLAG_GENERAL", CItemData::ITEM_FLAG_GENERAL);
#endif
#ifdef NEW_PVM_ATTRIBUTE
	PyModule_AddIntConstant(poModule, "APPLY_PVM_NORM_HIT_BONUS", CItemData::APPLY_PVM_NORM_HIT_BONUS);
#endif

}
// vaffanculo a chi t'e morto martysama
