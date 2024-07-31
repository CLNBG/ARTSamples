
//#include <ansi_c.h>	 
#include "windows.h"
#include <ACTS1000.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 




int main (int argc, char *argv[])

{

	HANDLE hDevice;
	LONG DevID, LinkType;
	LONG DeviceLgcID, DevicePhysID;

	
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   




	DeviceLgcID = 0;
SlectLT:
	printf("选择设备创建方式:0:逻辑 1:物理 ");
	scanf("%d", &LinkType);
	if (LinkType<0 || LinkType>1)
	{
		goto SlectLT;
	}

SlectID:
	printf("输入ID:");
	scanf("%d", &DevID);
	if (DevID <0)
	{
		goto SlectID;
	}
	switch (LinkType)
	{
	case 0:
		hDevice = ACTS1000_CreateDevice(DevID); // 创建设备对象
		break;
	case 1:
		hDevice = ACTS1000_CreateDeviceEx(DevID); // 创建设备对象
		break;
	}

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("ACTS1000_CreateDevice fail");
		getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	ACTS1000_GetDeviceCurrentID(hDevice, &DeviceLgcID, &DevicePhysID);
	printf("当前设备逻辑ID:%d 物理ID:%d\n", DeviceLgcID, DevicePhysID);

	printf("输入新物理ID:");
	scanf("%d", &DevicePhysID);
	ACTS1000_SetDevicePhysID(hDevice, DevicePhysID);
	ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
	
	return 0;
}

