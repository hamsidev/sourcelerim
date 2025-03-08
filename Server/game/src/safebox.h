#ifndef __INC_METIN_II_GAME_SAFEBOX_H__
#define __INC_METIN_II_GAME_SAFEBOX_H__
#include <memory>

class CHARACTER;
class CItem;
class CGrid;

class CSafebox
{
	public:
		CSafebox(LPCHARACTER pkChrOwner, int iSize, DWORD dwGold);
		~CSafebox();

		bool		Add(DWORD dwPos, LPITEM pkItem);
		LPITEM		Get(DWORD dwPos);
		LPITEM		Remove(DWORD dwPos);
		void		ChangeSize(int iSize);

		bool		MoveItem(BYTE bCell, BYTE bDestCell
#ifdef ENABLE_ITEM_COUNT_LIMIT_SYSTEM
		, DWORD count
#else
		, BYTE count
#endif
		);

		LPITEM		GetItem(BYTE bCell);

		void		Save();

		bool		IsEmpty(DWORD dwPos, BYTE bSize);
		bool		IsValidPosition(DWORD dwPos);

		void		SetWindowMode(BYTE bWindowMode);

	protected:
		void		__Destroy();

		LPCHARACTER	m_pkChrOwner;
		LPITEM		m_pkItems[SAFEBOX_MAX_NUM];
		std::unique_ptr<CGrid>		m_pkGrid; // @fixme191
		int		m_iSize;
		long		m_lGold;

		BYTE		m_bWindowMode;
};

#endif
// vaffanculo a chi t'e morto martysama
