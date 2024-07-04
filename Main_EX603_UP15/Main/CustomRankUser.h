#ifndef _CUSTOM_RANK_USER_H
#define _CUSTOM_RANK_USER_H
// ---
#include "Protocol.h"
#include "Object.h"
// ---
#define pObjPreviewThis        ((DWORD)0x096A4C0)
#define CS_GET_STRUCT(x, y)	   (1124 * x + *(int*) y)
#define Color4f(r, b, g, o)	   ((o << 24) + (g << 16) + (b << 8) + r)
// ---

// ---
struct PMSG_CUSTOM_RANKUSER
{
	PSBMSG_HEAD h;
	char szName[20];
	int iIndex;
	int iLevel;
	int iReset;
	int iMReset;
	int iKills;
	int iDeads;
	int iFor;
	int iEne;
	int iAgi;
	int iVit;
	int iCom;
	int iClasse;
	int iRank;

	// ---
	int irDanhHieu;
	int irQuanHam;
	int irTuLuyen;
	int irVipTaiKhoan;
#if(B_HON_HOAN)
	WORD CapDoHonHoan;
#endif
#if(FLAG_SKIN)
	int Flag;
#endif
	// ---
	char iVip[20];
};
// ---
struct CUSTOM_RANKUSER_DATA
{
	WORD m_Index;
	WORD m_Resets;
	WORD m_MResets;
	WORD m_Level;
	WORD m_Kills;
	WORD m_Deads;
	WORD m_For;
	WORD m_Vit;
	WORD m_Ene;
	WORD m_Agi;
	WORD m_Com;
	WORD m_Classe;
	WORD m_Rank;

	int m_rDanhHieu;
	int m_rQuanHam;
	int m_rTuLuyen;
	int m_rVipTaiKhoan;
#if(B_HON_HOAN)
	WORD m_CapDoHonHoan;
#endif
#if(FLAG_SKIN)
	int m_Flag;
#endif
	char m_Vip[20];
	BOOL m_Enabled;
	DWORD	ModelID;
	float	Width;
	float	Height;
	float	X;
	float	Y;
	float	MaxX;
	float	MaxY;
	DWORD	EventTick;
	bool	OnClick;
	bool	OnShow;
	BYTE	Attribute;
	char szTitleRank[20];
};
// ---
class CCustomRankUser
{
public:
	CCustomRankUser();
	// ---
	void DrawInfo();
	void RenderShowLogRank(DWORD ViewPortAddr, int index, int PosX, int PosY);
	void LoadInfoUser(int index, int PosX, int PosY, char* name);
	int  DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...);
	// ---
	void GCReqRankLevelUser(PMSG_CUSTOM_RANKUSER * aRecv);
	CUSTOM_RANKUSER_DATA m_Data[14000];

private:
	// ---
	//CUSTOM_RANKUSER_DATA m_Data[14000];
};
extern CCustomRankUser gCustomRankUser;
// ---
class CPreviewManager
{
public:
	bool ObjCheck(int index, int pThis);
	bool ObjSelect(int index, int pThis);
	// ---
	WORD  Index;
	BYTE  Live;
	BYTE  Type;
	BYTE  IsSafeZone;
	// ---
	VAngle Angle;
};
// ---
#endif