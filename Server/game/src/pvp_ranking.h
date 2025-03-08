#pragma once

class CPvPRanking : public singleton<CPvPRanking>
{
		struct TPvPInfo
		{
			int iKill;
			int iDead;

			TPvPInfo (int iMyKill, int iMyDead)
				: iKill (iMyKill), iDead (iMyDead)
			{}
		};
		typedef std::map<VID, TPvPInfo>	TPvPRankMap;
		#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
		typedef std::map<BYTE, long long>	TEmpireRankMap;
		#endif

	private:
		BYTE	m_pStatus;
		int		m_pMaxKill;
		BYTE	m_pIsEnd;
		BYTE	m_pTimeEvent;

	public:
		CPvPRanking (void);
		virtual ~CPvPRanking (void);

		void	AddKill (LPCHARACTER ch);
		void	AddDead (LPCHARACTER ch);
		void	AddMember (LPCHARACTER ch);
		void	Check();
		void	Destroy();
		bool	CheckPlayerLevel (LPCHARACTER ch);
		void	End();
		void	Start (BYTE minLevel, BYTE maxLevel, int maxKill, DWORD mapIndex);
		BYTE	GetStatus();
		BYTE	GetMaxLevel();
		BYTE	GetMinLevel();
		DWORD	GetMapIndex();
		#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
		BYTE	IsRankEmpire();
		#endif
		void	GoHome();
		void	TimeEvent();
		void	WarpToBase();
		void	Reset();

	protected:
		TPvPRankMap			m_map_pvpRank;
		#ifdef ENABLE_PVP_RANKING_WITH_EMPIRE
		TEmpireRankMap		m_map_empireRank;
		#endif
		std::set<DWORD>		m_pvp_Member;
};