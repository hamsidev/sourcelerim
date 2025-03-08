class CRonarkMapManager : public singleton<CRonarkMapManager>
{
public:
	typedef struct STeamData
	{
		DWORD	dwID;
		CGuild* pkGuild;
		int		iMemberCount;
		DWORD		iDamage;

		std::set<DWORD> set_pidJoiner;

		void Initialize();

		int GetAccumulatedJoinerCount();
		int GetCurJointerCount();

		void AppendMember(LPCHARACTER ch);
		void RemoveMember(LPCHARACTER ch);
		void AppendDamage(int damage);
	} TeamData;

	enum ERonarkStatus
	{
		RONARK_STATUS_CLOSED = 0,
		RONARK_STATUS_OPENED = 1,
		RONARK_STATUS_REWARD = 2,
	};

	typedef std::map<DWORD, TeamData*> TeamDataMap;
public:
	CRonarkMapManager(void);
	virtual ~CRonarkMapManager(void);

	TeamData* GetTable(DWORD dwID);

	DWORD GetGuildDamage(DWORD dwID);
	DWORD GetGuildMemberCount(DWORD dwID);

	void IncMember(LPCHARACTER ch);
	void DecMember(LPCHARACTER ch);

	void AppendGuild(CGuild* guild);
	void OnDamage(CGuild* guild, int damage);

	void SendScorePacket();

	void StartLoginEvent();
	void StartEvent();
	void SpawnRonark();
	void SetRonarkStatus(int iStatus, int iMap);
	int GetRonarkStatus();
	bool IsRonarkMap(int mapindex);
	bool IsRonarkActivate();

	void SetMemberLimitCount(int memberlimit);
	int GetMemberLimitCount();
	int GetTotalMemberCount() { return m_set_pkChr.size(); }
	DWORD GetWinnerGuild();

	void	SetBossVID(DWORD dwVID) { m_ibossVID = dwVID; }
	DWORD	GetBossVID() { return m_ibossVID; }

	void	SetRewardVID(DWORD dwVID) { m_irewardVID = dwVID; }
	DWORD	GetRewardVID() { return m_irewardVID; }

	void Packet(const void* pv, int size);
	void Notice(const char* psz);
	void SendHomeWithoutWinner();
	void SendHome();

	void DelEvent();

	void Reward();
	void ResetEvent();

	void Destroy();
protected:
	TeamDataMap	m_TeamDataMap;
	int m_iRonarkMapIndex;
	int m_iMemberLimitCount;
	int m_iActivateStatus;
	DWORD m_ibossVID;
	DWORD m_irewardVID;
	LPEVENT destroyEvent;
private:
	CHARACTER_SET m_set_pkChr;
};