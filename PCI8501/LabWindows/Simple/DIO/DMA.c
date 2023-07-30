#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "PCI8501.h"   

static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void);




int InputRange;
int SelectInputRange(void);



int main(int argc, char* argv[])
{
	BYTE bDISts[8], bDOSts[8];

	HANDLE hDevice;
	int DeviceLgcID = 0; 
	char Key=0;
		
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");  


 

	hDevice = PCI8501_CreateDevice(DeviceLgcID);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Create Device Error...\n");
		getch();
		return 0;
	}

	memset(&bDOSts, 0x00, sizeof(bDOSts)); // 强制初始化为0，以确保各个参数处于确定状态(强烈建议)
	bDOSts[0] = 0;
	bDOSts[1] = 1;
	bDOSts[2] = 0;
	bDOSts[3] = 1;
	bDOSts[4] = 0;
	bDOSts[5] = 1;
	bDOSts[6] = 0;
	bDOSts[7] = 1;

Repeat:
	printf("Press any key to set DO\n\n");
	
	PCI8501_SetDeviceDO(hDevice, bDOSts); // 开关量输出

	if (bDOSts[0])  printf("DO0 = On\n"); else printf("DO0 = Off\n");
	if (bDOSts[1])  printf("DO1 = On\n"); else printf("DO1 = Off\n");
	if (bDOSts[2])  printf("DO2 = On\n"); else printf("DO2 = Off\n");
	if (bDOSts[3])  printf("DO3 = On\n"); else printf("DO3 = Off\n");
	if (bDOSts[4])  printf("DO4 = On\n"); else printf("DO4 = Off\n");
	if (bDOSts[5])  printf("DO5 = On\n"); else printf("DO5 = Off\n");
	if (bDOSts[6])  printf("DO6 = On\n"); else printf("DO6 = Off\n");
	if (bDOSts[7])  printf("DO7 = On\n"); else printf("DO7 = Off\n");

	printf("\n");	
	printf("Press any key to get DI\n\n");

	if(!PCI8501_GetDeviceDI(hDevice, bDISts)) // 开关量输入
	{
		printf("PXI8501_GetDeviceDI...\n");
		getch();
		return 0;
	}
	
	if (bDISts[0])  printf("DI0 = On\n"); else printf("DI0 = Off\n");
	if (bDISts[1])  printf("DI1 = On\n"); else printf("DI1 = Off\n");
	if (bDISts[2])  printf("DI2 = On\n"); else printf("DI2 = Off\n");
	if (bDISts[3])  printf("DI3 = On\n"); else printf("DI3 = Off\n");
	if (bDISts[4])  printf("DI4 = On\n"); else printf("DI4 = Off\n");
	if (bDISts[5])  printf("DI5 = On\n"); else printf("DI5 = Off\n");
	if (bDISts[6])  printf("DI6 = On\n"); else printf("DI6 = Off\n");
	if (bDISts[7])  printf("DI7 = On\n"); else printf("DI7 = Off\n");

	printf("\n");
	Key;
	printf("Press ESC to quit\n");
	Key = getch();
	if(Key != 27) goto Repeat;

	PCI8501_ReleaseDevice(hDevice);
	return 0;
}

