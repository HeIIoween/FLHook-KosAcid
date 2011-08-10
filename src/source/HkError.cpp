#include "hook.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct HK_ERROR_INFO
{
	HK_ERROR hkErr;
	wchar_t *wszText;
};

HK_ERROR_INFO hkErrors[] = 
{
	{ HKE_OK,							L"Ok" },
	{ HKE_PLAYER_NOT_LOGGED_IN,			L"Player not logged in" },
	{ HKE_CHAR_DOES_NOT_EXIST,			L"Char does not exist" },
	{ HKE_COULD_NOT_DECODE_CHARFILE,	L"Could not decode charfile" },
	{ HKE_COULD_NOT_ENCODE_CHARFILE,	L"Could not encode charfile" },
	{ HKE_INVALID_BASENAME,				L"Invalid basename" },
	{ HKE_UNKNOWN_ERROR,				L"Unknown error" },
	{ HKE_INVALID_CLIENT_ID,			L"Invalid client id" },
	{ HKE_INVALID_GROUP_ID,				L"Invalid group id" },
	{ HKE_INVALID_ID_STRING,			L"Invalid id string" },
	{ HKE_INVALID_SYSTEM,				L"Invalid system" },
	{ HKE_PLAYER_NOT_IN_SPACE,			L"Player not in space" },
	{ HKE_PLAYER_NOT_DOCKED,			L"Player not docked" },
	{ HKE_PLAYER_NO_ADMIN,				L"Player is no admin" },
	{ HKE_WRONG_XML_SYNTAX,				L"Wrong XML syntax" },
	{ HKE_INVALID_GOOD,					L"Invalid good" },
	{ HKE_NO_CHAR_SELECTED,				L"Player has no char selected" },
	{ HKE_CHARNAME_ALREADY_EXISTS,		L"Charname already exists" },
	{ HKE_CHARNAME_TOO_LONG,			L"Charname is too long" },
	{ HKE_CHARNAME_TOO_SHORT,			L"Charname is too short" },
	{ HKE_AMBIGUOUS_SHORTCUT,			L"Ambiguous shortcut" },
	{ HKE_NO_MATCHING_PLAYER,			L"No matching player" },
	{ HKE_INVALID_SHORTCUT_STRING,		L"Invalid shortcut string" },
	{ HKE_MPNEWCHARACTERFILE_NOT_FOUND_OR_INVALID,	L"mpnewcharacter.fl not found or invalid" },
	{ HKE_INVALID_REP_GROUP,			L"Invalid reputation group" },
	{ HKE_COULD_NOT_GET_PATH,			L"Could not get the path to a file" },
	{ HKE_NO_TASKKILL,					L"taskkill.exe does not exist" },
	{ HKE_SYSTEM_NO_MATCH,				L"Your target's system and the player's system are not the same" },
	{ HKE_OBJECT_NO_DOCK,				L"Your target is not dockable" },
	{ HKE_CARGO_WONT_FIT,				L"Cargo exceeds ship's capacity" },
	{ HKE_NAME_NOT_MATCH_RESTRICTION,	L"The name of the character does not fit your admin restrictions" },
};

wstring HkErrGetText(HK_ERROR hkErr)
{
	for(uint i = 0; (i < (sizeof(hkErrors)/sizeof(HK_ERROR_INFO))); i++)
	{
		if(hkErrors[i].hkErr == hkErr)
			return hkErrors[i].wszText;
	}

	return L"No error text available";
}