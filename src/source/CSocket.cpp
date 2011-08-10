#include "CSocket.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSocket::DoPrint(wstring wscText)
{
	for(uint i = 0; (i < wscText.length()); i++)
	{
		if(wscText[i] == '\n')
		{
			wscText.replace(i, 1, L"\r\n");
			i++;
		}
	}

	int iSndBuf = 300000; // fix: set send-buffer to this size (bytes) 
    int size = sizeof(int); 
    setsockopt(this->s, SOL_SOCKET, SO_SNDBUF, (const char*)&iSndBuf, size);

	if(bEncrypted)
	{
		int iLen;
		char *data;
		const char* tempData;
		if(bUnicode)
		{
			uint iRem = wscText.length() % 4;
			if(iRem) //Data to be encrypted is not a multiple of 8 bytes, add 0x00s to compensate
			{
				iRem = 4 - iRem;
				wscText.resize(wscText.length()+iRem, L'\x00');
			}
			tempData = (const char*)wscText.data();
			iLen = wscText.length()*2;
			data = (char*)malloc(iLen);
			memcpy(data, tempData, iLen);
		}
		else
		{
			uint iRem = wscText.length() % 8;
			if(iRem)
			{
				iRem = 8 - iRem;
				string scText = wstos(wscText);
				scText.resize(scText.length()+iRem,'\x00');
				tempData = scText.data();
				iLen = scText.length();
				data = (char*)malloc(iLen);
				memcpy(data, tempData, iLen);
			}
			else
			{
				string scText = wstos(wscText);
				tempData = scText.data();
				iLen = wscText.length();
				data = (char*)malloc(iLen);
				memcpy(data, tempData, iLen);
			}
		}

		SwapBytes(data, iLen);
		if(Blowfish_Encrypt(bfc, data, iLen))
		{
			SwapBytes(data, iLen);
			send(this->s, data, iLen, 0);
			free(data);
		}
	}
	else
	{
		if(bUnicode)
			send(this->s, (const char*)wscText.c_str(), (int)wscText.length()*2, 0);
		else
			send(this->s, wstos(wscText).c_str(), (int)wscText.length(), 0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

wstring CSocket::GetAdminName()
{
	return L"Socket connection";
}
