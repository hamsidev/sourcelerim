#pragma once

#include <memory>
#include "GrpRenderTargetTexture.h"

class CInstanceBase;
class CGraphicImageInstance;

class CRenderTarget
{
	using TCharacterInstanceMap = std::map<DWORD, CInstanceBase*>;

public:
	CRenderTarget(DWORD width, DWORD height);
	~CRenderTarget();

	void SetVisibility(bool isShow);
	void RenderTexture() const;
	void SetRenderingRect(RECT* rect) const;

	void SelectModel(long long index);
	void SetScale(float x, float y, float z);
	bool CreateBackground(const char* imgPath, DWORD width, DWORD height);
	void RenderBackground() const;
	void UpdateModel();
	void DeformModel() const;
	void RenderModel() const;
	void SetWeapon(DWORD dwVnum);
	void SetArmor(DWORD vnum);
	void SetAcce(DWORD vnum);
#ifdef	ENABLE_AURA_SYSTEM
	void SetAura(DWORD vnum);
#endif
#ifdef ENABLE_WINGS_NEW
	void SetWing(DWORD dwVnum);
#endif
	void ChangeHair(DWORD vnum);
	void CreateTextures() const;
	void ReleaseTextures() const;

	DWORD GetVisibility() { return m_visible; }
	DWORD GetModel() { return dwModel; }
	DWORD GetWeapon() { return dwWeapon; }
	DWORD GetArmor() { return dwArmor; }
	DWORD GetAcce() { return dwAcce; }
#ifdef	ENABLE_AURA_SYSTEM
	DWORD GetAura() { return dwAura; }
#endif
	DWORD GetHair() { return dwHair; }
#ifdef ENABLE_WINGS_NEW
	const DWORD GetWing() { return dwWing; }
#endif
	void SetDefaultModelPart(BYTE type);
private:
	std::unique_ptr<CInstanceBase> m_pModel;
	std::unique_ptr<CGraphicImageInstance> m_background;
	std::unique_ptr<CGraphicRenderTargetTexture> m_renderTargetTexture;
	float m_modelRotation;
	bool m_visible;

	long long dwModel;
	DWORD dwWeapon;
	DWORD dwArmor;
	DWORD dwAcce;
#ifdef	ENABLE_AURA_SYSTEM
	DWORD dwAura;
#endif
	DWORD dwHair;
#ifdef ENABLE_WINGS_NEW
	DWORD dwWing;
#endif
};
