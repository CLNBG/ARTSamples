// ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"
#pragma warning(disable:4996)
int InputRange;
int SelectInputRange(void);

#define AD_DATA_LEN 1024*16 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
USHORT ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	LONG DevID, LinkType;
	LONG DeviceLgcID, DevicePhysID;



	DeviceLgcID = 0;
SlectLT:
	printf("ѡ���豸������ʽ:0:�߼� 1:���� ");
	scanf_s("%d", &LinkType);
	if (LinkType<0 || LinkType>1)
	{
		goto SlectLT;
	}

SlectID:
	printf("����ID:");
	scanf_s("%d", &DevID);
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
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	ACTS1000_GetDeviceCurrentID(hDevice, &DeviceLgcID, &DevicePhysID);
	printf("��ǰ�豸�߼�ID:%d ����ID:%d\n", DeviceLgcID, DevicePhysID);

	printf("����������ID:");
	scanf_s("%d", &DevicePhysID);
	ACTS1000_SetDevicePhysID(hDevice, DevicePhysID);
	ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
	return 0;
}