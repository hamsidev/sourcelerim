#ifndef __INC_METIN_II_GAME_SHOP_MANAGER_H__
#define __INC_METIN_II_GAME_SHOP_MANAGER_H__

class CShop;
typedef class CShop * LPSHOP;

class CShopManager : public singleton<CShopManager>
{
public:
	typedef std::map<DWORD, CShop *> TShopMap;

public:
	CShopManager();
	virtual ~CShopManager();

	bool	Initialize(TShopTable * table, int size);
	void	Destroy();

	LPSHOP	Get(DWORD dwVnum);
	LPSHOP	GetByNPCVnum(DWORD dwVnum);

	bool	StartShopping(LPCHARACTER pkChr, LPCHARACTER pkShopKeeper, int iShopVnum = 0);
	void	StopShopping(LPCHARACTER ch);

	void	Buy(LPCHARACTER ch, BYTE pos);
	
	void	Sell(LPCHARACTER ch, BYTE bCell
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	, DWORD bCount = 0
#else
	, BYTE bCount=0
#endif
#ifdef ENABLE_SPECIAL_STORAGE
		, BYTE bType = 0
#endif
	);

	LPSHOP	CreatePCShop(LPCHARACTER ch, TShopItemTable * pTable
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
	, DWORD bItemCount
#else
	, BYTE bItemCount
#endif
	);

	LPSHOP	FindPCShop(DWORD dwVID);
	void	DestroyPCShop(LPCHARACTER ch);

private:
	TShopMap	m_map_pkShop;
	TShopMap	m_map_pkShopByNPCVnum;
	TShopMap	m_map_pkShopByPC;

	bool	ReadShopTableEx(const char* stFileName);
};

#endif
// vaffanculo a chi t'e morto martysama
