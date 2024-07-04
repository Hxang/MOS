#include "stdafx.h"
#include "BotTrade.h"
#include "Monster.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "SkillManager.h"
#include "EffectManager.h"
#include "MasterSkillTree.h"
#include "ServerInfo.h"
#include "MemScript.h"
#include "Util.h"
#include "CashShop.h"
#include "Message.h"
#include "readscript2.h"
#include "Notice.h"
#include "ChaosBox.h"
#include "Trade.h"
#include "DSProtocol.h"
#include "Message.h"


ObjBotTrader BotTrader;

int ObjBotTrader::MixNumber(int index)
{
	for (int i = 0; i<this->MixCount; i++)
	{
		if (this->Mix[i].Index == index)
			return i;
	}
	return -1;
}

void ObjBotTrader::Read(char * FilePath)
{
	if (this->Enabled == true)
	{
		for (int botNum = 0; botNum<MAX_BOTTRADER; botNum++)
		{
			if (this->bot[botNum].Enabled == true)
			{
				int bIndex = this->bot[botNum].index;
				if (gObjIsConnected(bIndex) == TRUE)
				{
					gObjDel(bIndex);
				}
			}
		}
	}

	memset(&this->bot, 0, sizeof(this->bot));
	this->Enabled = FALSE;
	this->MixCount = 0;

	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, FilePath);
		return;
	}

	if (lpMemScript->SetBuffer(FilePath) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	try
	{
		while (true)
		{
			if (lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if (section == 0)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int BotNum = lpMemScript->GetNumber();

					if (BotNum < 0 || BotNum > MAX_BOTTRADER - 1)
					{
						ErrorMessageBox("BotTrader error: BotPetIndex:%d Sai Số Lượng Bot!", BotNum);
						return;
					}

					this->bot[BotNum].Class = lpMemScript->GetAsNumber();

					strncpy_s(this->bot[BotNum].Name, lpMemScript->GetAsString(), sizeof(this->bot[BotNum].Name));

					this->bot[BotNum].Map = lpMemScript->GetAsNumber();

					this->bot[BotNum].X = lpMemScript->GetAsNumber();

					this->bot[BotNum].Y = lpMemScript->GetAsNumber();

					this->bot[BotNum].Dir = lpMemScript->GetAsNumber();

					this->bot[BotNum].OnlyVip = lpMemScript->GetAsNumber();

					this->bot[BotNum].MixesCount = 0;

					this->bot[BotNum].Enabled = lpMemScript->GetAsNumber();
				}
				else if (section == 1)
				{
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int BotNum = lpMemScript->GetNumber();
					if (BotNum < 0 || BotNum > MAX_BOTTRADER - 1)
					{
						ErrorMessageBox("BotTrader error: BotPetIndex:%d doesnt exist", BotNum);
						return;
					}


					int Slot = lpMemScript->GetAsNumber();

					if (Slot < 0 || Slot > 8)
					{
						ErrorMessageBox("BotTrader error: Min Slot 0 ; Max Slot 8");
						return;
					}

					int iType = lpMemScript->GetAsNumber();

					int iIndex = lpMemScript->GetAsNumber();

					this->bot[BotNum].body[Slot].num = GET_ITEM(iType, iIndex);

					this->bot[BotNum].body[Slot].level = lpMemScript->GetAsNumber();

					this->bot[BotNum].body[Slot].opt = lpMemScript->GetAsNumber();

					this->bot[BotNum].body[Slot].Enabled = true;
				}
				else if (section == 2)
				{
					this->Enabled = true;
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					this->Mix[this->MixCount].Index = lpMemScript->GetNumber();

					strncpy_s(this->Mix[this->MixCount].Name, lpMemScript->GetAsString(), sizeof(this->Mix[this->MixCount].Name));

					this->Mix[this->MixCount].SuccessRate = lpMemScript->GetAsNumber();

					this->Mix[this->MixCount].PriceType = lpMemScript->GetAsNumber();

					this->Mix[this->MixCount].PriceCount = lpMemScript->GetAsNumber();

					this->MixCount++;
				}
				else if (section == 3)
				{
					this->Enabled = true;
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int mIndex = lpMemScript->GetNumber();
					int Num = this->MixNumber(mIndex);

					if (Num == -1)
					{
						ErrorMessageBox("BotTrader data error on Keep Items (INDEX:%d)", mIndex);
						return;
					}

					int iType = lpMemScript->GetAsNumber();

					int iIndex = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].Type = GET_ITEM(iType, iIndex);

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].LevelMin = 0;

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].LevelMax = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].OptionMin = 0;

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].OptionMax = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].Luck = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].Skill = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].Exc = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Keep[this->Mix[Num].i_KeepItem].DongThan = lpMemScript->GetAsNumber();

					this->Mix[Num].i_KeepItem++;
				}
				else if (section == 4)
				{
					this->Enabled = true;
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int mIndex = lpMemScript->GetNumber();
					int Num = this->MixNumber(mIndex);

					this->MixT[Num].i_NeedCount = lpMemScript->GetAsNumber();

					if (Num == -1)
					{
						ErrorMessageBox("BotTrader data error on Need Items (INDEX: %d)", mIndex);
						return;
					}

					int iType = lpMemScript->GetAsNumber();

					int iIndex = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].Type = GET_ITEM(iType, iIndex);

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].LevelMin = 0;

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].LevelMax = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].OptionMin = 0;

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].OptionMax = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].Luck = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].Skill = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].Exc = lpMemScript->GetAsNumber();

					this->MixT[Num].i_Need[this->MixT[Num].i_NeedCount].DongThan = lpMemScript->GetAsNumber();

					this->MixT[Num].i_NeedCount++;
				}//1
				else if (section == 5)
				{
					this->Enabled = true;
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int mIndex = lpMemScript->GetNumber();
					int Num = this->MixNumber(mIndex);

					if (Num == -1)
					{
						ErrorMessageBox("TradeMix data error on Success Items (INDEX:%d)", mIndex);
						return;
					}

					int iType = lpMemScript->GetAsNumber();

					int iIndex = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Type = GET_ITEM(iType, iIndex);

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].LevelMin = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].LevelMax = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].OptionMin = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].OptionMax = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Luck = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Skill = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].Exc = lpMemScript->GetAsNumber();

					this->Mix[Num].i_Success[this->Mix[Num].i_SuccessCount].DongThan = lpMemScript->GetAsNumber();

					this->Mix[Num].i_SuccessCount++;
				}
				else if (section == 6)
				{
					this->Enabled = true;
					if (strcmp("end", lpMemScript->GetAsString()) == 0)
					{
						break;
					}
					int BotNum = lpMemScript->GetNumber();
					if (BotNum < 0 || BotNum > MAX_BOTTRADER - 1)
					{
						ErrorMessageBox("BotTrader error: BotPetIndex: %d doesnt exist", BotNum);
						return;
					}

					int mIndex = lpMemScript->GetAsNumber();
					int Num = this->MixNumber(mIndex);
					if (Num == -1)
					{
						ErrorMessageBox("TradeMix data error on Success Items (INDEX: %d)", mIndex);
						return;
					}
					this->bot[BotNum].MixesIndex[this->bot[BotNum].MixesCount] = Num;

					this->bot[BotNum].MixesCount++;
					this->Enabled = true;
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}


void ObjBotTrader::MakeBot()
{
	if (this->Enabled == true)
	{
		for (int botNum = 0; botNum<MAX_BOTTRADER; botNum++)
		{
			if (this->bot[botNum].Enabled == true)
			{
				int aIndex = gObjAddSummon();
				if (aIndex >= 0)
				{
					LPOBJ lpObj = &gObj[aIndex];
					this->bot[botNum].index = aIndex;
					lpObj->PosNum = (WORD)-1;
					lpObj->X = this->bot[botNum].X;
					lpObj->Y = this->bot[botNum].Y;
					lpObj->OldX = this->bot[botNum].X;
					lpObj->OldY = this->bot[botNum].Y;
					lpObj->TX = this->bot[botNum].X;
					lpObj->TY = this->bot[botNum].Y;
					lpObj->MTX = this->bot[botNum].X;
					lpObj->MTY = this->bot[botNum].Y;
					lpObj->Dir = this->bot[botNum].Dir;
					lpObj->Map = this->bot[botNum].Map;
					lpObj->Live = TRUE;
					lpObj->PathCount = 0;
					lpObj->IsBot = 5;
					lpObj->Type = OBJECT_USER;
					//gObjSetMonsterTest(aIndex,this->bot[botNum].Class, "BotBuffer");
					gObjSetBots(aIndex, this->bot[botNum].Class);

					lpObj->ChangeUp = this->bot[botNum].Class % 16;	// Set Second Type of Character
					lpObj->Class = this->bot[botNum].Class / 16;
					lpObj->Level = 400;
					lpObj->Life = 1;
					lpObj->MaxLife = 2;
					lpObj->Mana = 1;
					lpObj->MaxMana = 2;
					lpObj->Experience = 0;
					//lpObj->DBClass = this->bot[botNum].Class;

					lpObj->Inventory = new CItem[INVENTORY_EXT4_SIZE];
					lpObj->Inventory1 = new CItem[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap1 = new BYTE[INVENTORY_EXT4_SIZE];
					lpObj->InventoryMap = new BYTE[INVENTORY_EXT4_SIZE];

					for (int i = 0; i<INVENTORY_EXT4_SIZE; i++)
					{
						lpObj->Inventory[i].Clear();
						lpObj->Inventory1[i].Clear();
					}

					memset(&lpObj->InventoryMap[0], (BYTE)-1, INVENTORY_EXT4_SIZE);
					memset(&lpObj->InventoryMap1[0], (BYTE)-1, INVENTORY_EXT4_SIZE);

					strncpy_s(lpObj->Name, this->bot[botNum].Name, sizeof(lpObj->Name));

					for (int i = 0; i<9; i++)
					{
						if (this->bot[botNum].body[i].num >= 0 && this->bot[botNum].body[i].Enabled == true)
						{
							CItem lpItem;

							lpObj->Inventory[i].Clear();

							lpItem.Clear();

							lpItem.m_Index = this->bot[botNum].body[i].num;

							lpItem.m_Option1 = 0;

							lpItem.m_Option2 = 1;

							lpItem.m_Option3 = this->bot[botNum].body[i].opt;

							lpItem.m_Level = this->bot[botNum].body[i].level;

							lpItem.m_Durability = 255.0f;

							lpItem.m_Serial = 0;

							lpItem.m_NewOption = 0;

							lpItem.m_SetOption = 0;

							lpItem.m_ItemOptionEx = 0;

							lpItem.m_IsPeriodicItem = 0;

							lpItem.m_IsExpiredItem = 0;

							memset(lpItem.m_SocketOption, 0xFF, MAX_SOCKET_OPTION);

							lpItem.m_JewelOfHarmonyOption = 0;

							lpItem.m_SocketOptionBonus = 0xFF;

							lpItem.Convert(lpItem.m_Index, lpItem.m_Option1, lpItem.m_Option2, lpItem.m_Option3, lpItem.m_NewOption, lpItem.m_SetOption, lpItem.m_JewelOfHarmonyOption, lpItem.m_ItemOptionEx, lpItem.m_SocketOption, lpItem.m_SocketOptionBonus);

							gItemManager.InventoryAddItem(aIndex, lpItem, i);
						}
					}

					lpObj->Inventory1 = lpObj->Inventory;
					lpObj->InventoryMap1 = lpObj->InventoryMap;
					//gObjMakePreviewCharSet(result);
					gObjectManager.CharacterMakePreviewCharSet(aIndex);

					lpObj->AttackType = 0;
					lpObj->BotSkillAttack = 0;

					lpObj->Attribute = 100;
					lpObj->TargetNumber = (WORD)-1;
					lpObj->ActionState.Emotion = 0;
					lpObj->ActionState.Attack = 0;
					lpObj->ActionState.EmotionCount = 0;
					lpObj->PathCount = 0;
					lpObj->BotPower = 0;
					lpObj->BotDefense = this->bot[botNum].OnlyVip;
					lpObj->BotLife = 1;
					lpObj->BotMaxLife = 1;

					lpObj->BotLvlUpDefense = 1;
					lpObj->BotLvlUpPower = 1;
					lpObj->BotLvlUpLife = 1;
					lpObj->BotLvlUpMana = 1;
					lpObj->BotLvlUpExp = 1;
					lpObj->BotLvlUpMaxLevel = 1;

					lpObj->MoveRange = 1;
					lpObj->BotFollowMe = 0;
					lpObj->NextExperience = gLevelExperience[lpObj->Level];
				}
			}
		}
	}
}

BOOL ObjBotTrader::IsInTrade(int aIndex)
{
	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);
	if (number != -1)
		return 1;
	else
		return 0;
}

int ObjBotTrader::GetBotIndex(int aIndex)
{
	for (int i = 0; i<MAX_BOTTRADER; i++)
	{
		if (this->bot[i].Enabled == true)
		{
			if (this->bot[i].index == aIndex)
				return i;
		}
	}
	return -1;
}

void ObjBotTrader::TradeCancel(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if (lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
	{
		return;
	}

	gTrade.ResetTrade(aIndex);
	gTrade.GCTradeResultSend(aIndex, 0);
}

void ObjBotTrader::TradeOk(int aIndex)
{
	int MixNum = -1;
	int MixItem = -1;

	int number = this->GetBotIndex(gObj[aIndex].TargetNumber);

	if (number == -1)
	{
		gObj[aIndex].Interface.state = 0;
		gObj[aIndex].Interface.type = 0;
		gObj[aIndex].TargetShopNumber = -1;
		gObj[aIndex].Transaction = 0;
		return;
	}
	this->TradeMix(aIndex, number);
}


BOOL ObjBotTrader::TradeOpen(int index, int nindex)
{
	if (gObjIsConnected(index) == 0)
	{
		return 0;
	}
	if (gObjIsConnected(nindex) == 0)
	{
		return 0;
	}
	int number = this->GetBotIndex(nindex);
	if (number == -1)
		return 0;
	char sbuf[512] = { 0 };
	LPOBJ lpObj = &gObj[index];
	LPOBJ lpBot = &gObj[nindex];

	if (lpObj->AccountLevel < this->bot[number].OnlyVip)
	{
		ChatSend(&gObj[this->bot[number].index], "Bạn Phải Mua Vip Mới Sử Dụng Được Dịch Vụ Này !");
		return 0;
	}

	if (this->Enabled == TRUE)
	{
		if (lpObj->Interface.use > 0)
		{
			return 0;
		}
		else
		{
			for (int n = 0; n < TRADE_SIZE; n++)
			{
				lpObj->Trade[n].Clear();
			}
			memset(lpObj->TradeMap, (BYTE)-1, TRADE_SIZE);

			gObjInventoryTransaction(lpObj->Index);


			gTrade.GCTradeResponseSend(lpObj->Index, 1, lpBot->Name, 400, 0);
			gTrade.GCTradeOkButtonSend(lpObj->Index, 1);
			lpObj->Interface.state = 1;
			lpObj->Interface.use = 1;
			lpObj->Interface.type = 1;
			lpObj->TradeMoney = 0;
			lpObj->TargetNumber = lpBot->Index;
			lpObj->Transaction = 1;

			char wbuf[1024] = { 0 };
			wsprintf(wbuf, "[BotTrader] (%s)(%s) OPEN", gObj[index].Account, gObj[index].Name);
			LogAdd(LOG_RED, wbuf);
			ChatSend(&gObj[this->bot[number].index], "Xin Chào Đây Là Dịch Vụ Đổi Vật Phẩm !");
		}
	}
	return 1;
}

int ObjBotTrader::ItemCount(int aIndex)
{
	int Count = 0;
	for (int n = 0; n < TRADE_SIZE; n++)
	{
		if (gObj[aIndex].Trade[n].IsItem() == 1)
		{
			Count++;
		}
	}
	return Count;
}

bool ObjBotTrader::SearchSameIDNumber(int mIndex, int To)
{
	if (To == 0)
	{
		return true;
	}
	for (int in = 0; in < To; in++)
	{
		if (this->MixT[mIndex].i_Need[in].Type == this->MixT[mIndex].i_Need[To].Type)
		{
			return false;
		}
	}
	return true;
}

BYTE ObjBotTrader::BoxExcOptions(int maxOptions)
{
	BYTE NumberOfOptions = 0;
	BYTE RetOption = 0, TempOption = 0, deadlock = 15;
	BYTE ExcOrgArr[6];

	if (maxOptions > 0)
	{
		if (maxOptions > 6)
			NumberOfOptions = 6;
		else
			NumberOfOptions = rand() % maxOptions + 1;

		//Exc Options IDs
		ExcOrgArr[0] = 8;
		ExcOrgArr[1] = 16;
		ExcOrgArr[2] = 2;
		ExcOrgArr[3] = 4;
		ExcOrgArr[4] = 32;
		ExcOrgArr[5] = 1;

		if (NumberOfOptions >= 6)
		{
			RetOption = ExcOrgArr[0] + ExcOrgArr[1] + ExcOrgArr[2] + ExcOrgArr[3] + ExcOrgArr[4] + ExcOrgArr[5];
			return RetOption;
		}

		if (NumberOfOptions == 5)
		{
			TempOption = ExcOrgArr[rand() % 6];

			RetOption = ExcOrgArr[0] + ExcOrgArr[1] + ExcOrgArr[2] + ExcOrgArr[3] + ExcOrgArr[4] + ExcOrgArr[5] - TempOption;
			return RetOption;
		}

		if (NumberOfOptions == 4)
		{
			RetOption = ExcOrgArr[0] + ExcOrgArr[1] + ExcOrgArr[2] + ExcOrgArr[3] + ExcOrgArr[4] + ExcOrgArr[5];
			while (true)
			{
				TempOption = ExcOrgArr[rand() % 6];

				if ((RetOption & TempOption) == TempOption)
				{
					RetOption -= TempOption;
					NumberOfOptions += 1;
				}

				deadlock -= 1;
				if (NumberOfOptions == 6 || deadlock == 0)
					break;
			}
			return RetOption;
		}

		if (NumberOfOptions == 3)
		{
			RetOption = 0;
			while (true)
			{
				TempOption = ExcOrgArr[rand() % 6];

				if ((RetOption & TempOption) != TempOption)
				{
					RetOption += TempOption;
					NumberOfOptions -= 1;
				}

				deadlock -= 1;
				if (NumberOfOptions == 0 || deadlock == 0)
					break;
			}
			return RetOption;
		}

		if (NumberOfOptions == 2)
		{
			RetOption = 0;
			while (true)
			{
				TempOption = ExcOrgArr[rand() % 6];

				if ((RetOption & TempOption) != TempOption)
				{
					RetOption += TempOption;
					NumberOfOptions -= 1;
				}

				deadlock -= 1;
				if (NumberOfOptions == 0 || deadlock == 0)
					break;
			}
			return RetOption;
		}

		if (NumberOfOptions == 1)
		{
			RetOption = ExcOrgArr[rand() % 6];
			return RetOption;
		}
	}
	return RetOption;
}


BYTE ObjBotTrader::TradeMix(int aIndex, int BotNum)
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

#if(Fix_Dupe)
	if (
		lpObj->Inventory == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "MuOnlineVN");
		return 1;
	}
#endif
	int mIndex;

	for (int i = 0; i<this->bot[BotNum].MixesCount; i++)
	{
		mIndex = this->bot[BotNum].MixesIndex[i];
		if (this->MixT[mIndex].i_NeedCount == this->ItemCount(aIndex))
		{
			int found = 0;
			for (int in = 0; in<this->MixT[mIndex].i_NeedCount; in++)
			{
				if (this->SearchSameIDNumber(mIndex, in) == true)
				{
					for (int n = 0; n < TRADE_SIZE; n++)
					{
						if (lpObj->Trade[n].IsItem() == 1)
						{
							if (this->MixT[mIndex].i_Need[in].Type == lpObj->Trade[n].m_Index)
							{
								if (lpObj->Trade[n].m_Level == this->MixT[mIndex].i_Need[in].LevelMax
									&& lpObj->Trade[n].m_Option2 >= this->MixT[mIndex].i_Need[in].Luck
									&& lpObj->Trade[n].m_Option3 >= this->MixT[mIndex].i_Need[in].OptionMax
									&& lpObj->Trade[n].m_Option1 >= this->MixT[mIndex].i_Need[in].Skill
									&& lpObj->Trade[n].m_NewOption >= this->MixT[mIndex].i_Need[in].Exc
									&& lpObj->Trade[n].m_SetOption >= this->MixT[mIndex].i_Need[in].DongThan)
								{
									found++;
								}
							}
						}
					}
				}
			}//2

			if (found == this->MixT[mIndex].i_NeedCount)
			{
				goto OK;
			}
		}
	}
	ChatSend(&gObj[this->bot[BotNum].index], "Vui long dat dung vat pham");
	goto Cancel;
OK:
	char sbuf[512] = { 0 };
	if (mIndex >= 0)
	{
		if (this->Mix[mIndex].PriceType == 0)
		{
			if (lpObj->Money < this->Mix[mIndex].PriceCount)
			{
				wsprintf(sbuf, "Bạn Cần %d Zen!", this->Mix[mIndex].PriceCount);
				ChatSend(&gObj[this->bot[BotNum].index], sbuf);
				goto Cancel;
			}
		}
		else if (this->Mix[mIndex].PriceType == 1)
		{
			if (lpObj->Coin1 < this->Mix[mIndex].PriceCount)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Cần %d WC Để Trao Đổi!", this->Mix[mIndex].PriceCount);
				ChatSend(&gObj[this->bot[BotNum].index], sbuf);
				gCashShop.CGCashShopPointRecv(lpObj->Index);
				goto Cancel;
			}
		}
		else if (this->Mix[mIndex].PriceType == 2)
		{
			if (lpObj->Coin2 < this->Mix[mIndex].PriceCount)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Cần %d WP Để Trao Đổi!", this->Mix[mIndex].PriceCount);
				ChatSend(&gObj[this->bot[BotNum].index], sbuf);
				gCashShop.CGCashShopPointRecv(lpObj->Index);
				goto Cancel;
			}
		}
		else if (this->Mix[mIndex].PriceType == 3)
		{
			if (lpObj->Coin3 < this->Mix[mIndex].PriceCount)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Bạn Cần %d GP Để Trao Đổi!", this->Mix[mIndex].PriceCount);
				ChatSend(&gObj[this->bot[BotNum].index], sbuf);
				gCashShop.CGCashShopPointRecv(lpObj->Index);
				goto Cancel;
			}
		}
		else
		{
			wsprintf(sbuf, "Wrong currency! Contact Admin check config!!!");
			ChatSend(&gObj[this->bot[BotNum].index], sbuf);
			goto Cancel;
		}

		srand(static_cast<int>(time(NULL)));
		int random = rand() % 100;
		if (random <= this->Mix[mIndex].SuccessRate)
		{
			//SUCCESS
			srand(static_cast<int>(time(NULL)));
			int rItem = rand() % this->Mix[mIndex].i_SuccessCount;
			int Level = 0;
			int Opt = 0;
			int Exc = 0;
			int SetOption = 0;
			int Luck = 0;
			int Skill = 0;

			if (this->Mix[mIndex].i_Success[rItem].LevelMax == this->Mix[mIndex].i_Success[rItem].LevelMin)
			{
				Level = this->Mix[mIndex].i_Success[rItem].LevelMax;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				Level = rand() % (this->Mix[mIndex].i_Success[rItem].LevelMax - this->Mix[mIndex].i_Success[rItem].LevelMin) + this->Mix[mIndex].i_Success[rItem].LevelMin;
			}
			if (this->Mix[mIndex].i_Success[rItem].OptionMax == this->Mix[mIndex].i_Success[rItem].OptionMin)
			{
				Opt = this->Mix[mIndex].i_Success[rItem].OptionMax;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				Opt = rand() % (this->Mix[mIndex].i_Success[rItem].OptionMax - this->Mix[mIndex].i_Success[rItem].OptionMin) + this->Mix[mIndex].i_Success[rItem].OptionMin;
			}
			if (this->Mix[mIndex].i_Success[rItem].Exc > 0)
			{
				Exc = this->BoxExcOptions(this->Mix[mIndex].i_Success[rItem].Exc);
			}
			if (this->Mix[mIndex].i_Success[rItem].DongThan > 0)
			{
				SetOption = this->Mix[mIndex].i_Success[rItem].DongThan;
			}
			if (this->Mix[mIndex].i_Success[rItem].Luck == 0)
			{
				Luck = 0;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE lRnd = rand() % 100;
				if (lRnd <= this->Mix[mIndex].i_Success[rItem].Luck)
				{
					Luck = 1;
				}
				else
				{
					Luck = 0;
				}
			}
			if (this->Mix[mIndex].i_Success[rItem].Skill == 0)
			{
				Skill = 0;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE sRnd = rand() % 100;
				if (sRnd <= this->Mix[mIndex].i_Success[rItem].Skill)
				{
					Skill = 1;
				}
				else
				{
					Skill = 0;
				}
			}
			if (this->Mix[mIndex].PriceType == 0)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Giao Dịch Thành Công Tiêu Hao: %d Zen !", this->Mix[mIndex].PriceCount);
			}
			else if (this->Mix[mIndex].PriceType == 1)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Giao Dịch Thành Công Tiêu Hao: %d WC !", this->Mix[mIndex].PriceCount);
			}
			else if (this->Mix[mIndex].PriceType == 2)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Giao Dịch Thành Công Tiêu Hao: %d WP !", this->Mix[mIndex].PriceCount);
			}
			else if (this->Mix[mIndex].PriceType == 3)
			{
				gNotice.GCNoticeSend(lpObj->Index, 1, 0, 0, 0, 0, 0, "Giao Dịch Thành Công Tiêu Hao: %d GP !", this->Mix[mIndex].PriceCount);
			}
			ChatSend(&gObj[this->bot[BotNum].index], "Giao Dịch Thành Công");
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, this->Mix[mIndex].i_Success[rItem].Type, Level, 0, Skill, Luck, Opt, -1, Exc, SetOption, 0, 0, 0, 0xFF, 0);
		}
		else
		{
			int rItem1 = rand() % this->Mix[mIndex].i_KeepItem;
			int Level = 0;
			int Opt = 0;
			int Exc = 0;
			int SetOption = 0;
			int Luck = 0;
			int Skill = 0;

			if (this->Mix[mIndex].i_Keep[rItem1].LevelMax == this->Mix[mIndex].i_Keep[rItem1].LevelMin)
			{
				Level = this->Mix[mIndex].i_Keep[rItem1].LevelMax;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				Level = rand() % (this->Mix[mIndex].i_Keep[rItem1].LevelMax - this->Mix[mIndex].i_Keep[rItem1].LevelMin) + this->Mix[mIndex].i_Keep[rItem1].LevelMin;
			}
			if (this->Mix[mIndex].i_Keep[rItem1].OptionMax == this->Mix[mIndex].i_Keep[rItem1].OptionMin)
			{
				Opt = this->Mix[mIndex].i_Keep[rItem1].OptionMax;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				Opt = rand() % (this->Mix[mIndex].i_Keep[rItem1].OptionMax - this->Mix[mIndex].i_Keep[rItem1].OptionMin) + this->Mix[mIndex].i_Keep[rItem1].OptionMin;
			}
			if (this->Mix[mIndex].i_Keep[rItem1].Exc > 0)
			{
				Exc = this->BoxExcOptions(this->Mix[mIndex].i_Keep[rItem1].Exc);
			}
			if (this->Mix[mIndex].i_Keep[rItem1].DongThan > 0)
			{
				SetOption = this->Mix[mIndex].i_Keep[rItem1].DongThan;
			}
			if (this->Mix[mIndex].i_Keep[rItem1].Luck == 0)
			{
				Luck = 0;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE lRnd = rand() % 100;
				if (lRnd <= this->Mix[mIndex].i_Keep[rItem1].Luck)
				{
					Luck = 1;
				}
				else
				{
					Luck = 0;
				}
			}
			if (this->Mix[mIndex].i_Keep[rItem1].Skill == 0)
			{
				Skill = 0;
			}
			else
			{
				srand(static_cast<int>(time(NULL)));
				BYTE sRnd = rand() % 100;
				if (sRnd <= this->Mix[mIndex].i_Keep[rItem1].Skill)
				{
					Skill = 1;
				}
				else
				{
					Skill = 0;
				}
			}

			ChatSend(&gObj[this->bot[BotNum].index], "Giao Dịch Thất bại");
			GDCreateItemSend(lpObj->Index, 0xEB, 0, 0, this->Mix[mIndex].i_Keep[rItem1].Type, Level, 0, Skill, Luck, Opt, -1, Exc, SetOption, 0, 0, 0, 0xFF, 0);
		}

		if (this->Mix[mIndex].PriceType == 0)
		{
			lpObj->Money -= this->Mix[mIndex].PriceCount;
			GCMoneySend(lpObj->Index, lpObj->Money);
		}
		else if (this->Mix[mIndex].PriceType == 1)
		{
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, this->Mix[mIndex].PriceCount, 0, 0, 0);
			gCashShop.CGCashShopPointRecv(lpObj->Index);
		}
		else if (this->Mix[mIndex].PriceType == 2)
		{
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, 0, this->Mix[mIndex].PriceCount, 0, 0);
			gCashShop.CGCashShopPointRecv(lpObj->Index);
		}
		else if (this->Mix[mIndex].PriceType == 3)
		{
			gCashShop.GDCashShopSubPointSaveSend(lpObj->Index, 0, 0, 0, this->Mix[mIndex].PriceCount, 0);
			gCashShop.CGCashShopPointRecv(lpObj->Index);
		}

		gObjInventoryCommit(aIndex);
		gObjectManager.CharacterMakePreviewCharSet(aIndex);
		gItemManager.GCItemListSend(aIndex);
		//GDCharacterInfoSaveSend(aIndex);
		GDPetItemInfoSend(aIndex, 0);
		lpObj->Interface.use = 0;
		lpObj->Interface.type = INTERFACE_NONE;
		lpObj->Interface.state = 0;
		lpObj->TargetNumber = -1;
		lpObj->TradeOk = 0;
		lpObj->TradeOkTime = 0;
		lpObj->TradeMoney = 0;
		GCMoneySend(aIndex, lpObj->Money);
		gTrade.GCTradeResultSend(aIndex, 1);
		gTrade.ClearTrade(lpObj);

		LogAdd(LOG_RED, "[BotTrader](Account: %s)(Name: %s) %s Completed [%d/%d]", lpObj->Account, lpObj->Name, this->Mix[mIndex].Name, random, this->Mix[mIndex].SuccessRate);
		return 1;
	}

	ChatSend(&gObj[this->bot[BotNum].index], "Mix Fail!");
	LogAdd(LOG_RED, "[BotTrader](Account: %s)(Name: %s) %s Fail [%d]", lpObj->Account, lpObj->Name, this->Mix[mIndex].Name, this->Mix[mIndex].SuccessRate);

Cancel:
	this->TradeCancel(aIndex);
	return 0;
}
