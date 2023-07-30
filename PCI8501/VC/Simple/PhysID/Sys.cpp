// Get Set Physical Index ��ʾ��������

#include "stdafx.h"
#include "windows.h"
#include "conio.h"

#include "PCI8501.h"

int main(int argc, char* argv[])
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;
	LONG nID = 0;
	LONG nCreateType = 0;
	printf("�����봴���豸��ʽ,0:�߼� 1:����  ");
	scanf_s("%d", &nCreateType);
	nCreateType &=1;


	printf("������ID,[0~255]  ");
	scanf_s("%d", &nID);
	nID &=0xFF;

	// ��һ�� �����豸����
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

	printf("��ǰ�豸�߼�ID=%d,����ID=%d\n", nDeviceLgcID, nDevicePhysID);
	
	printf("�������µ�����ID[0, 255]:  ");
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
		printf("��ȡ������ID=%d\n", nDevicePhysID);

Exit:
	// ���Ĳ� �ͷ��豸����
	if(!PCI8501_ReleaseDevice(hDevice))
	{
		printf("PCI8501_ReleaseDevice Error\n");
		_getch();
	}

	printf("��������˳�\n");
	_getch();
	return 0;
}

