#include "StdAfx.h"
#include "PythonApplication.h"

PyObject* renderTargetSelectModel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SelectModel(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetSetVisibility(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	bool isShow = false;
	if (!PyTuple_GetBoolean(poArgs, 1, &isShow))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetVisibility(isShow);

	return Py_BuildNone();
}

PyObject* renderTargetGetVisibility(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetVisibility() ? 1 : 0);
}

PyObject* renderTargetSetBackground(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	char* szPathName;
	if (!PyTuple_GetString(poArgs, 1, &szPathName))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->CreateBackground(
		szPathName, CPythonApplication::Instance().GetWidth(),
		CPythonApplication::Instance().GetHeight());
	return Py_BuildNone();
}

PyObject* renderTargetSetScale(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	float xScale;
	if (!PyTuple_GetFloat(poArgs, 1, &xScale))
		return Py_BadArgument();
	float yScale;
	if (!PyTuple_GetFloat(poArgs, 2, &yScale))
		return Py_BadArgument();
	float zScale;
	if (!PyTuple_GetFloat(poArgs, 3, &zScale))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetScale(zScale, yScale, xScale);
	return Py_BuildNone();
}

PyObject* renderTargetSetArmor(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	if (!pInstance)
		return Py_BuildNone();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetArmor(modelIndex);
	//	DWORD weaponIndex = pInstance->GetPart(CRaceData::PART_WEAPON);
	//#ifdef ENABLE_ACCE_SYSTEM
	//	DWORD acceIndex = pInstance->GetPart(CRaceData::PART_ACCE);
	//#endif
	//	CRenderTargetManager::Instance().GetRenderTarget(index)->SetWeapon(weaponIndex, 0);
	//#ifdef ENABLE_ACCE_SYSTEM
	//	CRenderTargetManager::Instance().GetRenderTarget(index)->SetAcce(acceIndex); // oder SetSash, je nach dem
	//#endif
	return Py_BuildNone();
}

PyObject* renderTargetSetAcce(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();
	CRenderTargetManager::Instance().GetRenderTarget(index)->SetAcce(modelIndex); // oder SetSash, je nach dem

	return Py_BuildNone();
}

#ifdef	ENABLE_AURA_SYSTEM
PyObject* renderTargetSetAura(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();
	CRenderTargetManager::Instance().GetRenderTarget(index)->SetAura(modelIndex); // oder SetSash, je nach dem

	return Py_BuildNone();
}
#endif

PyObject* renderTargetSetWeapon(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	//CRenderTargetManager::Instance().GetRenderTarget(index)->SetWeapon(0, 0);
	CRenderTargetManager::Instance().GetRenderTarget(index)->SetWeapon(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetSetHair(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeHair(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetGetModel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetModel());
}

PyObject* renderTargetGetArmor(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetArmor());
}

PyObject* renderTargetGetAcce(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetAcce());
}
#ifdef	ENABLE_AURA_SYSTEM
PyObject* renderTargetGetAura(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetAura());
}
#endif	

PyObject* renderTargetGetWeapon(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetWeapon());
}
PyObject* renderTargetGetHair(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetHair());
}

int renderTargetItemList(int itemVnum)
{
	if (itemVnum >= 360 && itemVnum <= 429)
		return 1;
	if (itemVnum >= 40210 && itemVnum <= 40242)
		return 1;
	if (itemVnum >= 40255 && itemVnum <= 40294)
		return 1;
	if (itemVnum >= 40315 && itemVnum <= 40338)
		return 1;
	if (itemVnum >= 86059 && itemVnum <= 86064)
		return 1;
	if (itemVnum >= 88001 && itemVnum <= 88009)
		return 1;

	return 0;
}

PyObject* renderTargetIsShow(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetVisibility() ? 1 : 0);
}

#ifdef ENABLE_WINGS_NEW
PyObject* renderTargetSetWing(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();
	CRenderTargetManager::Instance().GetRenderTarget(index)->SetWing(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetGetWing(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	return Py_BuildValue("i", CRenderTargetManager::Instance().GetRenderTarget(index)->GetWing());
}
#endif

void initRenderTarget() {
	static PyMethodDef s_methods[] =
	{
		{ "SelectModel", renderTargetSelectModel, METH_VARARGS },
		{ "SetVisibility", renderTargetSetVisibility, METH_VARARGS },
		{ "GetVisibility", renderTargetGetVisibility, METH_VARARGS },
		{ "SetBackground", renderTargetSetBackground, METH_VARARGS },
		{ "SetScale",	renderTargetSetScale,	METH_VARARGS },
		{ "SetArmor", renderTargetSetArmor, METH_VARARGS },
		{ "SetWeapon", renderTargetSetWeapon, METH_VARARGS },
		{ "SetAcce", renderTargetSetAcce, METH_VARARGS },
		// { "SetAura", renderTargetSetAura, METH_VARARGS },
#ifdef	ENABLE_DSPROJECT_ITEM_SYSTEM
		{ "SetDsproject", renderTargetSetDsproject, METH_VARARGS },
#endif
#ifdef	ENABLE_DSPROJECT_ARMOR_SYSTEM
		{ "SetDsprojectArmor", renderTargetSetDsprojectArmor, METH_VARARGS },
#endif
#ifdef	ENABLE_DSPROJECT_WEAPON_SYSTEM
		{ "SetDsprojectWeapon", renderTargetSetDsprojectWeapon, METH_VARARGS },
#endif
		{ "SetHair", renderTargetSetHair, METH_VARARGS },
		{ "GetModel", renderTargetGetModel, METH_VARARGS },
		{ "GetArmor", renderTargetGetArmor, METH_VARARGS },
		{ "GetWeapon", renderTargetGetWeapon, METH_VARARGS },
		{ "GetAcce", renderTargetGetAcce, METH_VARARGS },
		// { "GetAura", renderTargetGetAura, METH_VARARGS },
		{ "GetHair", renderTargetGetHair, METH_VARARGS },
		{ "IsShow", renderTargetIsShow, METH_VARARGS },
#ifdef ENABLE_WINGS_NEW
		{ "SetWing", renderTargetSetWing, METH_VARARGS },
		{ "GetWing", renderTargetGetWing, METH_VARARGS },
#endif
		{NULL, NULL, 0 },
	};

	PyObject* poModule = Py_InitModule("renderTarget", s_methods);
}