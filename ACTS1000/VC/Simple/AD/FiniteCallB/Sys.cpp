// ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"

#define AD_DATA_LEN 512*1024 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
USHORT ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)
ULONGLONG nReadCnt = 0;
LONG ADCallBackFunc(HANDLE hDevice, ULONG nSamplesPerChan, ULONG nChannelCount, void *callbackData)
{

	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	if (nSamplesPerChan>(AD_DATA_LEN/nChannelCount))
	{
		nSamplesPerChan = AD_DATA_LEN/nChannelCount;
	}

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	if(!ACTS1000_ReadDeviceAD(hDevice, ADBuffer, nSamplesPerChan*nChannelCount, &nRetSizeWords, &nAvailSampsPoints,0.1))
	{
		printf("ACTS1000_ReadDeviceAD error...\n");
		_getch();
		return 2;
	}
	if (nRetSizeWords!=(nSamplesPerChan*nChannelCount))
	{
		printf("nRetSizeWords error...\n");
		_getch();
		return 3;
	}
	nReadCnt+=nSamplesPerChan;
	printf("ADBuff[0]=0x%X\n", ADBuffer[0]);
	return 0;

}
int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;

	ACTS1000_PARA_AD ADPara; // Ӳ������
	ULONGLONG nReadSizeWords;
	ULONG InputRange = ACTS1000_INPUT_N1000_P1000mV;

	int nADChannel = 0;
	ACTS1000_AD_MAIN_INFO ADMainInfo;
	WORD wMaxLSB = 0x3FFF;
	float fLsbCount = 16384.0;

	ULONG nSamplesPerChan = 131072;

	DeviceLgcID = 0;
	printf("�������߼�ID:");
	scanf_s("%d", &DeviceLgcID);
	hDevice = ACTS1000_CreateDevice(DeviceLgcID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	ACTS1000_GetMainInfo(hDevice,&ADMainInfo);			// DDR2�ĳ���(��λ��MB)

	switch (ADMainInfo.nDeviceType>>16)
	{
	case 0x2012:
		printf("PXIE%04X\n",ADMainInfo.nDeviceType&0xFFFF);
		break;
	case 0x2111:
		printf("PCIE%04X\n",ADMainInfo.nDeviceType&0xFFFF);
		break;
	default:
		printf("ACTS1000-%04X\n",ADMainInfo.nDeviceType);
	}

	wMaxLSB = (WORD)(ADMainInfo.nSampCodeCount - 1);
	fLsbCount = (float)ADMainInfo.nSampCodeCount;

	ULONG nRevisionID = 0;
	if(!ACTS1000_GetBusInfo(hDevice, NULL, NULL, NULL, &nRevisionID))
	{
		printf("ACTS1000_GetBusInfo error...\n");
	}

	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)

	// Ԥ��Ӳ������
	ADPara.SampleMode			= ACTS1000_SAMPMODE_FINITE; // ���޵����
	ADPara.FreqDivision			= 1;		
	for (int nCH=0; nCH<ADMainInfo.nChannelCount; nCH++)
	{
		ADPara.bChannelArray[nCH]		= TRUE; // ����ͨ��0����
		ADPara.InputRange[nCH]		= ACTS1000_INPUT_N1000_P1000mV;		// ����ֻ֧�֡�1000mV
		ADPara.CouplingType[nCH]		= ACTS1000_COUPLING_DC;	// ֱ�����
	}	
	ADPara.M_Length				= 0;					// M����
	ADPara.N_Length				= 536870912;			// N����
	ADPara.PFISel				= ACTS1000_PFISEL_TRIG_IN;
	ADPara.TriggerMode			= ACTS1000_TRIGMODE_POST; // Ӳ���м䴥��(����Ԥ�������󴥷�����)
	ADPara.TriggerSource		= ACTS1000_TRIGMODE_SOFT;	// �������
	ADPara.TriggerDir			= ACTS1000_TRIGDIR_NEGATIVE; // �½��ش���
	ADPara.TrigLevelVolt		= 0;
	ADPara.ReferenceClock			= ACTS1000_RECLK_ONBOARD;	// ʹ���ڲ�ʱ��
	ADPara.TimeBaseClock			= ACTS1000_TBCLK_IN;	
	ADPara.bMasterEn			= 0;
	ADPara.TrigCount			= 1; // Ϊ0ʱ��ʾ���޴���
	ADPara.SyncTrigSignal		= ACTS1000_STS_TRIGGER0;
	ADPara.bClkOutEn			= 0;
	ADPara.ClkOutSel			= ACTS1000_CLKOUT_REFERENCE;
	ADPara.bTrigOutEn			= 0;
	ADPara.TrigOutPolarity		= ACTS1000_TOP_POSITIVE;
	ADPara.TrigOutWidth			= 50;
	ADPara.bSaveFile			= 0;
	_stprintf_s(ADPara.chFileName, 256, _T("%s"), L"I:\\3.pci");

	if(!ACTS1000_InitDeviceAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("InitDeviceAD error...\n");
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	int ChannelCount = 0;	
	for(nADChannel=0; nADChannel<ADMainInfo.nChannelCount; nADChannel++)
	{
		if(ADPara.bChannelArray[nADChannel] == TRUE)
		{
			ChannelCount++;
		}
	}

	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_MIDL)
	{
		nReadSizeWords = (ADPara.M_Length + ADPara.N_Length)*ChannelCount; 
	}
	else
	{
		nReadSizeWords = ADPara.N_Length;
		nReadSizeWords = nReadSizeWords*ChannelCount; 
	}
	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_DELAY)
	{
		if (ADPara.TrigCount==0)
		{
			nReadSizeWords = 0xFFFFFFFFFFFFFFFF;
		}
		else
		{
			nReadSizeWords = nReadSizeWords*ADPara.TrigCount;
		}
	}


	nReadCnt = 0;
	if (!ACTS1000_RegisterEveryNSamplesEvent(hDevice, nSamplesPerChan, 0, &ADCallBackFunc, NULL))
	{
		ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
		printf("ACTS1000_GetDMAMemorySize error...\n");
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	if(!ACTS1000_StartDeviceAD(hDevice)) // �����豸
	{
		printf("StartDeviceAD Error...\n");
		_getch();
	}
	if(!ACTS1000_SetDeviceTrigAD(hDevice)) // ����AD
	{
		printf("SetDeviceTrigAD Error...\n");
		_getch();
	}
	printf("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");

	while ( !_kbhit() )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	{			
		Sleep(1);
		if (nReadCnt*ChannelCount>=nReadSizeWords)
		{
			printf("���޵����(%I64d�ֽ�)��ɰ�������˳�\n", nReadCnt*ChannelCount*2);
			break;
		}
	}


	ACTS1000_StopDeviceAD(hDevice); // ֹͣAD
	ACTS1000_ReleaseDeviceAD(hDevice); // �ͷ�AD
	ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
	_getch();
	return 0;
}

