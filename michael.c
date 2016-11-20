#include<stdio.h>
#include<string.h>
#include"windivert.h"


#define MAXBUF 10000
int main()
{
	HANDLE handle;
	char packet[MAXBUF];
	WINDIVERT_ADDRESS addr;
	UINT packetLen;
	
	handle = WinDivertOpen("tcp",0 ,0 ,0);
	
	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Fail to WinDivertOpen \n");
	}

	while (TRUE)
	{
		if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen))
		{
			int err= GetLastError();
			printf("%d",err);
			printf("Fail to receive \n");
			continue;
		}

		if(addr.Direction == WINDIVERT_DIRECTION_OUTBOUND)
		for (int i = 0; i<packetLen-3 ; i++)
		{
			if (packet[i] == 'g' && packet[i + 1] == 'z' && packet[i + 2] == 'i' && packet[i + 3] == 'p')
			{
				packet[i] = ' ';
				packet[i+1] = ' ';
				packet[i+2] = ' ';
				packet[i+3] = ' ';
			}
		}
		if(addr.Direction == WINDIVERT_DIRECTION_INBOUND)
		for (int i = 0; i<packetLen - 6; i++)
		{
			if (packet[i] == 'M' && packet[i + 1] == 'i' && packet[i + 2] == 'c'&& packet[i + 3] == 'h' && packet[i+4] == 'a' && packet[i+5] == 'e' && packet[i+6] == 'l')
			{
				packet[i] = 'G';
				packet[i + 1] = 'i';
				packet[i + 2] = 'l';
				packet[i + 3] = 'b';
				packet[i + 4] = 'e';
				packet[i + 5] = 'r';
				packet[i + 6] = 't';
			}
		}
		
		WinDivertHelperCalcChecksums(packet,packetLen,0);
		if (!WinDivertSend(handle, packet, packetLen, &addr, NULL))
		{
			printf("Fail to WinDivertSend");
			continue;
		}
		continue;
	}
	
}