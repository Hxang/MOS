// ItemBagManager.h: interface for the CItemBagManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ItemBag.h"
#include "ItemBagEx.h"
#include "User.h"

enum eItemBagSpecialValue
{
	ITEM_BAG_BLOOD_CASTLE1 = 0,
	ITEM_BAG_BLOOD_CASTLE2 = 1,
	ITEM_BAG_BLOOD_CASTLE3 = 2,
	ITEM_BAG_BLOOD_CASTLE4 = 3,
	ITEM_BAG_BLOOD_CASTLE5 = 4,
	ITEM_BAG_BLOOD_CASTLE6 = 5,
	ITEM_BAG_BLOOD_CASTLE7 = 6,
	ITEM_BAG_BLOOD_CASTLE8 = 7,
	ITEM_BAG_CHAOS_CASTLE1 = 8,
	ITEM_BAG_CHAOS_CASTLE2 = 9,
	ITEM_BAG_CHAOS_CASTLE3 = 10,
	ITEM_BAG_CHAOS_CASTLE4 = 11,
	ITEM_BAG_CHAOS_CASTLE5 = 12,
	ITEM_BAG_CHAOS_CASTLE6 = 13,
	ITEM_BAG_CHAOS_CASTLE7 = 14,
	ITEM_BAG_STARTER_RING1 = 15,
	ITEM_BAG_STARTER_RING2 = 16,
	ITEM_BAG_SENIOR_MIX = 17,
	ITEM_BAG_LEO_THE_HELPER = 18,
	ITEM_BAG_ILLUSION_TEMPLE1 = 19,
	ITEM_BAG_ILLUSION_TEMPLE2 = 20,
	ITEM_BAG_ILLUSION_TEMPLE3 = 21,
	ITEM_BAG_ILLUSION_TEMPLE4 = 22,
	ITEM_BAG_ILLUSION_TEMPLE5 = 23,
	ITEM_BAG_ILLUSION_TEMPLE6 = 24,
	ITEM_BAG_CHERRY_BLOSSOM_MIX1 = 25,
	ITEM_BAG_CHERRY_BLOSSOM_MIX2 = 26,
	ITEM_BAG_CHERRY_BLOSSOM_MIX3 = 27,
	ITEM_BAG_SANTA_CLAUS = 28,
	ITEM_BAG_LUCKY_COIN1 = 29,
	ITEM_BAG_LUCKY_COIN2 = 30,
	ITEM_BAG_LUCKY_COIN3 = 31,
	ITEM_BAG_CHAOS_CARD_MIX1 = 32,
	ITEM_BAG_CHAOS_CARD_MIX2 = 33,
	ITEM_BAG_CHAOS_CARD_MIX3 = 34,
	ITEM_BAG_CHAOS_CARD_MIX4 = 35,
	ITEM_BAG_CHAOS_CARD_MIX5 = 36,
	ITEM_BAG_DOUBLE_GOER_SILVER_CHEST1 = 37,
	ITEM_BAG_DOUBLE_GOER_SILVER_CHEST2 = 38,
	ITEM_BAG_DOUBLE_GOER_SILVER_CHEST3 = 39,
	ITEM_BAG_DOUBLE_GOER_SILVER_CHEST4 = 40,
	ITEM_BAG_DOUBLE_GOER_SILVER_CHEST5 = 41,
	ITEM_BAG_DOUBLE_GOER_GOLDEN_CHEST1 = 42,
	ITEM_BAG_DOUBLE_GOER_GOLDEN_CHEST2 = 43,
	ITEM_BAG_DOUBLE_GOER_GOLDEN_CHEST3 = 44,
	ITEM_BAG_DOUBLE_GOER_GOLDEN_CHEST4 = 45,
	ITEM_BAG_DOUBLE_GOER_GOLDEN_CHEST5 = 46,
	ITEM_BAG_IMPERIAL_GUARDIAN_SIDE_BOSS1 = 47,
	ITEM_BAG_IMPERIAL_GUARDIAN_SIDE_BOSS2 = 48,
	ITEM_BAG_IMPERIAL_GUARDIAN_SIDE_BOSS3 = 49,
	ITEM_BAG_IMPERIAL_GUARDIAN_SIDE_BOSS4 = 50,
	ITEM_BAG_IMPERIAL_GUARDIAN_SIDE_BOSS5 = 51,
	ITEM_BAG_IMPERIAL_GUARDIAN_MAIN_BOSS1 = 52,
	ITEM_BAG_IMPERIAL_GUARDIAN_MAIN_BOSS2 = 53,
	ITEM_BAG_IMPERIAL_GUARDIAN_MAIN_BOSS3 = 54,
	ITEM_BAG_IMPERIAL_GUARDIAN_MAIN_BOSS4 = 55,
	ITEM_BAG_IMPERIAL_GUARDIAN_MAIN_BOSS5 = 56,
	ITEM_BAG_CRYWOLF = 57,
	ITEM_BAG_MU_RUMMY1 = 58,
	ITEM_BAG_MU_RUMMY2 = 59,
	ITEM_BAG_LOTTERY = 60,
	ITEM_BAG_GIFT = 61,
	ITEM_BAG_BLOOD_VIP = 62,
	ITEM_BAG_DEVIL_VIP = 63,
	ITEM_BAG_FIND_ADMIN = 64, 
	ITEM_BAG_GREATPK_KILL = 65,
	ITEM_BAG_GREATPK_DONE = 66,
};

struct ITEM_BAG_MANAGER_INFO
{
	int Index;
	int ItemIndex;
	int ItemLevel;
	int MonsterClass;
	int SpecialValue;
	int DropType;
	CItemBag ItemBag;
};

class CItemBagManager
{
public:
	CItemBagManager();
	virtual ~CItemBagManager();
	void Load(char* path);
	void LoadEventItemBag();
	bool GetItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,CItem* lpItem);
	bool GetItemByMonsterClass(int MonsterClass,LPOBJ lpObj,CItem* lpItem);
	bool GetItemBySpecialValue(int SpecialValue,LPOBJ lpObj,CItem* lpItem);
	int DropItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,int map,int x,int y);
	bool DropItemByMonsterClass(int MonsterClass,LPOBJ lpObj,int map,int x,int y);
	bool DropItemBySpecialValue(int SpecialValue,LPOBJ lpObj,int map,int x,int y);
	bool DropItemByIndexItemBag(int IndexItemBag, LPOBJ lpObj);

private:
	std::map<int,ITEM_BAG_MANAGER_INFO> m_ItemBagManagerInfo;
};

extern CItemBagManager gItemBagManager;
