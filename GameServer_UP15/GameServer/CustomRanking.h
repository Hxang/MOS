// ---
#include "Protocol.h"
// ---
#define MAX_RANK  15
// ---
struct CUSTOM_RANKING
{
	int index;
	char Name[60];
	char Col1[20];
	char Col2[20];
	char Col3[20];
};
// ---
struct CUSTOM_RANKING_DATA
{
	char Value1[20];
	char Value2[30];
	char Value3[30];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//
struct SDHP_CUSTOM_RANKING_SEND
{
	PBMSG_HEAD header;
	WORD index;
	WORD type;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//
struct SDHP_CUSTOM_RANKING_RECV
{
	PWMSG_HEAD header;
	int index;
	int type;
	int count;
};

//**********************************************//
//********** GameServer -> Cliente    **********//
//**********************************************//

struct PMSG_CUSTOM_RANKING_SEND
{
	PSWMSG_HEAD header;
	int index;
	char rankname[60];
	char col1[20];
	char col2[20];
	char col3[20];
	int count;
};

struct PMSG_CUSTOM_RANKING_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:BF:51
	int count;
	int CustomRankingSwitch;
};

//**********************************************//
//********** Cliente -> GameServer    **********//
//**********************************************//

struct PMSG_CUSTOM_RANKING_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:BF:51
};

struct PMSG_CUSTOM_RANKING_RECV
{
	PSBMSG_HEAD header; // C1:BF:51
	BYTE type;
};

// ---
class CCustomRanking
{
public:
	void Load(char* path);
	void GCReqRanking(int Index, PMSG_CUSTOM_RANKING_RECV* pMsg);
	void GCReqRankingCount(int Index, PMSG_CUSTOM_RANKING_COUNT_RECV* lpMsg);
	// ---
	int GetRankIndex(int aIndex);
	void CheckUpdate(LPOBJ lpObj);
	void GDCustomRankingRecv(SDHP_CUSTOM_RANKING_RECV* lpMsg);

private:
	int m_count;
	// ---
	CUSTOM_RANKING r_Data[MAX_RANK];
};
extern CCustomRanking gRanking;
// ---
