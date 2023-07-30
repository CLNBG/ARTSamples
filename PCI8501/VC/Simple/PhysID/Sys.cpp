// Get Set Physical Index 演示控制流程

#include "stdafx.h"
#include "windows.h"
#include "conio.h"

#include "PCI8501.h"

int main(int argc, char* argv[])
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	LONG nID = 0;
	LONG nCreateType = 0;
	printf("请输入创建设备方式,0:逻辑 1:物理  ");
	scanf_s("%d", &nCreateType);
	nCreateType &=1;


	printf("请输入ID,[0~255]  ");
	scanf_s("%d", &nID);
	nID &=0xFF;

	// 第一步 创建设备对象
	if (nCreateType == 0)
	{
		hDevice = PCI8501_CreateDevice(nID);
	}
	else
	{
		hDevice = PCI8501_CreateDeviceEx(nID);

	}
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Create Device Error\n");
		_getch();
		return 0;
	}


	LONG nDeviceLgcID, nDevicePhysID;

	if (!PCI8501_GetDeviceCurrentID(hDevice, &nDeviceLgcID, &nDevicePhysID))
	{
		printf("PCI8501_GetDeviceCurrentID Error\n");
		_getch();
		goto Exit;
	}

	printf("当前设备逻辑ID=%d,物理ID=%d\n", nDeviceLgcID, nDevicePhysID);
	
	printf("请输入新的物理ID[0, 255]:  ");
	scanf_s("%d", &nDevicePhysID);
	nDevicePhysID &= 0xFF;


	if(!PCI8501_SetDevicePhysID(hDevice, nDevicePhysID))
	{
		printf("PCI8501_SetDevicePhysID Error\n");
		_getch();
		goto Exit;
	}
	nDevicePhysID = 0;

	if (!PCI8501_GetDeviceCurrentID(hDevice, &nDeviceLgcID, &nDevicePhysID))
	{
		printf("PCI8501_GetDeviceCurrentID Error\n");
		_getch();
		goto Exit;
	}
	else
		printf("获取的物理ID=%d\n", nDevicePhysID);

Exit:
	// 第四步 释放设备对象
	if(!PCI8501_ReleaseDevice(hDevice))
	{
		printf("PCI8501_ReleaseDevice Error\n");
		_getch();
	}

	printf("按任意键退出\n");
	_getch();
	return 0;
}

