// 说明： 本程序演示了如何用程序查询方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"
#pragma warning(disable:4996)
int InputRange;
int SelectInputRange(void);

#define AD_DATA_LEN 1024*16 // 要读取和处理的AD数据长度（点或字）
USHORT ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	LONG DevID, LinkType;
	LONG DeviceLgcID, DevicePhysID;



	DeviceLgcID = 0;
SlectLT:
	printf("选择设备创建方式:0:逻辑 1:物理 ");
	scanf_s("%d", &LinkType);
	if (LinkType<0 || LinkType>1)
	{
		goto SlectLT;
	}

SlectID:
	printf("输入ID:");
	scanf_s("%d", &DevID);
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
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	ACTS1000_GetDeviceCurrentID(hDevice, &DeviceLgcID, &DevicePhysID);
	printf("当前设备逻辑ID:%d 物理ID:%d\n", DeviceLgcID, DevicePhysID);

	printf("输入新物理ID:");
	scanf_s("%d", &DevicePhysID);
	ACTS1000_SetDevicePhysID(hDevice, DevicePhysID);
	ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
	return 0;
}