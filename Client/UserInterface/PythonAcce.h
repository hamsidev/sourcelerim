#pragma once

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
#include "Packet.h"

class CPythonAcce : public CSingleton<CPythonAcce>
{
public:
	long long	dwPrice;
	typedef std::vector<TAcceMaterial> TAcceMaterials;

public:
	CPythonAcce();
	virtual ~CPythonAcce();

	void	Clear();
	void	AddMaterial(long long  dwRefPrice, BYTE bPos, TItemPos tPos);
	void	AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs);
	void	RemoveMaterial(long long dwRefPrice, BYTE bPos);
	long long	GetPrice() {return dwPrice;}
	bool	GetAttachedItem(BYTE bPos, BYTE & bHere, WORD & wCell);
	void	GetResultItem(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs);

protected:
	TAcceResult	m_vAcceResult;
	TAcceMaterials	m_vAcceMaterials;
};
#endif
// vaffanculo a chi t'e morto martysama
