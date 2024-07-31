
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
	printf("ѡ���豸������ʽ:0:�߼� 1:���� ");
	scanf("%d", &LinkType);
	if (LinkType<0 || LinkType>1)
	{
		goto SlectLT;
	}

SlectID:
	printf("����ID:");
	scanf("%d", &DevID);
	if (DevID <0)
	{
		goto SlectID;
	}
	switch (LinkType)
	{
	case 0:
		hDevice = ACTS1000_CreateDevice(DevID); // �����豸����
		break;
	case 1:
		hDevice = ACTS1000_CreateDeviceEx(DevID); // �����豸����
		break;
	}

	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("ACTS1000_CreateDevice fail");
		getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	ACTS1000_GetDeviceCurrentID(hDevice, &DeviceLgcID, &DevicePhysID);
	printf("��ǰ�豸�߼�ID:%d ����ID:%d\n", DeviceLgcID, DevicePhysID);

	printf("����������ID:");
	scanf("%d", &DevicePhysID);
	ACTS1000_SetDevicePhysID(hDevice, DevicePhysID);
	ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
	
	return 0;
}

