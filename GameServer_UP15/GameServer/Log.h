// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_LOG 20

enum eLogType
{
	LOG_GENERAL = 0,
	LOG_CHAT = 1,
	LOG_COMMAND = 2,
	LOG_TRADE = 3,
	LOG_CONNECT = 4,
	LOG_HACK = 5,
	LOG_CASH_SHOP = 6,
	LOG_CHAOS_MIX = 7,
	LOG_ANTIFLOOD = 8,
	LOG_RESET = 9,
	LOG_TIEN_TE = 10,
	LOG_KET_NOI = 11,
	LOG_THU_MUA = 12,

	LOG_DOI_ITEM = 13,
	LOG_DUNGLUYEN = 14,
	LOG_NGAN_HANG_NGOC = 15,
	LOG_CONG_HUONG = 16,
	LOG_DANH_HIEU = 17,
	LOG_TU_LUYEN = 18,
	LOG_QUAN_HAM = 19,


};

struct LOG_INFO
{
	BOOL Active;
	char Directory[256];
	int Day;
	int Month;
	int Year;
	char Filename[256];
	HANDLE File;
};

class CLog
{
public:
	CLog();
	virtual ~CLog();
	void AddLog(BOOL active,char* directory);
	void Output(eLogType type,char* text,...);
private:
	LOG_INFO m_LogInfo[MAX_LOG];
	int m_count;
};

extern CLog gLog;
