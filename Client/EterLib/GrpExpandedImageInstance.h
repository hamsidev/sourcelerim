#pragma once

#include "GrpImageInstance.h"

class CGraphicExpandedImageInstance : public CGraphicImageInstance
{
	public:
		static DWORD Type();
		static void DeleteExpandedImageInstance(CGraphicExpandedImageInstance * pkInstance)
		{
			pkInstance->Destroy();
			ms_kPool.Free(pkInstance);
		}

		enum ERenderingMode
		{
			RENDERING_MODE_NORMAL,
			RENDERING_MODE_SCREEN,
			RENDERING_MODE_COLOR_DODGE,
			RENDERING_MODE_MODULATE,
		};

	public:
		CGraphicExpandedImageInstance();
		virtual ~CGraphicExpandedImageInstance();

		void Destroy();

		void SetDepth(float fDepth);
		void SetOrigin();
		void SetOrigin(float fx, float fy);
		void SetRotation(float fRotation);
		void SetScale(float fx, float fy);
		void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
		void SetRenderingMode(int iMode);

	protected:
		void Initialize();

		void OnRender();
		void OnSetImagePointer();

		BOOL OnIsType(DWORD dwType);

	protected:
		float m_fDepth;
		D3DXVECTOR2 m_v2Origin;
#define ENABLE_MODEL_RENDER_TARGET
#ifndef ENABLE_MODEL_RENDER_TARGET
		D3DXVECTOR2 m_v2Scale;
#endif
		float m_fRotation;
		RECT m_RenderingRect;
		int m_iRenderingMode;

	public:
		static void CreateSystem(UINT uCapacity);
		static void DestroySystem();

		static CGraphicExpandedImageInstance* New();
		static void Delete(CGraphicExpandedImageInstance* pkImgInst);

		static CDynamicPool<CGraphicExpandedImageInstance>		ms_kPool;
};
// vaffanculo a chi t'e morto martysama
