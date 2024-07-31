
//#include <ansi_c.h>	 
#include "windows.h"
#include <ACTS1000.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 




#define AD_DATA_LEN 256*1024 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
USHORT ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)
int main (int argc, char *argv[])

{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;

	ACTS1000_PARA_AD ADPara; // Ӳ������
	ACTS1000_STATUS_AD ADStatus;
	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	ULONGLONG nReadSizeRemain;
	ULONGLONG nReadSizeWords;
	ULONG nRead;
	ULONG InputRange = ACTS1000_INPUT_N1000_P1000mV;

	int nADChannel = 0;
	WORD ADData;
	float fVolt;
	ACTS1000_AD_MAIN_INFO ADMainInfo;
	WORD wMaxLSB = 0x3FFF;
	float fLsbCount = 16384.0;
	
	int nCH=0;
	
	int ChannelCount = 0;
	
	LONG ulTime = GetTickCount();
	double dResult;
	__int64 i64Cnt=0;
	LONG ulTmp=0;
	LONG ulSecCnt=0;
	
	ULONGLONG ulReadCount = 0;			   
	ULONG nTChannel = 0;
	int Index=0;
	
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   


	DeviceLgcID = 0;
	printf("�������߼�ID:");
	scanf("%d", &DeviceLgcID);
	hDevice = ACTS1000_CreateDevice(DeviceLgcID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		getch();
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

	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)

	// Ԥ��Ӳ������
	ADPara.SampleMode			= ACTS1000_SAMPMODE_FINITE; // ���޵����
	ADPara.FreqDivision			= 1;		
	for ( nCH=0; nCH<ADMainInfo.nChannelCount; nCH++)
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

	if(!ACTS1000_InitDeviceAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("InitDeviceAD error...\n");
		getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	ChannelCount = 0;	
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



	ulTime = GetTickCount();
	 dResult = 1.0;
	 i64Cnt=0;
	 ulTmp=0;
	 ulSecCnt=-1;

	if(!ACTS1000_StartDeviceAD(hDevice)) // �����豸
	{
		printf("StartDeviceAD Error...\n");
		getch();
	}
	if(!ACTS1000_SetDeviceTrigAD(hDevice)) // ����AD
	{
		printf("SetDeviceTrigAD Error...\n");
		getch();
	}
	ulTime = GetTickCount();
	printf("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");

	if (ADPara.bSaveFile)	// �Զ�����
	{
		while(TRUE) 
		{
			if(!ACTS1000_GetDevStatusAD(hDevice, &ADStatus))
			{
				printf("GetDevStatusAD Error\n");
				goto ExitRead;
			}
			if(ADStatus.bComplete) // �Զ����̵������޵������Ƿ����
			{
				printf("���޲�����ɰ�������˳�\n");
				goto ExitRead;
			}
			else
			{
				if(kbhit())
					goto ExitRead; // ����û����������˳�
				Sleep(1);

				ulTmp = GetTickCount()-ulTime;
				if (ulTmp/1000>ulSecCnt)
				{
					ulSecCnt = ulTmp/1000;
					dResult = ADStatus.lSavePoints*1.00/((ulTmp+1)/1000.0);
					dResult=dResult/1000000.0;
					printf("%dS ",ulSecCnt);
					printf("%.3fM��/��\n",dResult);
				}
			}
		}
	}
	else
	{
		nReadSizeRemain = nReadSizeWords;
		while (nReadSizeRemain>0)
		{
			if (nReadSizeRemain>=AD_DATA_LEN)
			{
				nRead = AD_DATA_LEN;
			}
			else
			{
				nRead = nReadSizeRemain;
			}
NextRead:

			if(kbhit()) goto ExitRead;
			if(!ACTS1000_ReadDeviceAD(hDevice, ADBuffer, nRead, &nRetSizeWords, &nAvailSampsPoints,1.0))
			{
				printf("ReadDeviceAD error...\n");
				getch();
				goto ExitRead;
			}
			if (nRetSizeWords<=0)
			{
				goto NextRead;
			}
			nReadSizeRemain = nReadSizeRemain-nRetSizeWords;

#if 1
			ulTmp = GetTickCount()-ulTime;
			if (ulTmp/1000>ulSecCnt)
			{
				ulSecCnt = ulTmp/1000;
				dResult = (nReadSizeWords-nReadSizeRemain)*1.00/((ulTmp+1)/1000.0);
				dResult=dResult/1000000.0;
				printf("%dS ",ulSecCnt);
				printf("%.3fM��/��\n",dResult);
				nTChannel = 0;
				for(Index=0; Index<8; Index++)
				{			
					for(nADChannel=nTChannel; nADChannel<ChannelCount; nADChannel++)
					{
						if(ADPara.bChannelArray[nADChannel]  == TRUE) // �����ͨ��������
						{

							nTChannel = nADChannel + 1;
							ADData = ADBuffer[Index]&wMaxLSB;
							// ��ԭ��ת��Ϊ��ѹֵ
							switch(InputRange)
							{
							case 1: // -1000mV - +1000mV
								fVolt = (float)((2000.0/fLsbCount) * ADData - 1000.0);
								break;
							case 0:	// -5000mV - +5000mV
								fVolt = (float)((10000.0/fLsbCount) * ADData - 5000.0);
								break;
							default:
								break;
							}

							printf("CH%02d=%6.2f\t", nADChannel, fVolt); // ��ʾ��ѹֵ
							if ((Index+1) % ChannelCount == 0)
							{
								nTChannel = 0;
								printf("\n");
							}			
							break;
						}
					}
				} // for(Index=0; Index<64; Index++)
			}
#endif
		}
	
		if (nReadSizeRemain==0)
		{
			printf("���޵����(%I64d�ֽ�)��ɰ�������˳�\n", nReadSizeWords*2);
		}
	}

ExitRead:
	ACTS1000_StopDeviceAD(hDevice); // ֹͣAD
	ACTS1000_ReleaseDeviceAD(hDevice); // �ͷ�AD
	ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
	getch();
	
	
	return 0;
}

