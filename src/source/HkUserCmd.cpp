#include "hook.h"
#pragma warning(disable:4996)

#define PRINT_ERROR() { for(uint i = 0; (i < sizeof(wscError)/sizeof(wstring)); i++) PrintUserCmdText(iClientID, wscError[i]); return; }
#define PRINT_ERROR_NORETURN() { for(uint i = 0; (i < sizeof(wscError)/sizeof(wstring)); i++) PrintUserCmdText(iClientID, wscError[i]); }
#define PRINT_OK() PrintUserCmdText(iClientID, L"OK");
#define PRINT_DISABLED() PrintUserCmdText(iClientID, L"Command disabled");

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef void (*_UserCmdProc)(uint, wstring);

struct USERCMD
{
	wchar_t *wszCmd;
	_UserCmdProc proc;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintUserCmdText(uint iClientID, wstring wscText, ...)
{
	wchar_t wszBuf[1024*8] = L"";
	va_list marker;
	va_start(marker, wscText);
	_vsnwprintf(wszBuf, sizeof(wszBuf)-1, wscText.c_str(), marker);

	wstring wscXML = L"<TRA data=\"" + set_wscUserCmdStyle + L"\" mask=\"-1\"/><TEXT>" + XMLText(wszBuf) + L"</TEXT>";
	HkFMsg(iClientID, wscXML);
}

void PrintUserCmdText(wstring wscCharName, wstring wscText, ...)
{
	wchar_t wszBuf[1024*8] = L"";
	va_list marker;
	va_start(marker, wscText);
	_vsnwprintf(wszBuf, sizeof(wszBuf)-1, wscText.c_str(), marker);

	wstring wscXML = L"<TRA data=\"" + set_wscUserCmdStyle + L"\" mask=\"-1\"/><TEXT>" + XMLText(wszBuf) + L"</TEXT>";
	HkFMsg(wscCharName, wscXML);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintUniverseText(wstring wscText, ...)
{
	wchar_t wszBuf[1024*8] = L"";
	va_list marker;
	va_start(marker, wscText);
	_vsnwprintf(wszBuf, sizeof(wszBuf)-1, wscText.c_str(), marker);

	wstring wscXML = L"<TRA data=\"" + set_wscAdminCmdStyle + L"\" mask=\"-1\"/><TEXT>" + XMLText(wszBuf) + L"</TEXT>";
	HkFMsgU( wscXML);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_SetDieMsg(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdSetDieMsg)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /set diemsg <param>",
		L"<param>: all,system,self or none",
	};

	wstring wscDieMsg = ToLower(GetParam(wscParam, ' ', 0));;

	DIEMSGTYPE dieMsg;
	if(!wscDieMsg.compare(L"all"))
		dieMsg = DIEMSG_ALL;
	else if(!wscDieMsg.compare(L"system"))
		dieMsg = DIEMSG_SYSTEM;
	else if(!wscDieMsg.compare(L"none"))
		dieMsg = DIEMSG_NONE;
	else if(!wscDieMsg.compare(L"self"))
		dieMsg = DIEMSG_SELF;
	else 
		PRINT_ERROR();

	// save to ini
	GET_USERFILE(scUserFile);
	IniWrite(scUserFile, "settings", "DieMsg", itos(dieMsg));

	// save in ClientInfo
	ClientInfo[iClientID].dieMsg = dieMsg; 

	// send confirmation msg
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_SetDieMsgSize(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdSetDieMsgSize)
	{
		PRINT_DISABLED();
		return;
	}
	
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /set diemsgsize <size>",
		L"<size>: small, default",
	};

	wstring wscDieMsgSize = ToLower(GetParam(wscParam, ' ', 0));
//	wstring wscDieMsgStyle = ToLower(GetParam(wscParam, ' ', 1));

	CHATSIZE dieMsgSize;
	if(!wscDieMsgSize.compare(L"small"))
		dieMsgSize = CS_SMALL;
	else if(!wscDieMsgSize.compare(L"default"))
		dieMsgSize = CS_DEFAULT;
	else 
		PRINT_ERROR();

/*	CHATSTYLE dieMsgStyle;
	if(!wscDieMsgStyle.compare(L"default"))
		dieMsgStyle = CST_DEFAULT;
	else if(!wscDieMsgStyle.compare(L"bold"))
		dieMsgStyle = CST_BOLD;
	else if(!wscDieMsgStyle.compare(L"italic"))
		dieMsgStyle = CST_ITALIC;
	else if(!wscDieMsgStyle.compare(L"underline"))
		dieMsgStyle = CST_UNDERLINE;
	else 
		PRINT_ERROR(); */

	// save to ini
	GET_USERFILE(scUserFile);
	IniWrite(scUserFile, "Settings", "DieMsgSize", itos(dieMsgSize));
//	IniWrite(scUserFile, "Settings", "DieMsgStyle", itos(dieMsgStyle));

	// save in ClientInfo
	ClientInfo[iClientID].dieMsgSize = dieMsgSize; 
//	ClientInfo[iClientID].dieMsgStyle = dieMsgStyle;

	// send confirmation msg
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_SetChatFont(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdSetChatFont)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /set chatfont <size> <style>",
		L"<size>: small, default or big",
		L"<style>: default, bold, italic or underline",
	};

	wstring wscChatSize = ToLower(GetParam(wscParam, ' ', 0));
	wstring wscChatStyle = ToLower(GetParam(wscParam, ' ', 1));
	
	CHATSIZE chatSize;
	if(!wscChatSize.compare(L"small"))
		chatSize = CS_SMALL;
	else if(!wscChatSize.compare(L"default"))
		chatSize = CS_DEFAULT;
	else if(!wscChatSize.compare(L"big"))
		chatSize = CS_BIG;
	else 
		PRINT_ERROR();

	CHATSTYLE chatStyle;
	if(!wscChatStyle.compare(L"default"))
		chatStyle = CST_DEFAULT;
	else if(!wscChatStyle.compare(L"bold"))
		chatStyle = CST_BOLD;
	else if(!wscChatStyle.compare(L"italic"))
		chatStyle = CST_ITALIC;
	else if(!wscChatStyle.compare(L"underline"))
		chatStyle = CST_UNDERLINE;
	else 
		PRINT_ERROR();

	// save to ini
	GET_USERFILE(scUserFile);
	IniWrite(scUserFile, "settings", "ChatSize", itos(chatSize));
	IniWrite(scUserFile, "settings", "ChatStyle", itos(chatStyle));

	// save in ClientInfo
	ClientInfo[iClientID].chatSize = chatSize; 
	ClientInfo[iClientID].chatStyle = chatStyle; 

	// send confirmation msg
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Ignore(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdIgnore)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /ignore <charname> [<flags>]",
		L"<charname>: character name which should be ignored(case insensitive)",
		L"<flags>: combination of the following flags:",
		L" p - only affect private chat",
		L" i - <charname> may match partially",
		L"Examples:",
		L"\"/ignore SomeDude\" ignores all chatmessages from SomeDude",
		L"\"/ignore PlayerX p\" ignores all private-chatmessages from PlayerX",
		L"\"/ignore idiot i\" ignores all chatmessages from players whose charname contain \"idiot\" (e.g. \"[XYZ]IDIOT\", \"MrIdiot\", etc)",
		L"\"/ignore Fool pi\" ignores all private-chatmessages from players whose charname contain \"fool\"",
	};

	wstring wscAllowedFlags = L"pi";

	wstring wscCharname = GetParam(wscParam, ' ', 0);
	wstring wscFlags = ToLower(GetParam(wscParam, ' ', 1));

	if(!wscCharname.length())
		PRINT_ERROR();

	// check if flags are valid
	for(uint i = 0; (i < wscFlags.length()); i++)
	{
		if(wscAllowedFlags.find_first_of(wscFlags[i]) == -1)
			PRINT_ERROR();
	}

	if(ClientInfo[iClientID].lstIgnore.size() > set_iUserCmdMaxIgnoreList)
	{
		PrintUserCmdText(iClientID, L"Error: Too many entries in the ignore list, please delete an entry first!");
		return;
	}

	// save to ini
	GET_USERFILE(scUserFile);
	IniWriteW(scUserFile, "IgnoreList", itos((int)ClientInfo[iClientID].lstIgnore.size() + 1), (wscCharname + L" " + wscFlags));

	// save in ClientInfo
	IGNORE_INFO ii;
	ii.wscCharname = wscCharname;
	ii.wscFlags = wscFlags;
	ClientInfo[iClientID].lstIgnore.push_back(ii);

	// send confirmation msg
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_IgnoreID(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdIgnore)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /ignoreid <client-id> [<flags>]",
		L"<client-id>: client-id of character which should be ignored",
		L"<flags>: if \"p\"(without quotation marks) then only affect private chat",
	};

	wstring wscClientID = GetParam(wscParam, ' ', 0);
	wstring wscFlags = ToLower(GetParam(wscParam, ' ', 1));

	if(!wscClientID.length())
		PRINT_ERROR();

	if(wscFlags.length() && wscFlags.compare(L"p") != 0)
		PRINT_ERROR();

	if(ClientInfo[iClientID].lstIgnore.size() > set_iUserCmdMaxIgnoreList)
	{
		PrintUserCmdText(iClientID, L"Error: Too many entries in the ignore list, please delete an entry first!");
		return;
	}

	uint iClientIDTarget = ToInt(wscClientID);
	if(!HkIsValidClientID(iClientIDTarget) || HkIsInCharSelectMenu(iClientIDTarget))
	{
		PrintUserCmdText(iClientID, L"Error: Invalid client-id");
		return;
	}

	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientIDTarget);

	// save to ini
	GET_USERFILE(scUserFile);
	IniWriteW(scUserFile, "IgnoreList", itos((int)ClientInfo[iClientID].lstIgnore.size() + 1), (wscCharname + L" " + wscFlags));

	// save in ClientInfo
	IGNORE_INFO ii;
	ii.wscCharname = wscCharname;
	ii.wscFlags = wscFlags;
	ClientInfo[iClientID].lstIgnore.push_back(ii);

	// send confirmation msg
	PrintUserCmdText(iClientID, L"OK, \"%s\" added to ignore list", wscCharname.c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_IgnoreList(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdIgnore)
	{
		PRINT_DISABLED();
		return;
	}

	PrintUserCmdText(iClientID, L"ID | Charactername | Flags");
	int i = 1;
	foreach(ClientInfo[iClientID].lstIgnore, IGNORE_INFO, it)
	{
		PrintUserCmdText(iClientID, L"%.2u | %s | %s", i, it->wscCharname.c_str(), it->wscFlags.c_str());
		i++;
	}

	// send confirmation msg
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_DelIgnore(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdIgnore)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /delignore <id> [<id2> <id3> ...]",
		L"<id>: id of ignore-entry(see /ignorelist) or *(delete all)",
	};

	wstring wscID = GetParam(wscParam, ' ', 0);

	if(!wscID.length())
		PRINT_ERROR();

	GET_USERFILE(scUserFile);

	if(!wscID.compare(L"*"))
	{ // delete all
		IniDelSection(scUserFile, "IgnoreList");
		ClientInfo[iClientID].lstIgnore.clear();
		PRINT_OK();
		return;
	}

	list<uint> lstDelete;
	for(uint j = 1; wscID.length(); j++)
	{
		uint iID = _wtoi(wscID.c_str());
		if(!iID || (iID > ClientInfo[iClientID].lstIgnore.size()))
		{
			PrintUserCmdText(iClientID, L"Error: Invalid ID");
			return;
		}

		lstDelete.push_back(iID);
		wscID = GetParam(wscParam, ' ', j);
	}

	lstDelete.sort(greater<uint>());

	ClientInfo[iClientID].lstIgnore.reverse();
	foreach(lstDelete, uint, it)
	{
		uint iCurID = (uint)ClientInfo[iClientID].lstIgnore.size();
		foreach(ClientInfo[iClientID].lstIgnore, IGNORE_INFO, it2)
		{
			if(iCurID == (*it))
			{
				ClientInfo[iClientID].lstIgnore.erase(it2);
				break;
			}
			iCurID--;
		}
	}
	ClientInfo[iClientID].lstIgnore.reverse();

	// send confirmation msg
	IniDelSection(scUserFile, "IgnoreList");
	int i = 1;
	foreach(ClientInfo[iClientID].lstIgnore, IGNORE_INFO, it3)
	{
		IniWriteW(scUserFile, "IgnoreList", itos(i), ((*it3).wscCharname + L" " + (*it3).wscFlags));
		i++;
	}
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_AutoBuy(uint iClientID, wstring wscParam)
{
	if(!set_bUserCmdAutoBuy)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /autobuy <on|off>",
	};

	wscParam = ToLower(Trim(wscParam));

	if(wscParam == L"off")
	{
		GET_USERFILE(scUserFile);
		wstring wscFilename;
		HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename);
		string scSection = "autobuy_" + wstos(wscFilename);
		ClientInfo[iClientID].lstAutoBuyItems.clear();
		IniDelSection(scUserFile, scSection);
		PRINT_OK();
	}
	else if(wscParam == L"on")
	{
		GET_USERFILE(scUserFile);
		wstring wscFilename;
		HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename);
		string scSection = "autobuy_" + wstos(wscFilename);

		ClientInfo[iClientID].lstAutoBuyItems.clear();
		IniDelSection(scUserFile, scSection);
		list<CARGO_INFO> lstCargo;
		HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
		list<AUTOBUY_CARTITEM> lstItems;
		foreach(lstCargo, CARGO_INFO, cargo)
		{
			if(!cargo->bMounted)
			{
				//check for item in list so far
				bool bFoundItem = false;
				foreach(lstItems, AUTOBUY_CARTITEM, item)
				{
					if(item->iArchID == cargo->iArchID)
					{
						item->iCount++;
						IniWrite(scUserFile, scSection, utos(item->iArchID), utos(item->iCount));
						bFoundItem = true;
						break;
					}
				}
				if(!bFoundItem) //Not in existing list
				{
					IniWrite(scUserFile, scSection, utos(cargo->iArchID), utos(cargo->iCount));
					AUTOBUY_CARTITEM item;
					item.iArchID = cargo->iArchID;
					item.iCount = cargo->iCount;
					lstItems.push_back(item);
				}
			}
		}
		if(lstItems.size())
		{
			ClientInfo[iClientID].lstAutoBuyItems = lstItems;
			PRINT_OK();
		}
		else
			PrintUserCmdText(iClientID, L"Error: no unmounted items found");
	}
	else
		PRINT_ERROR();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_IDs(uint iClientID, wstring wscParam)
{
	wchar_t wszLine[128] = L"";
	list<HKPLAYERINFO> lstPlayers = HkGetPlayers();
	foreach(lstPlayers, HKPLAYERINFO, i)
	{
		wchar_t wszBuf[1024];
		swprintf(wszBuf, L"%s = %u | ", (*i).wscCharname.c_str(), (*i).iClientID);
		if((wcslen(wszBuf) + wcslen(wszLine)) >= sizeof(wszLine)/2)	{
			PrintUserCmdText(iClientID, L"%s", wszLine);
			wcscpy(wszLine, wszBuf);
		} else
			wcscat(wszLine, wszBuf);
	}

	if(wcslen(wszLine))
		PrintUserCmdText(iClientID, L"%s", wszLine);
	PrintUserCmdText(iClientID, L"OK");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_ID(uint iClientID, wstring wscParam)
{
	PrintUserCmdText(iClientID, L"Your client-id: %u", iClientID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Invite(uint iClientID, wstring wscParam)
{
	HK_ERROR inviteError = HkInviteToGroup(wscParam, ARG_CLIENTID(iClientID));
	if(!HKHKSUCCESS(inviteError))
	{
		PrintUserCmdText(iClientID, L"Error: " + HkErrGetText(inviteError));
	}
}

void UserCmd_InviteID(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /i$ <client-id>",
	};

	wstring wscClientID = GetParam(wscParam, ' ', 0);

	if(!wscClientID.length())
		PRINT_ERROR();

	uint iClientIDTarget = ToInt(wscClientID);
	if(!HkIsValidClientID(iClientIDTarget) || HkIsInCharSelectMenu(iClientIDTarget))
	{
		PrintUserCmdText(iClientID, L"Error: Invalid client-id");
		return;
	}
	
	HK_ERROR inviteError = HkInviteToGroup(ARG_CLIENTID(iClientIDTarget), ARG_CLIENTID(iClientID));
	if(!HKHKSUCCESS(inviteError))
	{
		PrintUserCmdText(iClientID, L"Error: " + HkErrGetText(inviteError));
	}
}

void UserCmd_InviteAll(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: No players found",
		L"Usage: /ia [name part]",
	};
	
	//wstring wscCharname = Players.GetActiveCharacterName(iClientID);
	uint iGroupID, iNumInvites = 0;
	HkGetGroupID(ARG_CLIENTID(iClientID), iGroupID);

	if(wscParam.length()) //part of a name has been passed
	{
		struct PlayerData *pPD = 0;
		while(pPD = Players.traverse_active(pPD))
		{
			uint iClientIDinvite = HkGetClientIdFromPD(pPD);

			const wchar_t *wszCharname = (wchar_t*)Players.GetActiveCharacterName(iClientIDinvite);
			if(!wszCharname)
				continue;

			wstring wscCharnameInvite = wszCharname;
			if(ToLower(wscCharnameInvite).find(ToLower(wscParam)) != -1)
			{
				uint iGroupIDinvite;
				HkGetGroupID(wscCharnameInvite, iGroupIDinvite);
				if((!iGroupIDinvite || iGroupIDinvite!=iGroupID) && iClientID!=iClientIDinvite)
				{
					HK_ERROR inviteError = HkInviteToGroup(wscCharnameInvite, ARG_CLIENTID(iClientID));
					if(!HKHKSUCCESS(inviteError))
					{
						PrintUserCmdText(iClientID, HkErrGetText(inviteError) + L" on inviting " + wscCharnameInvite);
					}
					else
					{
						iNumInvites++;
					}
				}
			}
		}
	}
	else //invite all players on server
	{
		struct PlayerData *pPD = 0;
		while(pPD = Players.traverse_active(pPD))
		{
			uint iClientIDinvite = HkGetClientIdFromPD(pPD);

			const wchar_t *wszCharname = (wchar_t*)Players.GetActiveCharacterName(iClientIDinvite);
			if(!wszCharname)
				continue;

			wstring wscCharnameInvite = wszCharname;
			uint iGroupIDinvite;
			HkGetGroupID(wscCharnameInvite, iGroupIDinvite);
			if((!iGroupIDinvite || iGroupIDinvite!=iGroupID) && iClientID!=iClientIDinvite)
			{
				HK_ERROR inviteError = HkInviteToGroup(wscCharnameInvite, ARG_CLIENTID(iClientID));
				if(!HKHKSUCCESS(inviteError))
				{
					PrintUserCmdText(iClientID, HkErrGetText(inviteError) + L" on inviting " + wscCharnameInvite);
				}
				else
				{
					iNumInvites++;
				}
			}
		}
	}
	
	if(!iNumInvites)
		PRINT_ERROR();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Cloak(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"Error: You are docked.");
		return;
	}
		
	if (ClientInfo[iClientID].bCanCloak)
	{
		mstime tmTimeNow = timeInMS();
		if (!ClientInfo[iClientID].bCloaked && !ClientInfo[iClientID].bIsCloaking)
		{
			if(ClientInfo[iClientID].bWantsCloak)
				PrintUserCmdText(iClientID, L"Time remaining until cloak: %i seconds", (int)((ClientInfo[iClientID].iCloakWarmup - (tmTimeNow - ClientInfo[iClientID].tmCloakTime))/1000.0f+0.5f));
			else if((tmTimeNow - ClientInfo[iClientID].tmCloakTime) > ClientInfo[iClientID].iCloakCooldown)
			{
				ClientInfo[iClientID].tmCloakTime = tmTimeNow;
				ClientInfo[iClientID].bWantsCloak = true;
				PrintUserCmdText(iClientID, L"Preparing to cloak...");
				if(set_bDropShieldsCloak)
					pub::SpaceObj::DrainShields(Players[iClientID].iSpaceObjID);
			} 
			else
				PrintUserCmdText(iClientID, L"Your cloak has not cooled down. Time remaining: %i seconds", (int)((ClientInfo[iClientID].iCloakCooldown - (tmTimeNow - ClientInfo[iClientID].tmCloakTime))/1000.0f+0.5f));
		} 
		else
			PrintUserCmdText(iClientID, L"You are already cloaked.  Time remaining: %i seconds", ClientInfo[iClientID].iCloakingTime ? (int)((ClientInfo[iClientID].iCloakingTime - (tmTimeNow - ClientInfo[iClientID].tmCloakTime))/1000.0f+0.5f) : 0);
	} 
	else
		PrintUserCmdText(iClientID, L"Error: Your ship does not have a Cloaking Device.");
}

void UserCmd_UnCloak(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip) {
		PrintUserCmdText(iClientID, L"Error: You are docked.");
		return;
	}

	if (ClientInfo[iClientID].bCanCloak) {
		if (!HKHKSUCCESS(HkUnCloak(iClientID)))
			PrintUserCmdText(iClientID, L"Error!");
	} else
		PrintUserCmdText(iClientID, L"Error: Your ship does not have a Cloaking Device.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Rename(uint iClientID, wstring wscParam)
{
	wchar_t wszChargeAmount[72];
	swprintf(wszChargeAmount, L"Renames are subject to a %i credit charge", set_iRenameCmdCharge);
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /rename <new charname>",
		wszChargeAmount,
	};

	if(set_iRenameCmdCharge == -1)//Command has been disabled
	{
		PRINT_DISABLED();
		return;
	}
	
	if(!wscParam.length())//User hasn't entered any params
	{
		PRINT_ERROR();
		return;
	}
	
	wstring wscCharName = ((wchar_t*)Players.GetActiveCharacterName(iClientID));
	int cashAmount=0;
	HkGetCash(ARG_CLIENTID(iClientID), cashAmount);
	if(cashAmount>=set_iRenameCmdCharge)
	{
		if(wscParam.find(' ',0)!=-1)
		{
			PrintUserCmdText(iClientID, L"Error: Character names must not have spaces in them.");
			return;
		}
		HK_ERROR renameError = HkRename(ARG_CLIENTID(iClientID), wscParam, false);
		if(!HKHKSUCCESS(renameError))
		{
			if(renameError==HKE_CHARNAME_ALREADY_EXISTS)
			{
				PrintUserCmdText(iClientID, L"Error: Specified character already exists. Choose a different name.");
			}
			else if(renameError==HKE_CHARNAME_TOO_LONG)
			{
				PrintUserCmdText(iClientID, L"Error: Specified character name is too lomg. Choose a different name.");
			}
			else if(renameError==HKE_CHARNAME_TOO_SHORT)
			{
				PrintUserCmdText(iClientID, L"Error: Specified character name is too short. Choose a different name.");
			}
			else
			{
				PRINT_ERROR();
			}
		}
		else
		{
			HkAddCash(wscParam, -set_iRenameCmdCharge);
			PrintUniverseText(wscCharName + L" is now known as " + wscParam);
		}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Error: Not enough cash, 500,000 credits required for rename.");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_SendCash(uint iClientID, wstring wscParam)
{
	if(set_fSendCashTax == -1.0f)//command has been disabled
	{
		PRINT_DISABLED();
		return;
	}
	
	wchar_t wszTaxAmount[72];
	swprintf(wszTaxAmount, L"Transfers are subject to a %f percent tax paid by the sender", set_fSendCashTax);
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /sendcash <charname> <amount>",
		wszTaxAmount,
	};

	uint iTimePlayed;
	if(!HKHKSUCCESS(HkGetTimePlayed(ARG_CLIENTID(iClientID), iTimePlayed)))
	{
		PrintUserCmdText(iClientID, L"Error: could not get time played");
		return;
	}
	if(set_iSendCashTime>0 && iTimePlayed<(uint)set_iSendCashTime*60)
	{
		PrintUserCmdText(iClientID, L"Error: you must play for at least %i minutes before being able to use /sendcash", set_iSendCashTime);
		return;
	}

	wstring targetChar = GetParam(wscParam, ' ', 0);
	wstring amountS = GetParam(wscParam, ' ', 1);
	if(!targetChar.length() || !amountS.length())
	{
		PRINT_ERROR();
	}
	else
	{
		wstring wscCharName = ((wchar_t*)Players.GetActiveCharacterName(iClientID));
		int amount = ToInt(amountS);
		if(amount<=0)
		{
			PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts of money.");
			return;
		}
		int availCash = 0;
		HK_ERROR cashError = HkGetCash(ARG_CLIENTID(iClientID), availCash);
		if(!HKHKSUCCESS(cashError))
		{
			PrintUserCmdText(iClientID, L"Error: Cannot check amount of cash available.");
		}
		else
		{
			if(amount*(1.0f+(set_fSendCashTax/100.0f)) > availCash)
			{
				PrintUserCmdText(iClientID, L"Error: You do not have the necessary funds available for the transfer.");
			}
			else
			{
				cashError = HkAddCash(targetChar, amount);
				if(!HKHKSUCCESS(cashError))
				{
					if(cashError==HKE_NO_CHAR_SELECTED)
					{
						PrintUserCmdText(iClientID, L"Error: The target account is at the character selection screen. Try again when they have completely logged on.");
					}
					else if(cashError==HKE_CHAR_DOES_NOT_EXIST)
					{
						PrintUserCmdText(iClientID, L"Error: The specified character does not exist.");
					}
					else if(cashError==HKE_COULD_NOT_DECODE_CHARFILE)
					{
						PrintUserCmdText(iClientID, L"Error: The specified character's file could not be decoded.");
					}
					else
					{
						PrintUserCmdText(iClientID, L"Error: unspecified.");
					}
				}
				else
				{
					cashError = HkAddCash(ARG_CLIENTID(iClientID), (int)(-(amount*(1.0f+(set_fSendCashTax/100.0f)))));
					if(!HKHKSUCCESS(cashError))
					{
						PrintUserCmdText(iClientID, L"Error: The money could not be subtracted from your balance. Oh noes!"); //This really shouldn't happen, but if it does, retract the money sent to the other char.
						cashError = HkAddCash(targetChar, -amount); 
						PrintUserCmdText(targetChar, wscCharName + L" tried to send you money but the transfer failed.");
						PrintUserCmdText(iClientID, L"Transfer failed.");
					}
					else
					{
						PRINT_OK();
						PrintUserCmdText(targetChar, L"You have been sent %u credit(s) by " + wscCharName + L".", amount);
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_SendCashID(uint iClientID, wstring wscParam)
{
	if(set_fSendCashTax == -1.0f)//command has been disabled
	{
		PRINT_DISABLED();
		return;
	}
	
	wchar_t wszTaxAmount[72];
	swprintf(wszTaxAmount, L"Transfers are subject to a %f percent tax paid by the sender", set_fSendCashTax);
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /sendcash <charname> <amount>",
		wszTaxAmount,
	};

	wstring targetChar = GetParam(wscParam, ' ', 0);
	wstring amountS = GetParam(wscParam, ' ', 1);
	if(!targetChar.length() || !amountS.length())
	{
		PRINT_ERROR();
	}
	else
	{
		wstring wscCharName = ((wchar_t*)Players.GetActiveCharacterName(iClientID));
		int amount = ToInt(amountS);
		if(amount<=0)
		{
			PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts of money.");
			return;
		}
		if(!HkIsValidClientID(ToInt(targetChar)))
		{
			PrintUserCmdText(iClientID, L"Error: Invalid client-id specified.");
			return;
		}
		targetChar = ((wchar_t*)Players.GetActiveCharacterName(ToInt(targetChar)));
		int availCash = 0;
		HK_ERROR cashError = HkGetCash(ARG_CLIENTID(iClientID), availCash);
		if(!HKHKSUCCESS(cashError))
		{
			PrintUserCmdText(iClientID, L"Error: Cannot check amount of cash available.");
		}
		else
		{
			if(amount*(1.0f+(set_fSendCashTax/100.0f)) > availCash)
			{
				PrintUserCmdText(iClientID, L"Error: You do not have the necessary funds available for the transfer.");
			}
			else
			{
				cashError = HkAddCash(targetChar, amount);
				if(!HKHKSUCCESS(cashError))
				{
					if(cashError==HKE_NO_CHAR_SELECTED)
					{
						PrintUserCmdText(iClientID, L"Error: The target account is at the character selection screen. Try again when they have completely logged on.");
					}
					else if(cashError==HKE_CHAR_DOES_NOT_EXIST)
					{
						PrintUserCmdText(iClientID, L"Error: The specified character does not exist.");
					}
					else if(cashError==HKE_COULD_NOT_DECODE_CHARFILE)
					{
						PrintUserCmdText(iClientID, L"Error: The specified character's file could not be decoded.");
					}
					else
					{
						PrintUserCmdText(iClientID, L"Error: unspecified.");
					}
				}
				else
				{
					cashError = HkAddCash(ARG_CLIENTID(iClientID), (int)(-(amount*(1.0f+(set_fSendCashTax/100.0f)))));
					if(!HKHKSUCCESS(cashError))
					{
						PrintUserCmdText(iClientID, L"Error: The money could not be subtracted from your balance. Oh noes!"); //This really shouldn't happen, but if it does, retract the money sent to the other char.
						cashError = HkAddCash(targetChar, -amount); 
						PrintUserCmdText(targetChar, wscCharName + L" tried to send you money but the transfer failed.");
						PrintUserCmdText(iClientID, L"Transfer failed.");
					}
					else
					{
						PRINT_OK();
						PrintUserCmdText(targetChar, L"You have been sent %u credit(s) by " + wscCharName + L".", amount);
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Afk(uint iClientID, wstring wscParam)
{
	if(wscParam.length())
	{
		ClientInfo[iClientID].wscAfkMsg = wscParam;
		PrintUserCmdText(iClientID, L"AFK message set");
	}
	else
	{
		if(!ClientInfo[iClientID].wscAfkMsg.size())
		{
			ClientInfo[iClientID].wscAfkMsg = L"I'm away from my keyboard right now.";
			PrintUserCmdText(iClientID, L"AFK message set");
		}
		else
		{
			ClientInfo[iClientID].wscAfkMsg = L"";
			PrintUserCmdText(iClientID, L"AFK message unset");
		}
	}
	ClientInfo[iClientID].iSentIds.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_ShieldsDown(uint iClientID, wstring wscParam)
{
	uint iShip;
	pub::Player::GetShip(iClientID, iShip);
	bool bHasShields;
	float fShieldHealth, fMaxShieldHealth;
	pub::SpaceObj::GetShieldHealth(iShip, fShieldHealth, fMaxShieldHealth, bHasShields);
	if(!bHasShields)
	{
		PrintUserCmdText(iClientID, L"Error: no shields present");
		return;
	}
	if(!fShieldHealth)
	{
		PrintUserCmdText(iClientID, L"Error: shields already down");
		return;
	}
	ClientInfo[iClientID].fShieldHealth = fShieldHealth;
	pub::SpaceObj::DrainShields(iShip);
	ClientInfo[iClientID].tmShieldsDownCmd = timeInMS();
	PRINT_OK();
}

void UserCmd_ShieldsUp(uint iClientID, wstring wscParam)
{
	if(!ClientInfo[iClientID].fShieldHealth)
	{
		PrintUserCmdText(iClientID, L"Error: shields down command not previously used");
		return;
	}
	if(timeInMS()-ClientInfo[iClientID].tmShieldsDownCmd > ((mstime)set_iShieldsUpTime)*1000)
	{
		PrintUserCmdText(iClientID, L"Error: you must use the shieldsup command within %u seconds of using the shieldsdown command", set_iShieldsUpTime);
		return;
	}
	uint iShip;
	pub::Player::GetShip(iClientID, iShip);
	HkSetShieldHealth(iShip, DC_HOOK, ClientInfo[iClientID].fShieldHealth);
	ClientInfo[iClientID].fShieldHealth = 0.0f;
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_MarkObj(uint iClientID, wstring wscParam)
{
	uint iShip, iTargetShip;
	pub::Player::GetShip(iClientID, iShip);
	pub::SpaceObj::GetTarget(iShip, iTargetShip);
	char err = HkMarkObject(iClientID, iTargetShip);
	switch(err)
	{
	case 0: 
		//PRINT_OK()
		break;
	case 1:
		PrintUserCmdText(iClientID, L"Error: you must have something targeted to mark it.");
		break;
	case 2:
		PrintUserCmdText(iClientID, L"Error: you cannot mark cloaked ships.");
		break;
	case 3:
		PrintUserCmdText(iClientID, L"Error: object is already marked.");
	default:
		break;
	}
}

void UserCmd_UnMarkObj(uint iClientID, wstring wscParam)
{
	uint iShip, iTargetShip;
	pub::Player::GetShip(iClientID, iShip);
	pub::SpaceObj::GetTarget(iShip, iTargetShip);
	char err = HkUnMarkObject(iClientID, iTargetShip);
	switch(err)
	{
	case 0: 
		//PRINT_OK()
		break;
	case 1:
		PrintUserCmdText(iClientID, L"Error: you must have something targeted to unmark it.");
		break;
	case 2:
		PrintUserCmdText(iClientID, L"Error: object is not marked.");
	default:
		break;
	}
}

void UserCmd_UnMarkAllObj(uint iClientID, wstring wscParam)
{
	HkUnMarkAllObjects(iClientID);
	//PRINT_OK()
}

void UserCmd_MarkObjGroup(uint iClientID, wstring wscParam)
{
	uint iShip, iTargetShip;
	pub::Player::GetShip(iClientID, iShip);
	pub::SpaceObj::GetTarget(iShip, iTargetShip);
	if(!iTargetShip)
	{
		PrintUserCmdText(iClientID, L"Error: you must have something targeted to mark it.");
		return;
	}

	vector<uint> vMembers;
	char szBuf[1024] = "";
	pub::Player::GetGroupMembers(iClientID, *((vector<uint>*)szBuf));
	vMembers = *((vector<uint>*)szBuf);
	for(uint i = 0 ; (i < vMembers.size()); i++)
	{
		if(ClientInfo[vMembers[i]].bIgnoreGroupMark)
			continue;
		uint iClientShip;
		pub::Player::GetShip(vMembers[i], iClientShip);
		if(iClientShip == iTargetShip)
			continue;

		HkMarkObject(vMembers[i], iTargetShip);
	}
	//PRINT_OK()
}

void UserCmd_UnMarkObjGroup(uint iClientID, wstring wscParam)
{
	uint iShip, iTargetShip;
	pub::Player::GetShip(iClientID, iShip);
	pub::SpaceObj::GetTarget(iShip, iTargetShip);
	if(!iTargetShip)
	{
		PrintUserCmdText(iClientID, L"Error: you must have something targeted to mark it.");
		return;
	}

	vector<uint> vMembers;
	char szBuf[1024] = "";
	pub::Player::GetGroupMembers(iClientID, *((vector<uint>*)szBuf));
	vMembers = *((vector<uint>*)szBuf);
	for(uint i = 0 ; (i < vMembers.size()); i++)
	{
		HkUnMarkObject(vMembers[i], iTargetShip);
	}
	//PRINT_OK()
}

void UserCmd_SetIgnoreGroupMark(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /ignoregroupmarks <on|off>",
	};

	if(ToLower(wscParam) == L"off")
	{
		ClientInfo[iClientID].bIgnoreGroupMark = false;
		wstring wscDir, wscFilename;
		CAccount *acc = Players.FindAccountFromClientID(iClientID);
		if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
		{
			string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
			string scSection = "general_" + wstos(wscFilename);
			IniWrite(scUserFile, scSection, "automarkenabled", "no");
			PrintUserCmdText(iClientID, L"Accepting marks from the group");
		}
	}
	else if(ToLower(wscParam) == L"on")
	{
		ClientInfo[iClientID].bIgnoreGroupMark = true;
		wstring wscDir, wscFilename;
		CAccount *acc = Players.FindAccountFromClientID(iClientID);
		if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
		{
			string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
			string scSection = "general_" + wstos(wscFilename);
			IniWrite(scUserFile, scSection, "automarkenabled", "yes");
			PrintUserCmdText(iClientID, L"Ignoring marks from the group");
		}
	}
	else
	{
		PRINT_ERROR();
	}
}

void UserCmd_AutoMark(uint iClientID, wstring wscParam)
{
	if(set_fAutoMarkRadius<=0.0f) //automarking disabled
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /automark <on|off> [radius in KM]",
	};

	wstring wscEnabled = ToLower(GetParam(wscParam, ' ', 0));

	if(!wscParam.length() || (wscEnabled!=L"on" && wscEnabled!=L"off"))
	{
		PRINT_ERROR();
		return;
	}

	wstring wscRadius = GetParam(wscParam, ' ', 1);
	float fRadius = 0.0f;
	if(wscRadius.length())
	{
		fRadius = ToFloat(wscRadius);
	}

	//I think this section could be done better, but I can't think of it now..
	if(!ClientInfo[iClientID].bMarkEverything)
	{
		if(wscRadius.length())
			ClientInfo[iClientID].fAutoMarkRadius = fRadius*1000;
		if(wscEnabled==L"on") //AutoMark is being enabled
		{
			ClientInfo[iClientID].bMarkEverything = true;
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			wstring wscDir, wscFilename;
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "automark", "yes");
				if(wscRadius.length())
					IniWrite(scUserFile, scSection, "automarkradius", ftos(ClientInfo[iClientID].fAutoMarkRadius));
			}
			PrintUserCmdText(iClientID, L"Automarking turned on within a %g KM radius", ClientInfo[iClientID].fAutoMarkRadius/1000);
		}
		else if(wscRadius.length())
		{
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			wstring wscDir, wscFilename;
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "automarkradius", ftos(ClientInfo[iClientID].fAutoMarkRadius));
			}
			PrintUserCmdText(iClientID, L"Radius changed to %g KMs", fRadius);
		}
	}
	else
	{
		if(wscRadius.length())
			ClientInfo[iClientID].fAutoMarkRadius = fRadius*1000;
		if(wscEnabled==L"off") //AutoMark is being disabled
		{
			ClientInfo[iClientID].bMarkEverything = false;
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			wstring wscDir, wscFilename;
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "automark", "no");
				if(wscRadius.length())
					IniWrite(scUserFile, scSection, "automarkradius", ftos(ClientInfo[iClientID].fAutoMarkRadius));
			}
			if(wscRadius.length())
				PrintUserCmdText(iClientID, L"Automarking turned off; radius changed to %g KMs", ClientInfo[iClientID].fAutoMarkRadius/1000);
			else
				PrintUserCmdText(iClientID, L"Automarking turned off");
		}
		else if(wscRadius.length())
		{
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			wstring wscDir, wscFilename;
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "automarkradius", ftos(ClientInfo[iClientID].fAutoMarkRadius));
			}
			PrintUserCmdText(iClientID, L"Radius changed to %g KMs", fRadius);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_BotCombat(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /botcombat <on|off>",
	};

	if(!set_bFlakVersion)
	{
		PRINT_DISABLED();
	}
	else
	{
		wscParam = Trim(wscParam);
		if(ToLower(wscParam)==L"on")
		{
			HkSetRep(ARG_CLIENTID(iClientID), L"fc_uk_grp", -0.9f);
			PRINT_OK();
		}
		else if(ToLower(wscParam)==L"off")
		{
			HkSetRep(ARG_CLIENTID(iClientID), L"fc_uk_grp", 0.9f);
			PRINT_OK();
		}
		else
		{
			PRINT_ERROR();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Dock(uint iClientID, wstring wscParam)
{
	if(set_iMobileDockRadius == -1)
	{
		PRINT_DISABLED();
		return;
	}
	wstring wscError[] = 
	{
		L"Error: Could not dock",
		L"Usage: /dock",
	};

	uint iShip, iDockTarget;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"Error: you are already docked");
		return;
	}
	pub::SpaceObj::GetTarget(iShip, iDockTarget);
	if(((uint)(iDockTarget/1000000000))) //trying to dock at solar
	{
		PrintUserCmdText(iClientID, L"Error: you must use /dock on players");
		return;
	}
	pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("dock"));
	HkIServerImpl::RequestEvent(0, iShip, iDockTarget, 0, 0, iClientID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Transfer(uint iClientID, wstring wscParam)
{
	if(set_iTransferCmdCharge == -1)
	{
		PRINT_DISABLED();
		return;
	}
	wchar_t wszChargeAmount[72];
	swprintf(wszChargeAmount, L"Transfers are subject to a %i credit charge", set_iTransferCmdCharge);
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /transfer <charname> <item>",
		wszChargeAmount,
	};
	
	wstring wscTargetChar = GetParam(wscParam, ' ', 0);
	wstring wscItem = GetParamToEnd(wscParam, ' ', 1);
	if(!wscTargetChar.length() || !wscItem.length())
	{
		PRINT_ERROR();
	}
	else
	{
		int iCash;
		HkGetCash(ARG_CLIENTID(iClientID), iCash);
		if(set_iTransferCmdCharge > iCash)
		{
			PrintUserCmdText(iClientID, L"Error: you do not have enough cash for the transfer");
			return;
		}

		uint iTargetClientID = ToUint(wscTargetChar);
		ushort iItemID = ToUint(wscItem);
		list<CARGO_INFO> lstCargo;
		HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
		bool bFoundItem = false;
		if(iItemID)
		{
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				if(!cargo->bMounted && cargo->iID == iItemID)
				{
					const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
					bool bMultiCount;
					memcpy(&bMultiCount, (char*)gi + 0x70, 1);
					if(!bMultiCount)
					{
						UINT_WRAP uw = UINT_WRAP(cargo->iArchID);
						if(set_btNoTrade->Find(&uw))
						{
							PrintUserCmdText(iClientID, L"Error: item is not intended to be traded.");
						}
						else
						{
							wstring wscCargo = L"";
							HK_ERROR errAdd = HkAddCargo(wscTargetChar, cargo->iArchID, 1, false);
							switch(errAdd)
							{
							case HKE_OK:
								wscCargo = HkGetWStringFromIDS(gi->iIDS);
								HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, 1);
								HkAddCash(ARG_CLIENTID(iClientID), -set_iTransferCmdCharge);
								PRINT_OK();
								PrintUserCmdText(wscTargetChar, L"You have received " + wstring(wscCargo[0]==L'A' || wscCargo[0]==L'E' || wscCargo[0]==L'I' || wscCargo[0]==L'O' || wscCargo[0]==L'U' ? L"an " : L"a ") + wscCargo + L" from " + (wchar_t*)Players.GetActiveCharacterName(iClientID) + L".");
								break;
							case HKE_CHAR_DOES_NOT_EXIST:
								PrintUserCmdText(iClientID, L"Error: specified character does not exist");
								break;
							case HKE_NO_CHAR_SELECTED:
								PrintUserCmdText(iClientID, L"Error: specified player is at the login screen - wait until they completely log in");
								break;
							case HKE_CARGO_WONT_FIT:
								PrintUserCmdText(iClientID, L"Error: specified character does not have enough room in their cargo hold to accomodate the item");
								break;
							default:
								PrintUserCmdText(iClientID, L"Error: unknown");
							}
						}
						bFoundItem = true;
						break;
					}
				}
			}
		}
		else
		{
			wscItem = ToLower(wscItem);
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				if(!cargo->bMounted)
				{
					const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
					bool bMultiCount;
					memcpy(&bMultiCount, (char*)gi + 0x70, 1);
					if(!bMultiCount)
					{
						wstring wscCargo = HkGetWStringFromIDS(gi->iIDS);
						if(wscItem == ToLower(wscCargo))
						{
							UINT_WRAP uw = UINT_WRAP(cargo->iArchID);
							if(set_btNoTrade->Find(&uw))
							{
								PrintUserCmdText(iClientID, L"Error: item is not intended to be traded.");
							}
							else
							{
								HK_ERROR errAdd = HkAddCargo(wscTargetChar, cargo->iArchID, 1, false);
								switch(errAdd)
								{
								case HKE_OK:
									HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, 1);
									HkAddCash(ARG_CLIENTID(iClientID), -set_iTransferCmdCharge);
									PRINT_OK();
									PrintUserCmdText(wscTargetChar, L"You have received " + wstring(wscCargo[0]==L'A' || wscCargo[0]==L'E' || wscCargo[0]==L'I' || wscCargo[0]==L'O' || wscCargo[0]==L'U' ? L"an " : L"a ") + wscCargo + L" from " + (wchar_t*)Players.GetActiveCharacterName(iClientID) + L".");
									break;
								case HKE_CHAR_DOES_NOT_EXIST:
									PrintUserCmdText(iClientID, L"Error: specified character does not exist");
									break;
								case HKE_NO_CHAR_SELECTED:
									PrintUserCmdText(iClientID, L"Error: specified player is at the login screen - wait until they completely log in");
									break;
								case HKE_CARGO_WONT_FIT:
									PrintUserCmdText(iClientID, L"Error: specified character does not have enough room in their cargo hold to accomodate the item");
									break;
								default:
									PrintUserCmdText(iClientID, L"Error: unknown");
								}
							}
							bFoundItem = true;
							break;
						}
					}
				}
			}
		}
		if(!bFoundItem)
		{
			PrintUserCmdText(iClientID, L"Error: could not find specified item.  Use /enumcargo to show a list of transferrable cargo and codes for use with /transfer.");
		}
	}
}

void UserCmd_TransferID(uint iClientID, wstring wscParam)
{
	if(set_iTransferCmdCharge == -1)
	{
		PRINT_DISABLED();
		return;
	}
	wchar_t wszChargeAmount[72];
	swprintf(wszChargeAmount, L"Transfers are subject to a %i credit charge", set_iTransferCmdCharge);
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /transfer$ <client-id> <item>",
		wszChargeAmount,
	};
	
	wstring wscTargetChar = GetParam(wscParam, ' ', 0);
	wstring wscItem = GetParamToEnd(wscParam, ' ', 1);
	if(!wscTargetChar.length() || !wscItem.length())
	{
		PRINT_ERROR();
	}
	else
	{
		int iCash;
		HkGetCash(ARG_CLIENTID(iClientID), iCash);
		if(set_iTransferCmdCharge > iCash)
		{
			PrintUserCmdText(iClientID, L"Error: you do not have enough cash for the transfer");
			return;
		}

		uint iTargetClientID = ToUint(wscTargetChar);
		ushort iItemID = ToUint(wscItem);
		list<CARGO_INFO> lstCargo;
		HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
		bool bFoundItem = false;
		if(iItemID)
		{
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				if(!cargo->bMounted && cargo->iID == iItemID)
				{
					const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
					bool bMultiCount;
					memcpy(&bMultiCount, (char*)gi + 0x70, 1);
					if(!bMultiCount)
					{
						UINT_WRAP uw = UINT_WRAP(cargo->iArchID);
						if(set_btNoTrade->Find(&uw))
						{
							PrintUserCmdText(iClientID, L"Error: item is not intended to be traded.");
						}
						else
						{
							wstring wscCargo = L"";
							HK_ERROR errAdd = HkAddCargo(ARG_CLIENTID(iTargetClientID), cargo->iArchID, 1, false);
							switch(errAdd)
							{
							case HKE_OK:
								wscCargo = HkGetWStringFromIDS(gi->iIDS);
								HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, 1);
								HkAddCash(ARG_CLIENTID(iClientID), -set_iTransferCmdCharge);
								PRINT_OK();
								PrintUserCmdText(iTargetClientID, L"You have received " + wstring(wscCargo[0]==L'A' || wscCargo[0]==L'E' || wscCargo[0]==L'I' || wscCargo[0]==L'O' || wscCargo[0]==L'U' ? L"an " : L"a ") + wscCargo + L" from " + (wchar_t*)Players.GetActiveCharacterName(iClientID) + L".");
								break;
							case HKE_CHAR_DOES_NOT_EXIST:
								PrintUserCmdText(iClientID, L"Error: specified character does not exist");
								break;
							case HKE_NO_CHAR_SELECTED:
								PrintUserCmdText(iClientID, L"Error: specified player is at the login screen - wait until they completely log in");
								break;
							case HKE_CARGO_WONT_FIT:
								PrintUserCmdText(iClientID, L"Error: specified character does not have enough room in their cargo hold to accomodate the item");
								break;
							default:
								PrintUserCmdText(iClientID, L"Error: unknown");
							}
						}
						bFoundItem = true;
						break;
					}
				}
			}
		}
		else
		{
			wscItem = ToLower(wscItem);
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				if(!cargo->bMounted)
				{
					const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
					bool bMultiCount;
					memcpy(&bMultiCount, (char*)gi + 0x70, 1);
					if(!bMultiCount)
					{
						wstring wscCargo = HkGetWStringFromIDS(gi->iIDS);
						if(wscItem == ToLower(wscCargo))
						{
							UINT_WRAP uw = UINT_WRAP(cargo->iArchID);
							if(set_btNoTrade->Find(&uw))
							{
								PrintUserCmdText(iClientID, L"Error: item is not intended to be traded.");
							}
							else
							{
								HK_ERROR errAdd = HkAddCargo(ARG_CLIENTID(iTargetClientID), cargo->iArchID, 1, false);
								switch(errAdd)
								{
								case HKE_OK:
									HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, 1);
									HkAddCash(ARG_CLIENTID(iClientID), -set_iTransferCmdCharge);
									PRINT_OK();
									PrintUserCmdText(iTargetClientID, L"You have received " + wstring(wscCargo[0]==L'A' || wscCargo[0]==L'E' || wscCargo[0]==L'I' || wscCargo[0]==L'O' || wscCargo[0]==L'U' ? L"an " : L"a ") + wscCargo + L" from " + (wchar_t*)Players.GetActiveCharacterName(iClientID) + L".");
									break;
								case HKE_CHAR_DOES_NOT_EXIST:
									PrintUserCmdText(iClientID, L"Error: specified character does not exist");
									break;
								case HKE_NO_CHAR_SELECTED:
									PrintUserCmdText(iClientID, L"Error: specified player is at the login screen - wait until they completely log in");
									break;
								case HKE_CARGO_WONT_FIT:
									PrintUserCmdText(iClientID, L"Error: specified character does not have enough room in their cargo hold to accomodate the item");
									break;
								default:
									PrintUserCmdText(iClientID, L"Error: unknown");
								}
							}
							bFoundItem = true;
							break;
						}
					}
				}
			}
		}
		if(!bFoundItem)
		{
			PrintUserCmdText(iClientID, L"Error: could not find specified item.  Use /enumcargo to show a list of transferrable cargo and codes for use with /transfer.");
		}
	}
}

void UserCmd_EnumCargo(uint iClientID, wstring wscParam)
{
	list<CARGO_INFO> lstCargo;
	HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
	uint iNum = 0;
	foreach(lstCargo, CARGO_INFO, it)
	{
		const GoodInfo *gi = GoodList::find_by_id(it->iArchID);
		if(!gi)
		continue;
		if(!it->bMounted && gi->iIDS)
		{
			PrintUserCmdText(iClientID, L"%u: %s=%u", it->iID, HkGetWStringFromIDS(gi->iIDS).c_str(), it->iCount);
			iNum++;
		}
	}
	if(!iNum)
	PrintUserCmdText(iClientID, L"Error: you have no unmounted equipment or goods");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_DeathPenalty(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /dp [on|off]",
	};

	if(!set_fDeathPenalty)
	{
		PRINT_DISABLED();
		return;
	}

	wscParam = ToLower(Trim(wscParam));

	if(wscParam.length()) //Arguments passed
	{
		if(wscParam == L"off")
		{
			ClientInfo[iClientID].bDisplayDPOnLaunch = false;
			wstring wscDir, wscFilename;
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "DPnotice", "no");
			}
			PRINT_OK();
		}
		else if(wscParam == L"on")
		{
			ClientInfo[iClientID].bDisplayDPOnLaunch = true;
			wstring wscDir, wscFilename;
			CAccount *acc = Players.FindAccountFromClientID(iClientID);
			if(HKHKSUCCESS(HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename)) && HKHKSUCCESS(HkGetAccountDirName(acc, wscDir)))
			{
				string scUserFile = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
				string scSection = "general_" + wstos(wscFilename);
				IniWrite(scUserFile, scSection, "DPnotice", "yes");
			}
			PRINT_OK();
		}
		else
			PRINT_ERROR();
	}
	else
	{
		PrintUserCmdText(iClientID, L"The death penalty is charged immediately when you die.");
		UINT_WRAP uw = UINT_WRAP(Players[iClientID].iShipArchID);
		if(!set_btNoDeathPenalty->Find(&uw))
		{
			float fValue;
			pub::Player::GetAssetValue(iClientID, fValue);
			int iOwed = (int)(fValue * set_fDeathPenalty);
			PrintUserCmdText(iClientID, L"The death penalty for your ship will be " + ToMoneyStr(iOwed) + L" credits.");
			list<wstring> lstItems = HkGetDeathPenaltyItems(iClientID);
			if(lstItems.size())
			{
				if(lstItems.size() == 1)
				{
					PrintUserCmdText(iClientID, L"The following item may be taken from your ship if you do not have enough money: %s.", lstItems.front().c_str());
				}
				else
				{
					wstring wscItems = L"";
					list<wstring>::iterator wscItem = lstItems.begin();
					list<wstring>::iterator wscItemEnd = lstItems.end();
					--wscItemEnd;
					for(;wscItem != wscItemEnd; wscItem++)
					{
						wscItems += *wscItem + L", ";
					}
					wscItems += L"and/or " + lstItems.back();
					PrintUserCmdText(iClientID, L"The following items may be taken from your ship if you do not have enough money: %s.", wscItems.c_str());
				}
			}
			PrintUserCmdText(iClientID, L"If you would like to turn off the death penalty notices, run this command with the argument \"off\".");
		}
		else
		{
			Archetype::Ship *ship = Archetype::GetShip(uw.val);
			if(ship->iIDSName)
			{
				wstring wscShip = HkGetWStringFromIDS(ship->iIDSName);
				PrintUserCmdText(iClientID, L"You don't have to pay the death penalty because you are flying " + wstring(wscShip[0]==L'A'||wscShip[0]==L'E'||wscShip[0]==L'I'||wscShip[0]==L'O'||wscShip[0]==L'U' ? L"an " : L"a ") + wscShip + L'.');
			}
			else
			{
				PrintUserCmdText(iClientID, L"You don't have to pay the death penalty because you are flying a specific ship.");
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_IgnoreUniverse(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /ignoreuniverse <on|off>",
	};

	if(!set_bUserCmdIgnore)
	{
		PRINT_DISABLED();
		return;
	}

	if(ToLower(wscParam) == L"on")
	{
		ClientInfo[iClientID].bIgnoreUniverse = true;
		GET_USERFILE(scUserFile);
		IniWrite(scUserFile, "settings", "IgnoreUniverse", "yes");
	}
	else if(ToLower(wscParam) == L"off")
	{
		ClientInfo[iClientID].bIgnoreUniverse = false;
		GET_USERFILE(scUserFile);
		IniWrite(scUserFile, "settings", "IgnoreUniverse", "no");
	}
	else
	{
		PRINT_ERROR();
		return;
	}
	PRINT_OK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Tag(uint iClientID, wstring wscParam)
{
	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /tag <faction name>"
	};

	if(!set_bUserCmdTag)
	{
		PRINT_DISABLED();
		return;
	}

	wscParam = Trim(wscParam);

	if(wscParam.length())
	{
		wscParam = ToLower(wscParam);
		uint iGroup = 0;
		foreach(lstTagFactions, RepCB, rep)
		{
			uint iIDS = Reputation::get_short_name(rep->iGroup);
			wstring wscFaction = HkGetWStringFromIDS(iIDS);
			wscFaction = ToLower(wscFaction);
			if(wscFaction == wscParam)
			{
				iGroup = rep->iGroup;
				break;
			}
			else
			{
				iIDS = Reputation::get_name(rep->iGroup);
				wscFaction = HkGetWStringFromIDS(iIDS);
				wscFaction = ToLower(wscFaction);
				if(wscFaction == wscParam)
				{
					iGroup = rep->iGroup;
					break;
				}
			}
		}
		if(iGroup)
		{
			int iRep;
			pub::Player::GetRep(iClientID, iRep);
			uint iIDS = Reputation::get_name(iGroup);
			wstring wscFaction = HkGetWStringFromIDS(iIDS);
			float fRep;
			pub::Reputation::GetGroupFeelingsTowards(iRep, iGroup, fRep);
			if(fRep < set_fMinTagRep)
			{
				PrintUserCmdText(iClientID, L"Error: your reputation of %g to " + wscFaction + L" is less than the required %g.", fRep, set_fMinTagRep);
			}
			else
			{
				pub::Reputation::SetAffiliation(iRep, iGroup);
				PrintUserCmdText(iClientID, L"Affiliation changed to " + wscFaction);
			}
		}
		else
		{
			PrintUserCmdText(iClientID, L"Error: could not find faction");
		}
	}
	else
	{
		PRINT_ERROR();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FTL
void UserCmd_FTL(uint iClientID, wstring wscParam) 
{
	GET_USERFILE(scUserFile);
    ClientInfo[iClientID].aFTL = false;
    uint iShip; 
    pub::Player::GetShip(iClientID, iShip);
    uint iTarget; 
    pub::SpaceObj::GetTarget(iShip, iTarget);
    uint iFuseID = CreateID(FtlFX.c_str());
    IObjRW *ship = GetIObjRW(iShip);
    uint iType;
    pub::SpaceObj::GetType(iTarget, iType);
    bool ShipFound=false;
	uint iShipArchID;
	pub::Player::GetShipID(iClientID, iShipArchID);
    UINT_WRAP uw = UINT_WRAP(iShipArchID);
	if(set_btBattleShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFreighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(ShipFound)
	{
		ClientInfo[iClientID].MsgFTL = true;
        if(iType==64 || iType==2048) //target is jumphole/gate 
		{
            float HullNow, MaxHull, FTLPower;
            pub::SpaceObj::GetHealth(iShip , HullNow, MaxHull);
            FTLPower = (HullNow / MaxHull);	
            if(FTLPower>0.5f)
			{
                if(timeInMS() > ClientInfo[iClientID].iFTL)
				{
                    HkInitFTLFuel(iClientID);
                    if(ClientInfo[iClientID].bHasFTLFuel)
					{
				        IniWrite(scUserFile, "FTL", "coords", wstos(wscParam));
	                    mstime tmFTL = timeInMS();
	                    ClientInfo[iClientID].iFTL = timeInMS() + set_FTLTimer;
                        pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("new_coordinates_received"));
                        HkUnLightFuse(ship, iFuseID, 0.0f);
                        HkLightFuse(ship, iFuseID, 0.0f, 0.5f, -1.0f);
                        ClientInfo[iClientID].tmSpawnTime = timeInMS();
	                    ClientInfo[iClientID].aFTL = true;
						ClientInfo[iClientID].Msg = true;
                        HkInstantDock(ARG_CLIENTID(iClientID), iTarget);
					}
                    else
					{
                        PrintUserCmdText(iClientID, L"Error: you have no Fuel");
					}
				}
                else
				{
                    PrintUserCmdText(iClientID, L"Error: FTL Not Charged Time Left %i ms",(int) (ClientInfo[iClientID].iFTL - timeInMS()));
				}
			}
            else
			{
                PrintUserCmdText(iClientID, L"Ship Repairs needed");
			}
   
		}
        else 
		{ 
            PrintUserCmdText(iClientID, L"Error: you must use this command on jumpholes/gates"); 
		}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Error: no ftl drive found");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FTL
void UserCmd_SFTL(uint iClientID, wstring wscParam) 
{
    uint iShip;
    pub::Player::GetShip(iClientID, iShip);
    uint iFuseID = CreateID(FtlFX.c_str());
    IObjRW *ship = GetIObjRW(iShip);
    float HullNow, MaxHull, FTLPower;
    pub::SpaceObj::GetHealth(iShip , HullNow, MaxHull);
    FTLPower = (HullNow / MaxHull);
	bool ShipFound=false;
	uint iShipArchID;
	pub::Player::GetShipID(iClientID, iShipArchID);
    UINT_WRAP uw = UINT_WRAP(iShipArchID);
	if(set_btBattleShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFreighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(ShipFound)
	{
		ClientInfo[iClientID].MsgFTL = true;
        if(FTLPower>0.5f)
		{
            if(timeInMS() > ClientInfo[iClientID].iFTL)
			{ 
                HkInitFTLFuel(iClientID);
                if(ClientInfo[iClientID].bHasFTLFuel)
				{
                    Vector VLmy; 
                    Matrix MyTemp;
                    pub::SpaceObj::GetLocation(iShip, VLmy, MyTemp); 
                    VLmy.x = 0;
                    VLmy.y = 0;
                    VLmy.z = 0;
                    wstring Vx = GetParam(wscParam, ' ', 0);
                    if(Vx.length() < 8)
					{VLmy.x = ToFloat(Vx);} 
                    wstring Vy = GetParam(wscParam, ' ', 1); 
                    if(Vy.length() < 8)
					{VLmy.y = ToFloat(Vy);}
                    wstring Vz = GetParam(wscParam, ' ', 2); 
                    if(Vz.length() < 8)
					{VLmy.z = ToFloat(Vz);}
                    ClientInfo[iClientID].tmSpawnTime = timeInMS();
                    pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("new_coordinates_received")); 
                    HkUnLightFuse(ship, iFuseID, 0.0f);
                    HkLightFuse(ship, iFuseID, 0.0f, 0.5f, -1.0f);
                    HkBeamInSys(ARG_CLIENTID(iClientID), VLmy, MyTemp);
	                HkUnLightFuse(ship, iFuseID, 0.0f);
                    HkLightFuse(ship, iFuseID, 0.0f, 0.5f, -1.0f);
	                pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("launch_procedure_complete"));
	                mstime tmFTL = timeInMS();
	                ClientInfo[iClientID].iFTL = timeInMS() + set_FTLTimer;
                    ClientInfo[iClientID].bHasFTLFuel = false;
				    ClientInfo[iClientID].Msg = true;
				}
                else
				{
                    PrintUserCmdText(iClientID, L"Error: you have no Fuel");
				}
			}
            else
			{
                PrintUserCmdText(iClientID, L"Error: FTL Not Charged Time Left %i ms",(int) (ClientInfo[iClientID].iFTL - timeInMS()));
			}
		}
        else
		{
            PrintUserCmdText(iClientID, L"Ship Repairs needed");
		}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Error: no ftl drive found");
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FTL
void UserCmd_CHORDS(uint iClientID, wstring wscParam) 
{
    uint iShip; 
    pub::Player::GetShip(iClientID, iShip);  
    uint iTarget; 
    pub::SpaceObj::GetTarget(iShip, iTarget);  
    Vector myLocation;
    Matrix myLocationm;
    pub::SpaceObj::GetLocation(iTarget, myLocation, myLocationm);
    PrintUserCmdText(iClientID, L"x %f",myLocation.x);
    PrintUserCmdText(iClientID, L"y %f",myLocation.y);
    PrintUserCmdText(iClientID, L"z %f",myLocation.z);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FTL
void UserCmd_JUMP(uint iClientID, wstring wscParam) 
{
    uint iShip;
    pub::Player::GetShip(iClientID, iShip);
    uint iFuseID = CreateID(FtlFX.c_str());
    IObjRW *ship = GetIObjRW(iShip);
    float HullNow, MaxHull, FTLPower;
    pub::SpaceObj::GetHealth(iShip , HullNow, MaxHull);
    FTLPower = (HullNow / MaxHull);
	bool ShipFound=false;
	uint iShipArchID;
	pub::Player::GetShipID(iClientID, iShipArchID);
    UINT_WRAP uw = UINT_WRAP(iShipArchID);
	if(set_btBattleShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFreighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(set_btFighterShipArchIDs->Find(&uw))
	{
		ShipFound=true;
	}
	if(ShipFound)
	{
		ClientInfo[iClientID].MsgFTL = true;
	    if(FTLPower>0.5f)
		{
            if(timeInMS() > ClientInfo[iClientID].iFTL)
			{ 
                HkInitFTLFuel(iClientID);
                if(ClientInfo[iClientID].bHasFTLFuel)
				{
                    Vector myJump; 
                    Matrix myJumpx;
                    pub::SpaceObj::GetLocation(iShip, myJump, myJumpx); 
                    myJump.x = 0;
                    myJump.y = 0;
                    myJump.z = 0;
                    uint iTarget;
                    pub::SpaceObj::GetTarget(iShip, iTarget);  
                    pub::SpaceObj::GetLocation(iTarget, myJump, myJumpx);
                    ClientInfo[iClientID].tmSpawnTime = timeInMS();
                    pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("new_coordinates_received"));
                    HkUnLightFuse(ship, iFuseID, 0.0f);
                    HkLightFuse(ship, iFuseID, 0.0f, 0.5f, -1.0f);
                    HkBeamInSys(ARG_CLIENTID(iClientID), myJump, myJumpx);
	                HkUnLightFuse(ship, iFuseID, 0.0f);
                    HkLightFuse(ship, iFuseID, 0.0f, 0.5f, -1.0f);
	                pub::Player::SendNNMessage(iClientID, pub::GetNicknameId("launch_procedure_complete"));
	                mstime tmFTL = timeInMS();
	                ClientInfo[iClientID].iFTL = timeInMS() + set_FTLTimer;
                    ClientInfo[iClientID].bHasFTLFuel = false;
	                ClientInfo[iClientID].Msg = true;
				}
                else
				{
                    PrintUserCmdText(iClientID, L"Error: you have no Fuel");
				}
			}
            else
			{
                PrintUserCmdText(iClientID, L"Error: FTL Not Charged Time Left %i ms",(int) (ClientInfo[iClientID].iFTL - timeInMS()));
			}
		}
        else
		{
            PrintUserCmdText(iClientID, L"Ship Repairs needed");
		}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Error: no ftl drive found");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Loc
void UserCmd_loc(uint iClientID, wstring wscParam)
{
	uint iShip;
    pub::Player::GetShip(iClientID, iShip);
    uint iShipArchID;
    pub::Player::GetShipID(iClientID, iShipArchID);
    UINT_WRAP uw = UINT_WRAP(iShipArchID);
	if(set_btScannerShipArchIDs->Find(&uw))
	{
	wstring wscCharname = GetParam(wscParam, ' ', 0);
	uint iShip;
	uint iPlayerId;
	Vector myLocation;
    Matrix myLocationm;
	if (0 < wscCharname.length())
	{
		iPlayerId = HkGetClientIdFromCharname(wscCharname); 
        if(!HkIsValidClientID(iPlayerId) || HkIsInCharSelectMenu(iPlayerId)) 
		{ 
            PrintUserCmdText(iClientID, L"Error: Player not found"); 
            return; 
		}
		if(HkGetPlayerSystem(iClientID)==HkGetPlayerSystem(iPlayerId))
		{
           pub::Player::GetShip(iPlayerId, iShip);
	       pub::SpaceObj::GetLocation(iShip, myLocation, myLocationm);
	       PrintUserCmdText(iClientID, L"x %f",myLocation.x);
           PrintUserCmdText(iClientID, L"y %f",myLocation.y);
           PrintUserCmdText(iClientID, L"z %f",myLocation.z);
		}
		else
		{
			PrintUserCmdText(iClientID, L"Out of Scanner Range");
		}
	}
	else
	{
		pub::Player::GetShip(iClientID, iShip);
	    pub::SpaceObj::GetLocation(iShip, myLocation, myLocationm);
	    PrintUserCmdText(iClientID, L"x %f",myLocation.x);
        PrintUserCmdText(iClientID, L"y %f",myLocation.y);
        PrintUserCmdText(iClientID, L"z %f",myLocation.z);
	}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Reserved For Scout Ships Only");
	}
}

void UserCmd_loc$(uint iClientID, wstring wscParam)
{
	uint iShip;
    pub::Player::GetShip(iClientID, iShip);
    uint iShipArchID;
    pub::Player::GetShipID(iClientID, iShipArchID);
    UINT_WRAP uw = UINT_WRAP(iShipArchID);
	if(set_btScannerShipArchIDs->Find(&uw))
	{
	uint iShip;
	Vector myLocation;
    Matrix myLocationm;

	wstring wscError[] = 
	{
		L"Error: Invalid parameters",
		L"Usage: /loc$ <client-id>",
	};

	wstring wscClientID = GetParam(wscParam, ' ', 0);

	if(!wscClientID.length())
		PRINT_ERROR();

	uint iPlayerId = ToInt(wscClientID);
	if(!HkIsValidClientID(iPlayerId) || HkIsInCharSelectMenu(iPlayerId))
	{
		PrintUserCmdText(iClientID, L"Error: Invalid client-id");
		return;
	}
	if(HkGetPlayerSystem(iClientID)==HkGetPlayerSystem(iPlayerId))
	{
        pub::Player::GetShip(iPlayerId, iShip);
	    pub::SpaceObj::GetLocation(iShip, myLocation, myLocationm);
	    PrintUserCmdText(iClientID, L"x %f",myLocation.x);
        PrintUserCmdText(iClientID, L"y %f",myLocation.y);
        PrintUserCmdText(iClientID, L"z %f",myLocation.z);
	}
	else
	{
		PrintUserCmdText(iClientID, L"Out of Scanner Range");
	}
	}
	else
	{
		PrintUserCmdText(iClientID, L"Reserved For Scout Ships Only");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserCmd_Ping(uint iClientID, wstring wscParam)
{
	int Ping = ClientInfo[iClientID].iAveragePing;
	int Loss = ClientInfo[iClientID].iAverageLoss;
	if(Ping > 0)
	{
		PrintUserCmdText(iClientID, L"Ping=%u Loss=%u", Ping, Loss);
	}
	else
	{
		PrintUserCmdText(iClientID, L"Not Calculated Try Again");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Store(uint iClientID, wstring wscParam)
{
	uint iShip; 
    pub::Player::GetShip(iClientID, iShip);
    uint iTarget; 
    pub::SpaceObj::GetTarget(iShip, iTarget);
    uint iType; 
    pub::SpaceObj::GetType(iTarget, iType);
    if(iType==8192)
	{
		uint wscGoods = ToUint(GetParam(wscParam, ' ', 0));
	    int wscCount = ToInt(GetParamToEnd(wscParam, ' ', 1));
		if(wscParam.find(L"all") != -1)
		{
			list<CARGO_INFO> lstCargo;
			HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
		        if(!gi)
	      	    continue;
				if(!cargo->bMounted && gi->iIDS)
				{
				    GET_USERSTORE(scUserStore);
				    int iGoods=0;
				    iGoods=IniGetI(scUserStore ,wstos(HkGetPlayerSystem(iClientID)), utos(cargo->iArchID), 0);
                    IniWrite(scUserStore, wstos(HkGetPlayerSystem(iClientID)), utos(cargo->iArchID), itos(iGoods+cargo->iCount));
				    HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, cargo->iCount);
				}
			}
			PrintUserCmdText(iClientID, L"Ok");
			return;
		}
		if(wscCount<=0)
		{
		   PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts");
		   return;
		}
	    if(wscGoods == 0 || wscCount == 0)
		{
		    PrintUserCmdText(iClientID, L"you must enter the right values try /enumcargo id amount");
		}
		else
		{
			list<CARGO_INFO> lstCargo;
			HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			foreach(lstCargo, CARGO_INFO, cargo)
			{
				if(cargo->iID == wscGoods)
				{
					if(cargo->iCount - wscCount <0)
					{
						PrintUserCmdText(iClientID, L"You dont have enough");
					}
					else
					{
						GET_USERSTORE(scUserStore);
						int iGoods=0;
						iGoods=IniGetI(scUserStore ,wstos(HkGetPlayerSystem(iClientID)), utos(cargo->iArchID), 0);
                        IniWrite(scUserStore, wstos(HkGetPlayerSystem(iClientID)), utos(cargo->iArchID), itos(iGoods+wscCount));
						HkRemoveCargo(ARG_CLIENTID(iClientID), cargo->iID, wscCount);
						PrintUserCmdText(iClientID, L"Ok");
					}
				}

			}
		}
	}
	else
	{
		PrintUserCmdText(iClientID, L"You must target a Storage Container");
	}
}

void UserCmd_Ustore(uint iClientID, wstring wscParam)
{
	GET_USERSTORE(scUserStore);
	list<INISECTIONVALUE> lstGoods;
	IniGetSection(scUserStore,wstos(HkGetPlayerSystem(iClientID)),  lstGoods);
	int count=0;
	uint wscGoods;
	foreach(lstGoods, INISECTIONVALUE, it3)
	{
		count = count+1;
		if(ToInt(GetParam(wscParam, ' ', 0)) == count)
		{
			wscGoods = ToUint(it3->scKey);
		}
	}
	int wscCount = ToInt(GetParamToEnd(wscParam, ' ', 1));
	if(wscCount<=0)
	{
		PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts");
		return;
	}
	if(wscGoods == 0 || wscCount == 0)
	{
		PrintUserCmdText(iClientID, L"you must enter the right values try /ustore id amount");
	}
	else
	{
	   uint iShip; 
       pub::Player::GetShip(iClientID, iShip);
       uint iTarget; 
       pub::SpaceObj::GetTarget(iShip, iTarget);
       uint iType; 
       pub::SpaceObj::GetType(iTarget, iType);
       if(iType==8192)
	   {
		   int iGoods = IniGetI(scUserStore ,wstos(HkGetPlayerSystem(iClientID)), utos(wscGoods), 0);
		   if(iGoods == 0)
		   {
			   PrintUserCmdText(iClientID, L"Goods not found");
		   }
		   else
		   {
			   if(iGoods - wscCount < 0)
			   {
				   PrintUserCmdText(iClientID, L"You dont have enough");
			   }
			   else
			   {
				   Archetype::Equipment *eq = Archetype::GetEquipment(wscGoods);
				   const GoodInfo *id = GoodList::find_by_archetype(wscGoods);
				   float fRemainingHold;
				   pub::Player::GetRemainingHoldSize(iClientID, fRemainingHold);
				   if(id->type == 0)
				   {
		              if(eq->fVolume*wscCount > fRemainingHold)
					  {
					      PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					      wscCount = ToInt(ftos(fRemainingHold));
					  }
				   }
				   int nCount=0;
				   int sCount=0;
				   uint iNanobotsID = 2911012559;
	               uint iShieldBatID = 2596081674;
	               Archetype::Ship *ship = Archetype::GetShip(Players[iClientID].iShipArchID);
				   list<CARGO_INFO> lstCargo;
			       HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			       foreach(lstCargo, CARGO_INFO, cargo)
				   {
				      if(cargo->iArchID == iNanobotsID){nCount = cargo->iCount;}
				      if(cargo->iArchID == iShieldBatID){sCount = cargo->iCount;}
                      if(wscGoods == iNanobotsID)
					  {
						 uint amount = nCount+wscCount;
			             if(amount > ship->iMaxNanobots)
						 {
				            PrintUserCmdText(iClientID, L"Warning: the number of nanobots is greater than allowed");
						    wscCount = ship->iMaxNanobots-nCount;
						 }
					  }
                      if(wscGoods == iShieldBatID)
					  {
                         uint amount = sCount+wscCount;
			             if(amount > ship->iMaxShieldBats)
						 {
				            PrintUserCmdText(iClientID, L"Warning: the number of shield batteries is greater than allowed");
						    wscCount = ship->iMaxShieldBats-sCount;
						 }
					  }
				   }
				   if(id->type == 1)
				   {
					   int uCount=0;
					   foreach(lstCargo, CARGO_INFO, cargo)
					   {
						   if(cargo->iArchID == wscGoods){uCount = cargo->iCount;}
						   if(wscCount+uCount > MAX_PLAYER_AMMO)
						   {
							   PrintUserCmdText(iClientID, L"Warning: the number of goods is greater than allowed");
							   wscCount=MAX_PLAYER_AMMO-uCount;
						   }
						   if(eq->fVolume*wscCount > fRemainingHold)
						   {
					          PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					          wscCount = 0;
						   }
					   }
				   }
				   if(eq->iGunType)//if not here items will stack and not show the amount
				   {
					   PrintUserCmdText(iClientID, L"Ok");
				   }
                   HkAddCargo(ARG_CLIENTID(iClientID), wscGoods, wscCount, false);
				   IniWrite(scUserStore, wstos(HkGetPlayerSystem(iClientID)), utos(wscGoods), itos(iGoods-wscCount));
				   const GoodInfo *gi = GoodList::find_by_id(wscGoods);
		           PrintUserCmdText(iClientID, L"You recived %s = %u from store", HkGetWStringFromIDS(gi->iIDS).c_str(), wscCount);
			   }
		   }
	   }
	   else
	   {
		  PrintUserCmdText(iClientID, L"You must target a Storage Container");
	   }
	}
}

void UserCmd_Istore(uint iClientID, wstring wscParam)
{
	list<INISECTIONVALUE> lstGoods;
	GET_USERSTORE(scUserStore);
	IniGetSection(scUserStore,wstos(HkGetPlayerSystem(iClientID)),  lstGoods);
	IniDelSection(scUserStore,wstos(HkGetPlayerSystem(iClientID)));
	int count=0;
	foreach(lstGoods, INISECTIONVALUE, it3)
	{
		if(ToInt(it3->scValue)>0)
		{
		   count = count+1;
		   const GoodInfo *gi = GoodList::find_by_id(ToUint(it3->scKey));
		   if(!gi)
			   continue;
		   PrintUserCmdText(iClientID, L"%u=%s=%u", count, HkGetWStringFromIDS(gi->iIDS).c_str(), ToInt(it3->scValue));
		   IniWrite(scUserStore, wstos(HkGetPlayerSystem(iClientID)), it3->scKey, it3->scValue);
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_Bank(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"You cant use that here");
		return;
	}
	GET_USERSTORE(scUserStore);
	int availCash = 0;
	HkGetCash(ARG_CLIENTID(iClientID), availCash);
	int amount = ToInt(GetParam(wscParam, ' ', 0));

	int samout = IniGetI(scUserStore, "Bank", "money", 0);
	if(amount == 0)
	{
		PrintUserCmdText(iClientID, L"You have %u in Bank", samout);
		return;
	}
	if(amount<=0)
	{
		PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts of money.");
		return;
	}
	if(amount > availCash)
	{
		PrintUserCmdText(iClientID, L"You dont have enough");
		return;
	}
	int amountfix = amount/1000000;
	int samoutfix = samout/1000000;
	int load = amountfix+samoutfix;
	int load1=1900;
	if(load > load1)
	{
		PrintUserCmdText(iClientID, L"You can only bank 1900000000 max");
		return;
	}
	else
	{
		HkAddCash(ARG_CLIENTID(iClientID),-amount);
		IniDelSection(scUserStore, "Bank");
		IniWrite(scUserStore, "Bank", "money", itos(samout+amount));
		PrintUserCmdText(iClientID, L"Ok");
	}
}

void UserCmd_wBank(uint iClientID, wstring wscParam)
{
	GET_USERSTORE(scUserStore);
	int availCash = 0;
	HkGetCash(ARG_CLIENTID(iClientID), availCash);
	int amount = ToInt(GetParam(wscParam, ' ', 0));

	int samout = IniGetI(scUserStore, "Bank", "money", 0);
	if(amount == 0)
	{
		PrintUserCmdText(iClientID, L"You have %u in Bank", samout);
		return;
	}
	if(amount<=0)
	{
		PrintUserCmdText(iClientID, L"Error: You cannot transfer negative or zero amounts of money.");
		return;
	}
	if(amount > samout)
	{
		PrintUserCmdText(iClientID, L"You dont have enough");
		return;
	}
	int availCashfix = availCash/1000000;
	int amountfix = amount/1000000;
	int load=availCashfix+amountfix;
	int load1=1900;
	if(load > load1)
	{
		PrintUserCmdText(iClientID, L"You can only carry 1900000000");
		return;
	}
	else
	{
		HkAddCash(ARG_CLIENTID(iClientID),amount);
		IniDelSection(scUserStore, "Bank");
		IniWrite(scUserStore, "Bank", "money", itos(samout-amount));
		PrintUserCmdText(iClientID, L"Ok");
	}
}

void UserCmd_eBuild(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"You cant use that here");
		return;
	}
    int counter=0;
	int countgoods = 0;
	int countgoodsT = 0;
	int BuildNumber = ToInt(GetParam(wscParam, ' ', 0));
	list<INISECTIONVALUE> lstBuilds;
	list<INISECTIONVALUE> lstParts;
	list<CARGO_INFO> lstCargo;
	IniGetSection(set_scBuildFilee, "build", lstBuilds);
	if(wscParam.length()>0)
	{
		foreach(lstBuilds,INISECTIONVALUE,builds)
		{
		    if(counter==BuildNumber)
			{
				IniGetSection(set_scBuildFilee, (builds->scKey), lstParts);
		        foreach(lstParts,INISECTIONVALUE,parts)
				{
					uint PartsID = CreateID(parts->scKey.c_str());
					countgoodsT ++;
	                HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
	                uint iNum = 0;
	                foreach(lstCargo, CARGO_INFO, cargo)
					{
		                const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
		                if(!gi)
		                continue;
		                if(!cargo->bMounted && gi->iIDS)
						{
			                iNum++;
							if(cargo->iArchID==PartsID && cargo->iCount>=ToInt(parts->scValue))
							{
								countgoods ++;
							}
						}
					}
	                if(!iNum)
					{
	                    PrintUserCmdText(iClientID, L"Error: you have no equipment or goods to build this");
						return;
					}
				}
			}
			counter++;
		}
		if(countgoods==countgoodsT)
		{
			counter=0;
			foreach(lstBuilds,INISECTIONVALUE,builds)
			{
				uint wscGoods = CreateID(builds->scKey.c_str());
				if(counter==BuildNumber)
				{
					int wscCount = 1;
					Archetype::Equipment *eq = Archetype::GetEquipment(wscGoods);
				    const GoodInfo *id = GoodList::find_by_archetype(wscGoods);
					if(!id)
						continue;
				    float fRemainingHold;
				    pub::Player::GetRemainingHoldSize(iClientID, fRemainingHold);
				    if(id->type == 0)
					{
		                if(eq->fVolume*wscCount > fRemainingHold)
						{
					        PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					        wscCount = 0;
						}
					}
				    int nCount=0;
				    int sCount=0;
				    uint iNanobotsID = 2911012559;
	                uint iShieldBatID = 2596081674;
	                Archetype::Ship *ship = Archetype::GetShip(Players[iClientID].iShipArchID);
				    list<CARGO_INFO> lstCargo;
			        HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			        foreach(lstCargo, CARGO_INFO, cargo)
					{
				        if(cargo->iArchID == iNanobotsID){nCount = cargo->iCount;}
				        if(cargo->iArchID == iShieldBatID){sCount = cargo->iCount;}
                        if(wscGoods == iNanobotsID)
						{
						    uint amount = nCount+wscCount;
			                if(amount > ship->iMaxNanobots)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of nanobots is greater than allowed");
						        wscCount = 0;
							}
						}
                        if(wscGoods == iShieldBatID)
						{
                            uint amount = sCount+wscCount;
			                if(amount > ship->iMaxShieldBats)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of shield batteries is greater than allowed");
						        wscCount = 0;
							}
						}
					}
				    if(id->type == 1)
					{
					    int uCount=0;
					    foreach(lstCargo, CARGO_INFO, cargo)
						{
						    if(cargo->iArchID == wscGoods){uCount = cargo->iCount;}
						    if(wscCount+uCount > MAX_PLAYER_AMMO)
							{
							    PrintUserCmdText(iClientID, L"Warning: the number of goods is greater than allowed");
							    wscCount=0;
							}
						    if(eq->fVolume*wscCount > fRemainingHold)
							{
					            PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					            wscCount = 0;
							}
						}
					}
					if(wscCount==1)
					{
						IniGetSection(set_scBuildFilee, (builds->scKey), lstParts);
					    foreach(lstParts,INISECTIONVALUE,parts)
						{
							uint PartsID = CreateID(parts->scKey.c_str());
						    HkRemoveCargoByGoodID(iClientID, PartsID, ToInt(parts->scValue));
						}
					    HkAddCargo(ARG_CLIENTID(iClientID), wscGoods, wscCount, false);
						PrintUserCmdText(iClientID, L"Item Built");
					}
					if(eq->iGunType)//if not here items will stack and not show the amount
					{
					    
					}
				}
				counter++;
			}
			PrintUserCmdText(iClientID, L"OK");
		}
		else
		{
			PrintUserCmdText(iClientID, L"You dont have enough materials to build that");
		}
		return;
	}
	foreach(lstBuilds,INISECTIONVALUE,builds)
	{
		uint BuildsID = CreateID(builds->scKey.c_str());
		IniGetSection(set_scBuildFilee, (builds->scKey), lstParts);
		const GoodInfo *gi = GoodList::find_by_id(BuildsID);
		PrintUserCmdText(iClientID, L"%u %s", counter, HkGetWStringFromIDS(gi->iIDS).c_str());
		counter++;
		foreach(lstParts,INISECTIONVALUE,parts)
		{
			uint PartsID = CreateID(parts->scKey.c_str());
			const GoodInfo *gp = GoodList::find_by_id(PartsID);
		    PrintUserCmdText(iClientID, L"  %s=%u", HkGetWStringFromIDS(gp->iIDS).c_str(), ToInt(parts->scValue));
		}
        PrintUserCmdText(iClientID, L"");
	}
}

void UserCmd_wBuild(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"You cant use that here");
		return;
	}
    int counter=0;
	int countgoods = 0;
	int countgoodsT = 0;
	int BuildNumber = ToInt(GetParam(wscParam, ' ', 0));
	list<INISECTIONVALUE> lstBuilds;
	list<INISECTIONVALUE> lstParts;
	list<CARGO_INFO> lstCargo;
	IniGetSection(set_scBuildFilew, "build", lstBuilds);
	if(wscParam.length()>0)
	{
		foreach(lstBuilds,INISECTIONVALUE,builds)
		{
		    if(counter==BuildNumber)
			{
				IniGetSection(set_scBuildFilew, (builds->scKey), lstParts);
		        foreach(lstParts,INISECTIONVALUE,parts)
				{
					uint PartsID = CreateID(parts->scKey.c_str());
					countgoodsT ++;
	                HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
	                uint iNum = 0;
	                foreach(lstCargo, CARGO_INFO, cargo)
					{
		                const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
		                if(!gi)
		                continue;
		                if(!cargo->bMounted && gi->iIDS)
						{
			                iNum++;
							if(cargo->iArchID==PartsID && cargo->iCount>=ToInt(parts->scValue))
							{
								countgoods ++;
							}
						}
					}
	                if(!iNum)
					{
	                    PrintUserCmdText(iClientID, L"Error: you have no equipment or goods to build this");
						return;
					}
				}
			}
			counter++;
		}
		if(countgoods==countgoodsT)
		{
			counter=0;
			foreach(lstBuilds,INISECTIONVALUE,builds)
			{
				uint wscGoods = CreateID(builds->scKey.c_str());
				if(counter==BuildNumber)
				{
					int wscCount = 1;
					Archetype::Equipment *eq = Archetype::GetEquipment(wscGoods);
				    const GoodInfo *id = GoodList::find_by_archetype(wscGoods);
					if(!id)
						continue;
				    float fRemainingHold;
				    pub::Player::GetRemainingHoldSize(iClientID, fRemainingHold);
				    if(id->type == 0)
					{
		                if(eq->fVolume*wscCount > fRemainingHold)
						{
					        PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					        wscCount = 0;
						}
					}
				    int nCount=0;
				    int sCount=0;
				    uint iNanobotsID = 2911012559;
	                uint iShieldBatID = 2596081674;
	                Archetype::Ship *ship = Archetype::GetShip(Players[iClientID].iShipArchID);
				    list<CARGO_INFO> lstCargo;
			        HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			        foreach(lstCargo, CARGO_INFO, cargo)
					{
				        if(cargo->iArchID == iNanobotsID){nCount = cargo->iCount;}
				        if(cargo->iArchID == iShieldBatID){sCount = cargo->iCount;}
                        if(wscGoods == iNanobotsID)
						{
						    uint amount = nCount+wscCount;
			                if(amount > 0)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of nanobots is greater than allowed");
						        wscCount = ship->iMaxNanobots-nCount;
							}
						}
                        if(wscGoods == iShieldBatID)
						{
                            uint amount = sCount+wscCount;
			                if(amount > ship->iMaxShieldBats)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of shield batteries is greater than allowed");
						        wscCount = 0;
							}
						}
					}
				    if(id->type == 1)
					{
					    int uCount=0;
					    foreach(lstCargo, CARGO_INFO, cargo)
						{
						    if(cargo->iArchID == wscGoods){uCount = cargo->iCount;}
						    if(wscCount+uCount > MAX_PLAYER_AMMO)
							{
							    PrintUserCmdText(iClientID, L"Warning: the number of goods is greater than allowed");
							    wscCount=0;
							}
						    if(eq->fVolume*wscCount > fRemainingHold)
							{
					            PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					            wscCount = 0;
							}
						}
					}
					if(wscCount==1)
					{
						IniGetSection(set_scBuildFilew, (builds->scKey), lstParts);
					    foreach(lstParts,INISECTIONVALUE,parts)
						{
							uint PartsID = CreateID(parts->scKey.c_str());
						    HkRemoveCargoByGoodID(iClientID, PartsID, ToInt(parts->scValue));
						}
					    HkAddCargo(ARG_CLIENTID(iClientID), wscGoods, wscCount, false);
						PrintUserCmdText(iClientID, L"Item Built");
					}
					if(eq->iGunType)//if not here items will stack and not show the amount
					{
					
					}
				}
				counter++;
			}
			PrintUserCmdText(iClientID, L"OK");
		}
		else
		{
			PrintUserCmdText(iClientID, L"You dont have enough materials to build that");
		}
		return;
	}
	foreach(lstBuilds,INISECTIONVALUE,builds)
	{
		uint BuildsID = CreateID(builds->scKey.c_str());
		IniGetSection(set_scBuildFilew, (builds->scKey), lstParts);
		const GoodInfo *gi = GoodList::find_by_id(BuildsID);
		PrintUserCmdText(iClientID, L"%u %s", counter, HkGetWStringFromIDS(gi->iIDS).c_str());
		counter++;
		foreach(lstParts,INISECTIONVALUE,parts)
		{
			uint PartsID = CreateID(parts->scKey.c_str());
			const GoodInfo *gp = GoodList::find_by_id(PartsID);
		    PrintUserCmdText(iClientID, L"  %s=%u", HkGetWStringFromIDS(gp->iIDS).c_str(), ToInt(parts->scValue));
		}
        PrintUserCmdText(iClientID, L"");
	}
}

void UserCmd_aBuild(uint iClientID, wstring wscParam)
{
	uint iShip = 0;
	pub::Player::GetShip(iClientID, iShip);
	if(!iShip)
	{
		PrintUserCmdText(iClientID, L"You cant use that here");
		return;
	}
    int counter=0;
	int countgoods = 0;
	int countgoodsT = 0;
	int BuildNumber = ToInt(GetParam(wscParam, ' ', 0));
	list<INISECTIONVALUE> lstBuilds;
	list<INISECTIONVALUE> lstParts;
	list<CARGO_INFO> lstCargo;
	IniGetSection(set_scBuildFilea, "build", lstBuilds);
	if(wscParam.length()>0)
	{
		foreach(lstBuilds,INISECTIONVALUE,builds)
		{
		    if(counter==BuildNumber)
			{
				IniGetSection(set_scBuildFilea, (builds->scKey), lstParts);
		        foreach(lstParts,INISECTIONVALUE,parts)
				{
					uint PartsID = CreateID(parts->scKey.c_str());
					countgoodsT ++;
	                HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
	                uint iNum = 0;
	                foreach(lstCargo, CARGO_INFO, cargo)
					{
		                const GoodInfo *gi = GoodList::find_by_id(cargo->iArchID);
		                if(!gi)
		                continue;
		                if(!cargo->bMounted && gi->iIDS)
						{
			                iNum++;
							if(cargo->iArchID==PartsID && cargo->iCount>=ToInt(parts->scValue))
							{
								countgoods ++;
							}
						}
					}
	                if(!iNum)
					{
	                    PrintUserCmdText(iClientID, L"Error: you have no equipment or goods to build this");
						return;
					}
				}
			}
			counter++;
		}
		if(countgoods==countgoodsT)
		{
			counter=0;
			foreach(lstBuilds,INISECTIONVALUE,builds)
			{
				uint wscGoods = CreateID(builds->scKey.c_str());
				if(counter==BuildNumber)
				{
					int wscCount = 1;
					Archetype::Equipment *eq = Archetype::GetEquipment(wscGoods);
				    const GoodInfo *id = GoodList::find_by_archetype(wscGoods);
					if(!id)
						continue;
				    float fRemainingHold;
				    pub::Player::GetRemainingHoldSize(iClientID, fRemainingHold);
				    if(id->type == 0)
					{
		                if(eq->fVolume*wscCount > fRemainingHold)
						{
					        PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					        wscCount = 0;
						}
					}
				    int nCount=0;
				    int sCount=0;
				    uint iNanobotsID = 2911012559;
	                uint iShieldBatID = 2596081674;
	                Archetype::Ship *ship = Archetype::GetShip(Players[iClientID].iShipArchID);
				    list<CARGO_INFO> lstCargo;
			        HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
			        foreach(lstCargo, CARGO_INFO, cargo)
					{
				        if(cargo->iArchID == iNanobotsID){nCount = cargo->iCount;}
				        if(cargo->iArchID == iShieldBatID){sCount = cargo->iCount;}
                        if(wscGoods == iNanobotsID)
						{
						    uint amount = nCount+wscCount;
			                if(amount > ship->iMaxNanobots)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of nanobots is greater than allowed");
						        wscCount = 0;
							}
						}
                        if(wscGoods == iShieldBatID)
						{
                            uint amount = sCount+wscCount;
			                if(amount > ship->iMaxShieldBats)
							{
				                PrintUserCmdText(iClientID, L"Warning: the number of shield batteries is greater than allowed");
						        wscCount = 0;
							}
						}
					}
				    if(id->type == 1)
					{
					    int uCount=0;
					    foreach(lstCargo, CARGO_INFO, cargo)
						{
						    if(cargo->iArchID == wscGoods){uCount = cargo->iCount;}
						    if(wscCount+uCount > MAX_PLAYER_AMMO)
							{
							    PrintUserCmdText(iClientID, L"Warning: the number of goods is greater than allowed");
							    wscCount=0;
							}
						    if(eq->fVolume*wscCount > fRemainingHold)
							{
					            PrintUserCmdText(iClientID, L"You dont have enough cargo space");
					            wscCount = 0;
							}
						}
					}
					if(wscCount==1)
					{
						IniGetSection(set_scBuildFilea, (builds->scKey), lstParts);
					    foreach(lstParts,INISECTIONVALUE,parts)
						{
							uint PartsID = CreateID(parts->scKey.c_str());
						    HkRemoveCargoByGoodID(iClientID, PartsID, ToInt(parts->scValue));
						}
					    HkAddCargo(ARG_CLIENTID(iClientID), wscGoods, wscCount, false);
						PrintUserCmdText(iClientID, L"Item Built");
					}
					if(eq->iGunType)//if not here items will stack and not show the amount
					{
					    
					}
				}
				counter++;
			}
			PrintUserCmdText(iClientID, L"OK");
		}
		else
		{
			PrintUserCmdText(iClientID, L"You dont have enough materials to build that");
		}
		return;
	}
	foreach(lstBuilds,INISECTIONVALUE,builds)
	{
		uint BuildsID = CreateID(builds->scKey.c_str());
		IniGetSection(set_scBuildFilea, (builds->scKey), lstParts);
		const GoodInfo *gi = GoodList::find_by_id(BuildsID);
		PrintUserCmdText(iClientID, L"%u %s", counter, HkGetWStringFromIDS(gi->iIDS).c_str());
		counter++;
		foreach(lstParts,INISECTIONVALUE,parts)
		{
			uint PartsID = CreateID(parts->scKey.c_str());
			const GoodInfo *gp = GoodList::find_by_id(PartsID);
		    PrintUserCmdText(iClientID, L"  %s=%u", HkGetWStringFromIDS(gp->iIDS).c_str(), ToInt(parts->scValue));
		}
        PrintUserCmdText(iClientID, L"");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserCmd_Kills(uint iClientID, wstring wscParam)
{
	wstring wscClientID = GetParam(wscParam, ' ', 0);
	int iNumKills;
	wstring mainrank;
	list<wstring> lstLines;
	int count;
	if(!wscClientID.length())
	{
		wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
		HkReadCharFile(wscCharname, lstLines);
		pub::Player::GetNumKills(iClientID, iNumKills);
		PrintUserCmdText(iClientID, L"number of pvp kills = %i",iNumKills);
		foreach(lstLines, wstring, str)
		{
		    if(!ToLower((*str)).find(L"ship_type_killed"))
			{
				uint iShipArchID=0;
				wstring wsTemp = GetParamToEnd((*str), ' ', 2).c_str();
				count = ToInt(GetParam((wsTemp), ',', 1).c_str());
				iNumKills+=count;
				iShipArchID = ToUint(GetParam((wsTemp), ',', 0).c_str());
				Archetype::Ship *ship = Archetype::GetShip(iShipArchID);
				PrintUserCmdText(iClientID, L"npc kills  %s %i",HkGetWStringFromIDS(ship->iIDSName).c_str(),count);
			}
		}
		foreach(lstRanks, INISECTIONVALUE, rank)
		{
			int lvl = ToInt(rank->scValue);
			if(lvl<=iNumKills)
			{
				mainrank=stows(rank->scKey);
			}
		}
		PrintUserCmdText(iClientID, L"your total kills is = %i" ,iNumKills);
		PrintUserCmdText(iClientID, L"your rank is = %s" ,mainrank.c_str());
		return;
	}

	uint iClientIDPlayer = HkGetClientIdFromCharname(wscClientID);
	if(iClientIDPlayer == -1)
	{
		PrintUserCmdText(iClientID, L"ERROR player not found");
		return;
	}
	HkReadCharFile(wscClientID, lstLines);
	pub::Player::GetNumKills(iClientIDPlayer, iNumKills);
	PrintUserCmdText(iClientID, L"number of pvp kills = %i",iNumKills);
	foreach(lstLines, wstring, str)
	{
		if(!ToLower((*str)).find(L"ship_type_killed"))
		{
			uint iShipArchID=0;
			wstring wsTemp = GetParamToEnd((*str), ' ', 2).c_str();
			count = ToInt(GetParam((wsTemp), ',', 1).c_str());
			iNumKills+=count;
			iShipArchID = ToUint(GetParam((wsTemp), ',', 0).c_str());
			Archetype::Ship *ship = Archetype::GetShip(iShipArchID);
			PrintUserCmdText(iClientID, L"npc kills  %s %i",HkGetWStringFromIDS(ship->iIDSName).c_str(),count);
		}
	}
	foreach(lstRanks, INISECTIONVALUE, rank)
	{
		int lvl = ToInt(rank->scValue);
		if(lvl<=iNumKills)
		{
			mainrank=stows(rank->scKey);
		}
	}
	PrintUserCmdText(iClientID, L"there total kills is = %i" ,iNumKills);
	PrintUserCmdText(iClientID, L"there rank is = %s" ,mainrank.c_str());
}

void UserCmd_Kills$(uint iClientID, wstring wscParam)
{
	wstring wscClientTarget = GetParam(wscParam, ' ', 0);
	int iNumKills;
	wstring mainrank;
	list<wstring> lstLines;
	int count;
    uint iClientIDTarget = ToInt(wscClientTarget);
	if(!HkIsValidClientID(iClientIDTarget) || HkIsInCharSelectMenu(iClientIDTarget))
	{
		PrintUserCmdText(iClientID, L"ERROR player not found");
		return;
	}
	wstring wscClientID = ((wchar_t*)Players.GetActiveCharacterName(iClientIDTarget));
	HkReadCharFile(wscClientID, lstLines);
	pub::Player::GetNumKills(iClientIDTarget, iNumKills);
	PrintUserCmdText(iClientID, L"number of pvp kills = %i",iNumKills);
	foreach(lstLines, wstring, str)
	{
		if(!ToLower((*str)).find(L"ship_type_killed"))
		{
			uint iShipArchID=0;
			wstring wsTemp = GetParamToEnd((*str), ' ', 2).c_str();
			count = ToInt(GetParam((wsTemp), ',', 1).c_str());
			iNumKills+=count;
			iShipArchID = ToUint(GetParam((wsTemp), ',', 0).c_str());
			Archetype::Ship *ship = Archetype::GetShip(iShipArchID);
			PrintUserCmdText(iClientID, L"npc kills  %s %i",HkGetWStringFromIDS(ship->iIDSName).c_str(),count);
		}
	}
	foreach(lstRanks, INISECTIONVALUE, rank)
	{
		int lvl = ToInt(rank->scValue);
		if(lvl<=iNumKills)
		{
			mainrank=stows(rank->scKey);
		}
	}
	PrintUserCmdText(iClientID, L"there total kills is = %i" ,iNumKills);
	PrintUserCmdText(iClientID, L"there rank is = %s" ,mainrank.c_str());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
list<BOUNTY_HUNT> lstBountyHunt;
void UserCmd_BountyHunt(uint iClientID, const wstring wscParam)
{
	if(!set_bBhEnabled)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Usage: /bountyhunt <charname> <credits> [<minutes>]",
		L"or: /bountyhuntid <id> <credits> [<minutes>]",
		L" -> offers a reward <credits> for killing <charname> within the next <minutes>",
	};


	mstime nowtime = timeInMS();
	wstring wscTarget = GetParam(wscParam, ' ', 0);
	wstring wscCredits = GetParam(wscParam, ' ', 1);
	wstring wscTime = GetParam(wscParam, ' ', 2);
	if(!wscTarget.length() || !wscCredits.length())
	{
		PRINT_ERROR_NORETURN();// kein return
		if(lstBountyHunt.begin() != lstBountyHunt.end()){
			PrintUserCmdText(iClientID, L"offered bountyhunts:");
			foreach(lstBountyHunt, BOUNTY_HUNT, it)
			{
				PrintUserCmdText(iClientID,L" kill %s and earn %u credits (%u minutes left)",it->wscTarget.c_str(),it->uiCredits,(uint)((it->msEnd - nowtime) / 60000));
			}
		}
		return;
	}
	
	uint uiTargetID = 0, uiPrize = 0, uiTime = 30;
	uiPrize = wcstol(wscCredits.c_str(),NULL,10);
	uiTime = wcstol(wscTime.c_str(),NULL,10);
	uiTargetID = HkGetClientIdFromCharname(wscTarget);

	int iRankTarget;
	pub::Player::GetRank(uiTargetID, iRankTarget);

	//checks
	if(uiTargetID == -1 || HkIsInCharSelectMenu(uiTargetID) )
	{
		PrintUserCmdText(iClientID, L"Char %s not online", wscTarget.c_str());
		return;
	}

	if(iRankTarget < set_iLowLevelProtect)
	{
		PrintUserCmdText(iClientID, L"Lowlevelchars may not be hunted.");
		return;
	}
	if(uiPrize > 50000000 || uiPrize == 0)
	{
		PrintUserCmdText(iClientID, L"prize %u not allowed (min:1 max:50000000)", uiPrize);
		return;
	}
	if(uiTime < 1 || uiTime > 240)
	{
		PrintUserCmdText(iClientID, L"Hunting time: 30 minutes");
		uiTime = 30;
	}
//prize checken
	wstring wscInitiatior = L"the nomad king";
	if(iClientID)
	{ //wenn 0, dann Systemauftrag
		uint uiClientCash;
		pub::Player::InspectCash(iClientID, (int &)uiClientCash);
		if(uiClientCash < uiPrize)
		{
			PrintUserCmdText(iClientID, L"you do not possess enough credits");
			return;
		}
		pub::Player::AdjustCash(iClientID,-uiPrize);
		wscInitiatior = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	}


	BOUNTY_HUNT bh;
	bh.uiInitiatorID = iClientID;
	bh.msEnd = nowtime + (mstime)(uiTime * 60000);
	bh.wscInitiator = wscInitiatior;
	bh.uiCredits = uiPrize;
	bh.wscTarget = wscTarget;
	bh.uiTargetID = uiTargetID;
	lstBountyHunt.push_back(bh);

	PrintUniverseText(L"BOUNTYHUNT: %s offers %u credits for killing %s in %u minutes.",bh.wscInitiator.c_str(),bh.uiCredits,bh.wscTarget.c_str(),uiTime);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_BountyHuntId(uint iClientID, const wstring wscParam)
{
	if(!set_bBhEnabled)
	{
		PRINT_DISABLED();
		return;
	}

	wstring wscError[] = 
	{
		L"Usage: /bountyhunt <charname> <credits> [<minutes>]",
		L"or: /bountyhuntid <id> <credits> [<minutes>]",
		L" -> offers a reward <credits> for killing <charname|id> within the next <minutes>",
	};


	mstime nowtime = timeInMS();
	wstring wscTarget = GetParam(wscParam, ' ', 0);
	wstring wscCredits = GetParam(wscParam, ' ', 1);
	wstring wscTime = GetParam(wscParam, ' ', 2);
	if(!wscTarget.length() || !wscCredits.length())
	{
		PRINT_ERROR_NORETURN();// kein return
		if(lstBountyHunt.begin() != lstBountyHunt.end())
		{
			PrintUserCmdText(iClientID, L"offered bountyhunts:");
			foreach(lstBountyHunt, BOUNTY_HUNT, it)
			{
				PrintUserCmdText(iClientID,L" kill %s and earn %u credits (%u minutes left)",it->wscTarget.c_str(),it->uiCredits,(uint)((it->msEnd - nowtime) / 60000));
			}
		}
		return;
	}

	uint iClientIDTarget = ToInt(wscTarget);
	if(!HkIsValidClientID(iClientIDTarget) || HkIsInCharSelectMenu(iClientIDTarget))
	{
		PrintUserCmdText(iClientID, L"Error: Invalid client-id");
		return;
	}

	wstring wscParamNew;
	wstring wscCharName;
	wscCharName = ((wchar_t*)Players.GetActiveCharacterName(iClientIDTarget));
	wscParamNew = wstring(wscCharName + L" "+ wscCredits + L" " +wscTime);
	UserCmd_BountyHunt(iClientID, wscParamNew);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserCmd_Restart(uint iClientID, wstring wscParam)
{
	if(wscParam.length()>0)
	{
		foreach(lstRestart,INISECTIONVALUE,restart)
		{
			if(stows((*restart).scKey) == wscParam)
			{
				string scRep = IniGetS(set_scShipsFile, wstos(wscParam), "rep", "");
				if(scRep.length()>0)
				{
					uint iRepGroupID;
	                pub::Reputation::GetReputationGroup(iRepGroupID, scRep.c_str());
		            int iPlayerRep;
	                pub::Player::GetRep(iClientID, iPlayerRep);
		            pub::Reputation::SetReputation(iPlayerRep, iRepGroupID, 0.9f);
					wstring wscCharname = ((wchar_t*)Players.GetActiveCharacterName(iClientID));
					HkSaveChar(wscCharname);
				}
	            HkPlayerRestart(iClientID,wscParam);
			}
		}
	}
	else
	{
		foreach(lstRestart,INISECTIONVALUE,restart)
		{
		    PrintUserCmdText(iClientID, L"<faction> %s",stows(restart->scKey).c_str());
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UserCmd_List(uint iClientID, wstring wscParam)
{
	if(wscParam.length()>0)
	{
		uint uiTargetID = HkGetClientIdFromCharname(wscParam);
		if(uiTargetID == -1 || HkIsInCharSelectMenu(uiTargetID) )
		{
		    PrintUserCmdText(iClientID, L"Error: %s not found", wscParam.c_str());
		    return;
		}
		wstring wscPlayer = ((wchar_t*)Players.GetActiveCharacterName(uiTargetID));
		wstring wscShip = HkGetWStringFromIDS(Archetype::GetShip(Players[uiTargetID].iShipArchID)->iIDSName);
		wstring Faction;
		HkGetAffiliation(wscPlayer,Faction);
		int FactionLess = strlen((char*)Faction.c_str());
		if (FactionLess!=0)
		{
			PrintUserCmdText(iClientID, L"%s [%s] %s",wscPlayer.c_str(),Faction.c_str(),wscShip.c_str());
		}
		else
		{
		    PrintUserCmdText(iClientID, L"%s [%s] %s",wscPlayer.c_str(),L"unknown",wscShip.c_str());
		}
	}
	else
	{
		struct PlayerData *pPD = 0;
		while(pPD = Players.traverse_active(pPD))
		{
			uint iPlayerID = HkGetClientIdFromPD(pPD);
			if(ClientInfo[iPlayerID].tmF1TimeDisconnect)
				continue;
			wstring wscPlayer = ((wchar_t*)Players.GetActiveCharacterName(iPlayerID));
			wstring wscShip = HkGetWStringFromIDS(Archetype::GetShip(Players[iPlayerID].iShipArchID)->iIDSName);
			wstring Faction;
			HkGetAffiliation(wscPlayer,Faction);
			int FactionLess = strlen((char*)Faction.c_str());
		    if (FactionLess!=0)
			{
			    PrintUserCmdText(iClientID, L"%s [%s] %s",wscPlayer.c_str(),Faction.c_str(),wscShip.c_str());
			}
		    else
			{
		        PrintUserCmdText(iClientID, L"%s [%s] %s",wscPlayer.c_str(),L"unknown",wscShip.c_str());
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserCmd_Pods(uint iClientID, wstring wscParam)
{
	list<CARGO_INFO> lstCargo;
	HkEnumCargo(ARG_CLIENTID(iClientID), lstCargo, 0);
	uint iNum = 0;
	PrintUserCmdText(iClientID,L"[Hold]");
	foreach(lstCargo, CARGO_INFO, it)
	{
		const GoodInfo *gi = GoodList::find_by_id(it->iArchID);
		if(!gi)
		continue;
		if(!it->bMounted && gi->iIDS)
		{
			PrintUserCmdText(iClientID, L"  %s=%u", HkGetWStringFromIDS(gi->iIDS).c_str(), it->iCount);
			iNum++;
		}
	}
	if(!iNum)
	PrintUserCmdText(iClientID, L"  you have no goods in your hold");
	foreach(lstCargo, CARGO_INFO, it)
	{
		const GoodInfo *gi = GoodList::find_by_id(it->iArchID);
		if(!gi)
		   continue;
		if(it->bMounted && gi->iIDS)
		{
			CARGO_POD FindPod = CARGO_POD(it->iArchID, 0);
	        CARGO_POD *pod = set_btCargoPod->Find(&FindPod);
			if(it->iID && pod)
			{
				CAccount *acc = Players.FindAccountFromClientID(iClientID);
	            wstring wscDir;
	            HkGetAccountDirName(acc, wscDir);
	            string scUserStore = scAcctPath + wstos(wscDir) + "\\flhookuser.ini";
	            wstring wscFilename;
	            HkGetCharFileName(ARG_CLIENTID(iClientID), wscFilename);
	            string scSection = utos(it->iID) + "_" + wstos(wscFilename);
				int counter=0;
				list<INISECTIONVALUE> amount;
				amount.clear();
				IniGetSection(scUserStore, scSection, amount);
				foreach(amount,INISECTIONVALUE,lstI)
				{
					counter+=ToInt(stows(lstI->scValue).c_str());
				}
				PrintUserCmdText(iClientID,L"[Cargo Pod = %s capacity left = %i]",HkGetWStringFromIDS(gi->iIDS).c_str(),pod->capacity-counter);
				IniDelSection(scUserStore,scSection);
				foreach(amount,INISECTIONVALUE,lstgoods)
				{
					if(ToInt(stows(lstgoods->scValue).c_str())>0)
					{
						IniWrite(scUserStore, scSection, lstgoods->scKey, lstgoods->scValue);
					    const GoodInfo *ls = GoodList::find_by_id(ToInt(stows(lstgoods->scKey).c_str()));;
		                if(!ls)
		                   continue;
					    PrintUserCmdText(iClientID, L"  %s=%s", HkGetWStringFromIDS(ls->iIDS).c_str(), stows(lstgoods->scValue).c_str());
					}
				}
			}
		}      
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UserCmd_RestartInfo(uint iClientID, wstring wscParam)
{
	time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
	PrintUserCmdText(iClientID, L"Server Local Time %s",stows(asctime(timeinfo)).c_str());
	foreach(lstServerRestart, INISECTIONVALUE, t)
	{
		string scTimers = t->scKey;
		PrintUserCmdText(iClientID, L"schedule restart = %s",stows(scTimers).c_str());

	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CMDHELPINFO
{
	bool	bShowCmd;
	wchar_t *wscCommand;
	wchar_t *wscHelpTxt;
};

CMDHELPINFO HelpInfo[] = 
{
	{ true,	L"set diemsg",		L"Usage: /set diemsg xxx\n  while xxx must be one of the following values:\n  - all = all deaths will be displayed\n  - system = only display deaths occurring in the system you are currently in\n  - self = only display deaths the player is involved in(either as victim or killer)\n  - none = don't show any death-messages"},
	{ true, L"set diemsgsize",	L"Usage: /set diemsgsize <small/default>\n  - change the size of the diemsgs"},
	{ true, L"set chatfont",	L"Usage: /set chatfont <size> <style>\n  <size>: small, default or big\n  <style>: default, bold, italic or underline\n  this lets every user adjust the appearance of the chat-messages"},
	{ false,L"set",				L"See set diemsg, set diemsgsize, and set chatfont"},
	{ true, L"ignorelist",		L"Usage: /ignorelist\n  display ignore list"},
	{ true, L"delignore",		L"Usage: /delignore <id> [<id2> <id3> ...]\n  delete ignore entry"},
	{ true, L"ignore",			L"Usage: /ignore <charname> [<flags>]\n            /ignore$ <client-id> [<flags>]\n  ignore chat from certain players"},
	{ true, L"ignoreuniverse",	L"Usage: /ignoreuniverse <on|off>\n  Ignores chat from the universe channel"},
	{ true, L"autobuy",			L"Usage: /autobuy <on|off>\n  - on: saves the current configuration of unmounted items and attempts to\n  complete that configuration upon entering a base.\n  - off: deletes the current configuration."},
	{ true, L"ids",				L"Usage: /ids\n  show client-ids of all players"},
	{ true, L"id",				L"Usage: /id\n  show own client-id"},
	{ false,L"i",				L"Usage: /invite$ <client-id>\n            /i$ <client-id>\n            /i <charname>\n  invite player to group"},
	{ true, L"invite",			HelpInfo[10].wscHelpTxt},
	{ true, L"inviteall",		L"Usage: /inviteall [name part]\n            /ia [name part]\n  Invites all players on server to join into a group with you.\n  If [name part] is present, it only invites those who have [name part] in their name."},
	{ false,L"ia",				HelpInfo[12].wscHelpTxt},
	{ true, L"cloak",			L"Usage: /cloak\n                /c\n  Cloak the ship if a cloaking device is present on it.\n  If used when cloaked, shows the time remaining for the cloak."},
	{ false,L"c",				HelpInfo[14].wscHelpTxt},
	{ true, L"uncloak",			L"Usage: /uncloak\n                /uc\n  Uncloak the ship if it is cloaked."},
	{ true, L"rename",			L"Usage: /rename <new name>\n  Renames the logged-in character to the new name.  Enter the command to find out how much it costs."},
	{ true, L"sendcash",		L"Usage: /sendcash <charname> <amount>\n            /sendcash$ <client-id> <amount>\n  Sends <amount> of credits to <charname>.  Charges tax to the sender."},
	{ true, L"shieldsdown",		L"Usage: /shieldsdown\n            /sd\n  Makes the shields on your ship fail."},
	{ false,L"sd",				HelpInfo[19].wscHelpTxt},
	{ true, L"shieldsup",		L"Usage: /shieldsup\n            /su\n  Makes the shields on your ship recharge to the levels they were at when the shields\n  down command was used.  Note that it will not make your shields go up instantly."},
	{ false,L"su",				HelpInfo[21].wscHelpTxt},
	{ true, L"mark",			L"Usage: /mark\n            /m\n  Makes the selected object appear in the important section of the contacts and\n  have an arrow on the side of the screen, as well as have > and < on the sides\n  of the selection box."},
	{ false,L"m",				HelpInfo[23].wscHelpTxt},
	{ true, L"unmark",			L"Usage: /unmark\n            /um\n  Unmarks the selected object marked with the /mark (/m) command."},
	{ false,L"um",				HelpInfo[25].wscHelpTxt},
	{ true, L"groupmark",		L"Usage: /groupmark\n            /gm\n  Marks selected object for the entire group."},
	{ false,L"gm",				HelpInfo[27].wscHelpTxt},
	{ true, L"groupunmark",		L"Usage: /groupunmark\n            /gum\n  Unmarks the selected object for the entire group."},
	{ false,L"gum",				HelpInfo[29].wscHelpTxt},
	{ true,	L"ignoregroupmarks",L"Usage: /ignoregroupmarks <on|off>\n  Ignores marks from others in your group."},
	{ true, L"automark",		L"Usage: /automark <on|off> [radius in KM]\n  Automatically marks all ships in KM radius.  Bots are marked automatically in the\n  range specified whether on or off.  If you want to completely disable automarking,\n  set the radius to a number <= 0."},
	{ true, L"afk",				L"Usage: /afk [Message]\n  Sets away from keyboard message, which is automatically sent to people who message you.\n  If no message is appended to the command and AFK is set, then AFK is unset; otherwise the message is updated."},
	{ true, L"botcombat",		L"Usage: /botcombat <on|off>\n  Turns your bots (AI Companion option) hostile to you for combat purposes."},
	{ true, L"dock",			L"Usage: /dock\n            /d\n  Docks your ship at a valid player ship, allowing you to repair and buy equipment\n  as with a normal base."},
	{ false,L"d",				HelpInfo[35].wscHelpTxt},
	{ true, L"transfer",		L"Usage: /transfer <charname> <item>            \n/transfer$ <client-id> <item>\n  Transfers <item> to <charname>.  Valid items are those that are not grouped\n  (like commodities and ammo).  Enter the command to find how how much it costs."},
	{ true, L"enumcargo",		L"Usage: /enumcargo\n  Prints out a number for each cargo item that can be used with the /transfer command."},
	{ true, L"dp",				L"Usage: /dp [on|off]\n  Shows information about the death penalty.  Also sets whether a notice about how\n  much the death penalty costs is shown upon launch."},
	{ true, L"tag",				L"/tag <faction>\n  Changes your affiliation to <faction>, making it appear beside your name ingame."},
	{ true, L"coords",			L"Usage: /c\n  shows the targets coordinates"},
	{ true, L"ftl",				L"Usage: /ftl\n  ftl X Y Z"},
	{ true, L"sftl",			L"Usage: /sftl\n  ftl X Y Z"},
	{ true, L"j",			    L"Usage: /j\n  jump to target"},
	{ true, L"jump",			L"Usage: /jump\n  jump to target"},
	{ true, L"ping",		    L"Usage: /ping"},
	{ true, L"p",		    	L"Usage: /p"},
	{ true, L"loc",			    L"Usage: /loc location of point in space a player is at X Y Z"},
	{ true, L"loc$",			L"Usage: /loc$ location of point in space a player is at X Y Z"},
	{ true, L"store",			L"Usage: /store <n> <n> <----amount or /store all the number is from /enumcargo"},
	{ true, L"unstore",			L"Usage: /unstore <n> <n> <----amount the number is from /enumcargo"},
	{ true, L"sinfo",		    L"Usage: /sinfo shows all goods in storage"},
	{ true, L"bank",		    L"Usage: /bank lets you bank money to the store"},
	{ true, L"wbank",		    L"Usage: /wbank lets you withdraw money to the store"},
	{ true, L"bequip",		    L"Usage: /bequip shows the build list /bequip <n> will build it"},
	{ true, L"bwep",	    	L"Usage: /bwep shows the build list /bwep <n> will build it"},
	{ true, L"bammo",		    L"Usage: /bammo shows the build list /bammo <n> will build it"},
	{ true, L"bountyhunt",		L"Usage: /bountyhunt <charname> <credits> [<minutes>]"},
	{ true, L"bountyhuntid",	L"Usage: /bountyhuntid <id> <credits> [<minutes>]"},
	{ true, L"kills",		    L"Usage: /kills <charname> shows the kills the player has leave empty to view self"},
	{ true, L"kills$",		    L"Usage: /kills <id> shows the kills the player has"},
	{ true, L"restart",		    L"Usage: /template <faction> rember you will lose the current ship you have allso all cargo and equipment and given the default ship off the faction you chose"},
	{ true, L"list",            L"Usage: /list <id> shows players faction and ship"},
	{ true, L"rinfo",           L"Usage: /rinfo to show you the schedule restarts"},
	{ true, L"pods",            L"Usage: /pods shows the cargo stored in the cargo pods"},
	{ true, L"help [command]",	L"Usage: /help [command]\n            /? [command]\n  Gets help on available commands"},
	{ false,L"?",				HelpInfo[40].wscHelpTxt},
};

void UserCmd_Help(uint iClientID, wstring wscParam)
{
	if(!Trim(wscParam).length()) //No args, print out list of commands
	{
		for(uint i = 0; (i < sizeof(HelpInfo)/sizeof(CMDHELPINFO)); i++)
		{
			if(HelpInfo[i].bShowCmd)
			{
				wstring wscCommand = L"/";
				wscCommand += HelpInfo[i].wscCommand;
				foreach(lstHelpHide,INISECTIONVALUE,hide)
				{
					wstring hidecom = stows(hide->scKey);
					if(wscCommand.find(hidecom)==0)
					{
						wscCommand = L"";
					}
				}
				if(wscCommand.length()>0)
				{
				    PrintUserCmdText(iClientID, wscCommand);
				}
			}
		}
	}
	else
	{
		wscParam = ReplaceStr(wscParam, L"/", L"");
		wscParam = ReplaceStr(wscParam, L"$", L"");
		
		for(uint i = 0; (i < sizeof(HelpInfo)/sizeof(CMDHELPINFO)); i++)
		{
			if(HelpInfo[i].wscCommand == ToLower(wscParam))
			{
				wstring wscHelpText = HelpInfo[i].wscHelpTxt;
				int iFind = wscHelpText.find(L"\n", 0);
				while(iFind!=-1)
				{
					PrintUserCmdText(iClientID, wscHelpText.substr(0, iFind));
					wscHelpText = wscHelpText.substr(iFind+1);
					iFind = wscHelpText.find(L"\n", 0);
				}
				PrintUserCmdText(iClientID, wscHelpText);
				return;
			}
		}
		PrintUserCmdText(iClientID, L"Could not find help for the specified command");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

USERCMD UserCmds[] =
{
	{ L"/set diemsg",			UserCmd_SetDieMsg},
	{ L"/set diemsgsize",		UserCmd_SetDieMsgSize},
	{ L"/set chatfont",			UserCmd_SetChatFont},
	{ L"/ignorelist",			UserCmd_IgnoreList},
	{ L"/delignore",			UserCmd_DelIgnore},
	{ L"/ignore",				UserCmd_Ignore},
	{ L"/ignore$",				UserCmd_IgnoreID},
	{ L"/autobuy",				UserCmd_AutoBuy},
	{ L"/ids",					UserCmd_IDs},
	{ L"/id",					UserCmd_ID},
	{ L"/invite",				UserCmd_Invite},
	{ L"/i",					UserCmd_Invite},
	{ L"/i$",					UserCmd_InviteID},
	{ L"/invite$",				UserCmd_InviteID},
	{ L"/ia",					UserCmd_InviteAll},
	{ L"/inviteall",			UserCmd_InviteAll},
	{ L"/cloak",				UserCmd_Cloak},
	{ L"/c",					UserCmd_Cloak},
	{ L"/uncloak",				UserCmd_UnCloak},
	{ L"/uc",					UserCmd_UnCloak},
	{ L"/rename",				UserCmd_Rename},
	{ L"/sendcash",				UserCmd_SendCash},
	{ L"/sendcash$",			UserCmd_SendCashID},
	{ L"/afk",					UserCmd_Afk},
	{ L"/shieldsdown",			UserCmd_ShieldsDown},
	{ L"/sd",					UserCmd_ShieldsDown},
	{ L"/shieldsup",			UserCmd_ShieldsUp},
	{ L"/su",					UserCmd_ShieldsUp},
	{ L"/mark",					UserCmd_MarkObj},
	{ L"/m",					UserCmd_MarkObj},
	{ L"/unmark",				UserCmd_UnMarkObj},
	{ L"/um",					UserCmd_UnMarkObj},
	{ L"/unmarkall",			UserCmd_UnMarkAllObj},
	{ L"/uma",					UserCmd_UnMarkAllObj},
	{ L"/groupmark",			UserCmd_MarkObjGroup},
	{ L"/gm",					UserCmd_MarkObjGroup},
	{ L"/groupunmark",			UserCmd_UnMarkObjGroup},
	{ L"/gum",					UserCmd_UnMarkObjGroup},
	{ L"/ignoregroupmarks",		UserCmd_SetIgnoreGroupMark},
	{ L"/automark",				UserCmd_AutoMark},
	{ L"/botcombat",			UserCmd_BotCombat},
	{ L"/dock",					UserCmd_Dock},
	{ L"/d",					UserCmd_Dock},
	{ L"/transfer",				UserCmd_Transfer},
	{ L"/transfer$",			UserCmd_TransferID},
	{ L"/enumcargo",			UserCmd_EnumCargo},
	{ L"/dp",					UserCmd_DeathPenalty},
	{ L"/ignoreuniverse",		UserCmd_IgnoreUniverse},
	{ L"/tag",					UserCmd_Tag},
	{ L"/ftl",                  UserCmd_FTL},
    { L"/sftl",                 UserCmd_SFTL},
	{ L"/coords",               UserCmd_CHORDS},
    { L"/j",                    UserCmd_JUMP},
    { L"/jump",                 UserCmd_JUMP},
	{ L"/loc",			        UserCmd_loc},
	{ L"/loc$",			        UserCmd_loc$},
	{ L"/store",		        UserCmd_Store},
	{ L"/unstore",		        UserCmd_Ustore},
	{ L"/sinfo",	            UserCmd_Istore},
	{ L"/bank",                 UserCmd_Bank},
	{ L"/wbank",                UserCmd_wBank},
	{ L"/p",                    UserCmd_Ping},
	{ L"/ping",                 UserCmd_Ping},
	{ L"/bequip",               UserCmd_eBuild},
	{ L"/bwep",                 UserCmd_wBuild},
	{ L"/bammo",                UserCmd_aBuild},
	{ L"/kills",                UserCmd_Kills},
	{ L"/kills$",               UserCmd_Kills$},
	{ L"/bountyhunt",		    UserCmd_BountyHunt},
	{ L"/bountyhuntid",         UserCmd_BountyHuntId},
	{ L"/template",              UserCmd_Restart},
	{ L"/list",                 UserCmd_List},
	{ L"/rinfo",                UserCmd_RestartInfo},
	{ L"/pods",                 UserCmd_Pods},
	{ L"/help",					UserCmd_Help},
	{ L"/?",					UserCmd_Help},
};

bool UserCmd_Process(uint iClientID, wstring wscCmd)
{
	wstring wscCmdLower = ToLower(wscCmd);

	for(uint i = 0; (i < sizeof(UserCmds)/sizeof(USERCMD)); i++)
	{
		if(wscCmdLower.find(ToLower(UserCmds[i].wszCmd)) == 0)
		{
			wstring wscParam = L"";
			if(wscCmd.length() > wcslen(UserCmds[i].wszCmd))
			{
				if(wscCmd[wcslen(UserCmds[i].wszCmd)] != ' ')
					continue;
				wscParam = wscCmd.substr(wcslen(UserCmds[i].wszCmd) + 1);
			}
			UserCmds[i].proc(iClientID, wscParam);
			return true;
		}
	}

	return false;
}

int BOUNTY_HUNT::sBhCounter = 0;
bool BOUNTY_HUNT::operator ==(struct BOUNTY_HUNT const & sT)
{
	if(iCounter  == sT.iCounter)
	{
		return true;
	}
	return false;
}

void BhTimeOutCheck()
{
	mstime nowtime = timeInMS();
	foreach(lstBountyHunt, BOUNTY_HUNT, it)
	{
		if(it->msEnd < nowtime)
		{
			HkAddCash(it->wscTarget, it->uiCredits);
			PrintUniverseText(L"BOUNTYHUNT: %s was not hunted down and earned %u credits.",it->wscTarget.c_str(),it->uiCredits);
			lstBountyHunt.remove(*it);
			BhTimeOutCheck();
			break;
			//funzt nicht:it = lstBountyHunt.begin();
		}
	}
}

void BhKillCheck(uint uiClientID, uint uiKillerID)
{
	mstime nowtime = timeInMS();
	foreach(lstBountyHunt, BOUNTY_HUNT, it)
	{
		if(it->uiTargetID == uiClientID)
		{
			//NPC oder Admin oder selfkill oder unknown Kill:
			if(uiKillerID == 0 || uiClientID == uiKillerID)
			{
				PrintUniverseText(L"BOUNTYHUNT: the hunt for %s still goes on.",it->wscTarget.c_str());
			}
			else
			{
				wstring wscWinnerCharname;
				wscWinnerCharname = (wchar_t*)Players.GetActiveCharacterName(uiKillerID);
				if(wscWinnerCharname.size() > 0)
				{
					HkAddCash(wscWinnerCharname, it->uiCredits);
					PrintUniverseText(L"BOUNTYHUNT: %s has killed %s and earned %u credits.",wscWinnerCharname.c_str(),it->wscTarget.c_str(),it->uiCredits);
				}
				else
				{//nur zur Sicherheit!
					HkAddCash(it->wscInitiator, it->uiCredits);
				}

			lstBountyHunt.remove(*it);
			//funzt nicht: it = lstBountyHunt.begin();
			BhKillCheck(uiClientID, uiKillerID);
			break;
			}
		}
	}
}

void CancelBh(uint uiClientID)
{
	mstime nowtime = timeInMS();
	foreach(lstBountyHunt, BOUNTY_HUNT, it){
		if(it->uiTargetID == uiClientID)
		{
			if( it->uiInitiatorID != 0)
			{
				HkAddCash(it->wscInitiator, it->uiCredits);
			}
			PrintUniverseText(L"BOUNTYHUNT: Dissatisfied, %s withdraws his offer.",it->wscInitiator.c_str());
			lstBountyHunt.remove(*it);
			CancelBh(uiClientID);
			break;
			//funzt nicht it = lstBountyHunt.begin();
		}
	}
}
