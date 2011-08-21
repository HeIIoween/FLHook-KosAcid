#include "hook.h"
#pragma warning(disable:4996)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HkHandleCheater(uint iClientID, bool bBan, wstring wscReason, ...)
{
	wchar_t wszBuf[1024*8] = L"";
	va_list marker;
	va_start(marker, wscReason);

	_vsnwprintf(wszBuf, (sizeof(wszBuf) / 2) - 1, wscReason.c_str(), marker);

	if(!(wchar_t*)Players.GetActiveCharacterName(iClientID))
		return;

	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	HkAddCheaterLog(wscCharname, wszBuf);

	wchar_t wszBuf2[256];
	swprintf(wszBuf2, L"Possible cheating detected: %s", wscCharname.c_str());
	if(wscReason[0] != '#')
		HkMsgU(wszBuf2);
	if(bBan)
		HkBan(ARG_CLIENTID(iClientID), true);
	if(wscReason[0] != '#')
		HkKick(ARG_CLIENTID(iClientID));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool HkAddCheaterLog(wstring wscCharname, wstring wscReason)
{
	FILE *f = fopen((scAcctPath + "flhook_cheaters.log").c_str(), "at");
	if(!f)
		return false;

	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);

	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	fprintf(f, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d Possible cheating detected (%s) by %s(%s)(%s)\n", stNow->tm_mon + 1, stNow->tm_mday, stNow->tm_year + 1900, stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscReason).c_str(), wstos(wscCharname).c_str(), wstos(wscAccountDir).c_str(), wstos(HkGetAccountID(acc)).c_str());
	fclose(f);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool HkAddKickLog(uint iClientID, wstring wscReason, ...)
{
	wchar_t wszBuf[1024*8] = L"";
	va_list marker;
	va_start(marker, wscReason);

	_vsnwprintf(wszBuf, (sizeof(wszBuf) / 2) - 1, wscReason.c_str(), marker);

	FILE *f = fopen((scAcctPath + "flhook_kicks.log").c_str(), "at");
	if(!f)
		return false;

	const wchar_t *wszCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	if(!wszCharname)
		wszCharname = L"";

	CAccount *acc = Players.FindAccountFromClientID(iClientID);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);

	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	fprintf(f, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d Kick (%s): %s(%s)(%s)\n", stNow->tm_mon + 1, stNow->tm_mday, stNow->tm_year + 1900, stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wszBuf).c_str(), wstos(wszCharname).c_str(), wstos(wscAccountDir).c_str(), wstos(HkGetAccountID(acc)).c_str());
	fclose(f);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ??? what have i done? 8[

bool HkAddConnectLog(uint iClientID)
{
	FILE *f = fopen((scAcctPath + "flhook_connects.log").c_str(), "at");
	if(!f)
		return false;

	const wchar_t *wszCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	if(!wszCharname)
		wszCharname = L"";

	CAccount *acc = Players.FindAccountFromClientID(iClientID);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);

	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
//	fprintf(f, "%.2d/%.2d/%.4d %.2d:%.2d:%.2d Kick (%s): %s(%s)(%s)\n", stNow->tm_mon + 1, stNow->tm_mday, stNow->tm_year + 1900, stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscReason).c_str(), wstos(wszCharname).c_str(), wstos(wscAccountDir).c_str(), wstos(HkGetAccountID(acc)).c_str());
	fclose(f);
	return true;
}
//Anticheat
bool HkAddChatLogSpeed(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "speedhack" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d[%s]\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscAccountDir).c_str());
	fclose(f);
	return true;
}

bool HkAddChatLogProc(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "process" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d[%s]\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscAccountDir).c_str());
	fclose(f);
	return true;
}

bool HkAddChatLogATProc(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "atprocess" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d[%s]\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscAccountDir).c_str());
	fclose(f);
	return true;
}

bool HkAddChatLogCRC(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "crccheats" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d[%s]\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscAccountDir).c_str());
	fclose(f);
	return true;
}

bool HkAddChatLogSystem(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "chat_" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
    wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d[%s]%s\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec, wstos(wscAccountDir).c_str(), wstos(HkGetPlayerSystem(iClientID)).c_str());
	fclose(f);
	return true;
}

bool HkAddChatLogGroup(uint iClientID, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "group_" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
    wstring wscCharname = (wchar_t*)Players.GetActiveCharacterName(iClientID);
	CAccount *acc = HkGetAccountByCharname(wscCharname);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s]%.2d:%.2d:%.2d\n", wstos(wscCharname).c_str(), wstos(wscMessage).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec);
	fclose(f);
	return true;
}

bool HkAddChatLogPM(wstring ClientID, wstring ClientIDto, wstring wscMessage)
{
	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "pm_" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	CAccount *acc = HkGetAccountByCharname(ClientID);
	wstring wscAccountDir;
	HkGetAccountDirName(acc, wscAccountDir);
	fprintf(f, "%s[%s] to %s%.2d:%.2d:%.2d\n", wstos(ClientID).c_str(), wstos(wscMessage).c_str(), wstos(ClientIDto).c_str(), stNow->tm_hour, stNow->tm_min, stNow->tm_sec);
	fclose(f);
	return true;
}

bool HkAddChatLogKill(uint iClientIDVictim, uint iClientIDKiller)
{
	if(!iClientIDKiller)
		return true;
	wstring wscVictim = (wchar_t*)Players.GetActiveCharacterName(iClientIDVictim);
	wstring wscKiller = (wchar_t*)Players.GetActiveCharacterName(iClientIDKiller);
	wstring wscSystem = HkGetPlayerSystem(iClientIDVictim);
	wstring wscDeathship = HkGetWStringFromIDS(Archetype::GetShip(Players[iClientIDVictim].iShipArchID)->iIDSName);
	wstring wscKillship = HkGetWStringFromIDS(Archetype::GetShip(Players[iClientIDKiller].iShipArchID)->iIDSName);
	
	for(int i=0; i<wscDeathship.length(); i++)
	{
       if(wscDeathship[i] == ' ') wscDeathship.erase(i,1);
	}

	for(int i=0; i<wscKillship.length(); i++)
	{
       if(wscKillship[i] == ' ') wscKillship.erase(i,1);
	}

	time_t tNow = time(0);
	struct tm *stNow = localtime(&tNow);
	FILE *f = fopen((scAcctPath + "playerkills" + itos(stNow->tm_mon + 1) + "_" + itos(stNow->tm_mday) + "_" + itos(stNow->tm_year + 1900) + ".log").c_str(), "at");
	if(!f)
	return false;
	wchar_t killlog[256];
	swprintf(killlog,L"kill System:%s Killer:%s Killship:%s Victim:%s Vicship:%s",wscSystem.c_str(), wscKiller.c_str(), wscKillship.c_str(), wscVictim.c_str(), wscDeathship.c_str());
	ProcessEvent(killlog);
	fprintf(f, "%.2d:%.2d:%.2d %s\n", stNow->tm_hour, stNow->tm_min, stNow->tm_sec,wstos(killlog).c_str());
	fclose(f);
	return true;
}

