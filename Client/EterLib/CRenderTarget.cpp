#include "StdAfx.h"
#include "CRenderTarget.h"
#include "../EterLib/Camera.h"
#include "../EterLib/CRenderTargetManager.h"
#include "../EterPythonLib/PythonGraphic.h"

#include "../EterBase/CRC32.h"
#include "../GameLib/GameType.h"
#include "../GameLib/MapType.h"
#include "../GameLib/ItemData.h"
#include "../GameLib/ActorInstance.h"
#include "../UserInterface/InstanceBase.h"

#include "ResourceManager.h"

#include "../UserInterface/StdAfx.h"
#include "../UserInterface/GameType.h"
#include "../UserInterface/PythonPlayer.h"
#include "../GameLib/ItemManager.h"


CRenderTarget::CRenderTarget(const DWORD width, const DWORD height) :
	m_pModel(nullptr),
	m_background(nullptr),
	m_modelRotation(0),
	m_visible(false)
	, dwModel(-1)
	, dwWeapon(0)
	, dwArmor(0)
	, dwAcce(0)
#ifdef	ENABLE_AURA_SYSTEM
	, dwAura(0)
#endif
	, dwHair(0)
#ifdef ENABLE_WINGS_NEW
	, dwWing(0)
#endif
{
	auto pTex = new CGraphicRenderTargetTexture;
	if (!pTex->Create(width, height, D3DFMT_X8R8G8B8, D3DFMT_D16)) {
		delete pTex;
		TraceError("CRenderTarget::CRenderTarget: Could not create CGraphicRenderTargetTexture %dx%d", width, height);
		throw std::runtime_error("CRenderTarget::CRenderTarget: Could not create CGraphicRenderTargetTexture");
	}

	m_renderTargetTexture = std::unique_ptr<CGraphicRenderTargetTexture>(pTex);
}

CRenderTarget::~CRenderTarget()
{
}

void CRenderTarget::SetVisibility(bool isShow)
{
	m_visible = isShow;
	if (isShow)
	{
		if (m_pModel)
		{
			m_pModel->SetAlwaysRender(true);
		}
		//m_modelRotation = 0.0f;
		//m_pModel->SetRotation(m_modelRotation);
	}
	else
	{
		m_pModel->SetAlwaysRender(false);
		//m_modelRotation = 0.0f;
		//m_pModel->SetRotation(m_modelRotation);
	}
}

void CRenderTarget::RenderTexture() const
{
	m_renderTargetTexture->Render();
}

void CRenderTarget::SetRenderingRect(RECT* rect) const
{
	m_renderTargetTexture->SetRenderingRect(rect);
}

void CRenderTarget::CreateTextures() const
{
	m_renderTargetTexture->CreateTextures();
}

void CRenderTarget::ReleaseTextures() const
{
	m_renderTargetTexture->ReleaseTextures();
}

void CRenderTarget::SelectModel(long long index)
{
	if (index == -1)
{
		m_pModel->SetAlwaysRender(false);
		m_modelRotation = 0.0f;
		m_pModel->SetRotation(m_modelRotation);
		//delete m_pModel;
		m_pModel.reset();
		dwModel = -1;
		dwWeapon = 0;
		dwArmor = 0;
		dwAcce = 0;
		// dwAura = 0;
		dwHair = 0;
#ifdef ENABLE_WINGS_NEW
		dwWing = 0;
#endif
		return;
	}

	CInstanceBase::SCreateData kCreateData{};

	kCreateData.m_bType = index < 9 ? CActorInstance::TYPE_PC : CActorInstance::TYPE_NPC;
	kCreateData.m_dwRace = index;
	dwModel = index;

	auto model = std::make_unique<CInstanceBase>();
	if (!model->Create(kCreateData))
	{
		if (m_pModel)
		{
			m_pModel.reset();
			//dwModel = -1;
		}
		return;
	}
	m_pModel = std::move(model);

	m_pModel->NEW_SetPixelPosition(TPixelPosition(0, 0, 0));

	if (index < 10)
	{
		m_pModel->GetGraphicThingInstancePtr()->ClearAttachingEffect();
		//m_modelRotation = 0.0f;
		//m_pModel->Refresh(CRaceMotionData::NAME_WAIT, true);
		//m_pModel->SetLoopMotion(CRaceMotionData::NAME_WAIT);
		//m_pModel->SetRotation(0.0f);
	}
	else
	{
		dwWeapon = 0;
		dwArmor = 0;
		dwAcce = 0;
		dwHair = 0;
		// dwAura = 0;
#ifdef ENABLE_WINGS_NEW
		dwWing = 0;
#endif
	}

	m_pModel->SetAlwaysRender(true);

	auto& camera_manager = CCameraManager::instance();
	camera_manager.SetCurrentCamera(CCameraManager::SHOPDECO_CAMERA);
	camera_manager.GetCurrentCamera()->SetTargetHeight(110.0);
	camera_manager.ResetToPreviousCamera();


	if (index < 10)
	{
		SetDefaultModelPart(0);
	}
}

bool CRenderTarget::CreateBackground(const char* imgPath, const DWORD width, const DWORD height)
{
	if (m_background)
		return false;

	m_background = std::make_unique<CGraphicImageInstance>();

	const auto graphic_image = dynamic_cast<CGraphicImage*>(CResourceManager::instance().GetResourcePointer(imgPath));
	if (!graphic_image)
	{
		m_background.reset();
		return false;
	}

	m_background->SetImagePointer(graphic_image);
	m_background->SetScale(static_cast<float>(width) / graphic_image->GetWidth(), static_cast<float>(height) / graphic_image->GetHeight());
	return true;
}

void CRenderTarget::RenderBackground() const
{
	if (!m_visible)
		return;

	if (!m_background)
		return;

	auto& rectRender = *m_renderTargetTexture->GetRenderingRect();
	m_renderTargetTexture->SetRenderTarget();

	CGraphicRenderTargetTexture::Clear();
	CPythonGraphic::Instance().SetInterfaceRenderState();

	//const auto width = static_cast<float>(rectRender.right - rectRender.left);
	//const auto height = static_cast<float>(rectRender.bottom - rectRender.top);

	//CPythonGraphic::Instance().SetViewport(0.0f, 0.0f, width, height);

	m_background->Render();
	m_renderTargetTexture->ResetRenderTarget();
	//CPythonGraphic::Instance().RestoreViewport();
}

void CRenderTarget::UpdateModel()
{
	if (!m_visible || !m_pModel)
		return;

	if (m_modelRotation < 360.0f)
		m_modelRotation += 1.0f;
	else
		m_modelRotation = 0.0f;

	m_pModel->SetRotation(m_modelRotation);
	m_pModel->Transform();
	m_pModel->GetGraphicThingInstanceRef().RotationProcess();
}

void CRenderTarget::DeformModel() const
{
	if (!m_visible)
		return;

	if (m_pModel)
		m_pModel->Deform();
}

void CRenderTarget::RenderModel() const
{
	if (!m_visible)
	{
		return;
	}

	auto& python_graphic = CPythonGraphic::Instance();
	auto& camera_manager = CCameraManager::instance();
	auto& state_manager = CStateManager::Instance();

	auto& rectRender = *m_renderTargetTexture->GetRenderingRect();

	if (!m_pModel)
	{
		return;
	}

	m_renderTargetTexture->SetRenderTarget();

	if (!m_background)
	{
		m_renderTargetTexture->Clear();
	}

	python_graphic.ClearDepthBuffer();

	const auto fov = python_graphic.GetFOV();
	const auto aspect = python_graphic.GetAspect();
	const auto near_y = python_graphic.GetNear();
	const auto far_y = python_graphic.GetFar();

	const auto width = static_cast<float>(rectRender.right - rectRender.left);
	const auto height = static_cast<float>(rectRender.bottom - rectRender.top);

	state_manager.SetRenderState(D3DRS_FOGENABLE, FALSE);

	python_graphic.SetViewport(0.0f, 0.0f, width, height);

	python_graphic.PushState();

	camera_manager.SetCurrentCamera(CCameraManager::SHOPDECO_CAMERA);
	camera_manager.GetCurrentCamera()->SetViewParams(
		D3DXVECTOR3{ 0.0f, -1500.0f, 600.0f },
		D3DXVECTOR3{ 0.0f, 0.0f, 95.0f },
		D3DXVECTOR3{ 0.0f, 0.0f, 1.0f }
	);

	python_graphic.UpdateViewMatrix();

	python_graphic.SetPerspective(10.0f, width / height, 100.0f, 3000.0f);

	m_pModel->Render();
	m_pModel->GetGraphicThingInstanceRef().RenderAllAttachingEffect();
	camera_manager.ResetToPreviousCamera();
	python_graphic.RestoreViewport();
	python_graphic.PopState();
	python_graphic.SetPerspective(fov, aspect, near_y, far_y);
	m_renderTargetTexture->ResetRenderTarget();
	state_manager.SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void CRenderTarget::SetScale(float x, float y, float z)
{
	m_pModel->GetGraphicThingInstancePtr()->SetScale(x, y, z);
}

void CRenderTarget::SetArmor(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwArmor > 0)
	{
		m_pModel->ChangeArmor(0);
		dwArmor = 0;
		return;
	}

	SetDefaultModelPart(1);

	if (dwArmor != vnum)
	{
		m_pModel->ChangeArmor(vnum);
		dwArmor = vnum;

		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Acce));

		if (dwAcce != ItemIndex || m_pModel->GetPart(CRaceData::PART_ACCE) != dwAcce)
		{
			m_pModel->ChangeAcce(ItemIndex - 85000);

			dwAcce = ItemIndex;
		}
	}
}

void CRenderTarget::SetWeapon(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwWeapon > 0)
	{
		m_pModel->ChangeWeapon(0);
		dwWeapon = 0;
		return;
	}

	SetDefaultModelPart(2);

	if (dwWeapon != vnum)
	{
		m_pModel->ChangeWeapon(0);
		m_pModel->ChangeWeapon(vnum);
		//m_pModel->RefreshState(CRaceMotionData::NAME_WAIT, TRUE);
		dwWeapon = vnum;
	}
}


void CRenderTarget::ChangeHair(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwHair > 0)
	{
		m_pModel->ChangeHair(0);
		dwHair = 0;
		return;
	}

	SetDefaultModelPart(3);

	if (dwHair != vnum)
	{
		if (vnum == 0)
		{
			m_pModel->ChangeHair(0);
			dwHair = 0;
		}
		else
		{
			CItemData* pItemData;
			if (CItemManager::Instance().GetItemDataPointer(vnum, &pItemData))
			{
				m_pModel->ChangeHair(pItemData->GetValue(3));
				dwHair = vnum;
			}
			else
			{
				m_pModel->ChangeHair(vnum);
				dwHair = vnum;
			}
		}
	}
}

void CRenderTarget::SetAcce(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwAcce > 0)
	{
		m_pModel->ChangeAcce(0);
		dwAcce = 0;
		return;
	}

	SetDefaultModelPart(4);

	if (dwAcce != vnum || m_pModel->GetPart(CRaceData::PART_ACCE) != dwAcce)
	{
		m_pModel->ChangeAcce(vnum); // eypcn render göstermiyorsa burayı vnum olarak yap yanındaki sayıyı sil
		dwAcce = vnum;
	}
}

#ifdef	ENABLE_AURA_SYSTEM
void CRenderTarget::SetAura(DWORD vnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwAura > 0)
	{
		m_pModel->ChangeAura(0);
		dwAura = 0;
		return;
	}

	SetDefaultModelPart(5);

	if (dwAura != vnum || m_pModel->GetPart(CRaceData::PART_AURA) != dwAura)
	{
		m_pModel->ChangeAura(vnum);
		dwAura = vnum;
	}
}
#endif

#ifdef ENABLE_WINGS_NEW
void CRenderTarget::SetWing(DWORD dwVnum)
{
	if (!m_visible || !m_pModel)
		return;

	if (m_pModel->GetRace() >= 9 && dwWing > 0)
	{
		m_pModel->ChangeWing(0);
		dwWing = 0;
		return;
	}

	SetDefaultModelPart(5);

	if (dwWing != dwVnum || m_pModel->GetPart(CRaceData::PART_WING) != dwWing)
	{
		m_pModel->ChangeWing(dwVnum);
		dwWing = dwVnum;
	}
}
#endif

void CRenderTarget::SetDefaultModelPart(BYTE type)
{
	if (type != 1)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Body));
		if (ItemIndex == 0)
		{
			ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Equipment_Body));
			//m_pModel->ChangeArmor(ItemIndex);
			//dwArmor = ItemIndex;
		}
		if (dwArmor != ItemIndex)
		{
			m_pModel->ChangeArmor(ItemIndex);
			dwArmor = ItemIndex;
		}
	}
	
	if (type != 2)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Start + CItemData::COSTUME_WEAPON));
		if (ItemIndex == 0)
			ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Equipment_Weapon));

		if (dwWeapon != ItemIndex)
		{
			m_pModel->ChangeWeapon(0);
			m_pModel->ChangeWeapon(ItemIndex);
			dwWeapon = ItemIndex;
		}
	}

	if (type != 3)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Hair));

		if (dwHair != ItemIndex)
		{
			if (ItemIndex == 0)
			{
				m_pModel->ChangeHair(0);
				dwHair = ItemIndex;
			}
			else
			{
				CItemData* pItemData;
				if (CItemManager::Instance().GetItemDataPointer(ItemIndex, &pItemData))
				{
					m_pModel->ChangeHair(pItemData->GetValue(3));
					dwHair = ItemIndex;
				}
			}
			
		}
	}

	if (type != 4)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Acce));

		if (dwAcce != ItemIndex || m_pModel->GetPart(CRaceData::PART_ACCE) != dwAcce)
		{
			m_pModel->ChangeAcce(ItemIndex - 85000);
			
			dwAcce = ItemIndex;
		}
	}


#ifdef	ENABLE_AURA_SYSTEM
	if (type != 5)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Aura));

		if (dwAura != ItemIndex || m_pModel->GetPart(CRaceData::PART_AURA) != dwAura)
		{
			m_pModel->ChangeAura(ItemIndex);

			dwAura = ItemIndex;
		}
	}
#endif

#ifdef ENABLE_WINGS_NEW
	if (type != 6)
	{
		int ItemIndex = 0;
		ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos(INVENTORY, c_Costume_Slot_Wing));
		if (dwWing != ItemIndex || m_pModel->GetPart(CRaceData::PART_WING) != dwWing)
		{
			m_pModel->ChangeWing(ItemIndex);
			dwWing = ItemIndex;
		}
	}
#endif

}
