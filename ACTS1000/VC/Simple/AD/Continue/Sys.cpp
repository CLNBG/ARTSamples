// ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"

int InputRange;

#define AD_DATA_LEN 256*1024 // Ҫ��ȡ�ʹ�����AD���ݳ��ȣ�����֣�
USHORT ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)


char g_strAIRange[3][64] = {"5~-5V", "1~-1V", "10~-10V"};
int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;
	ULONGLONG ulDDR2LengthByte = 0;

	ACTS1000_PARA_AD ADPara; // Ӳ������
	ACTS1000_STATUS_AD ADStatus;
	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	ULONG nRead;

	int nCurrentChannel = 0,nADChannel = 0;
	WORD ADData;
	float fVolt;
	ACTS1000_AD_MAIN_INFO ADMainInfo;
	WORD wMaxLSB = 0x3FFF;
	float fLsbCount = 16384.0;
	ULONG nSampRangeCount;
	ULONG nSampRangeVal[8];


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
	ACTS1000_GetSupportRangeInfo(hDevice,&nSampRangeCount,nSampRangeVal);			

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

	for (int i=0; i<nSampRangeCount; i++)
	{
		printf("Range%d = %d(%s)\n", i, nSampRangeVal[i],g_strAIRange[nSampRangeVal[i]]);
	}

	InputRange = nSampRangeVal[0]; //


	ulDDR2LengthByte = ADMainInfo.nDepthOfMemory;

	wMaxLSB = (WORD)(ADMainInfo.nSampCodeCount - 1);
	fLsbCount = (float)ADMainInfo.nSampCodeCount;





	ULONG nRevisionID = 0;
	if(!ACTS1000_GetBusInfo(hDevice, NULL, NULL, NULL, &nRevisionID))
	{
		printf("ACTS1000_GetBusInfo error...\n");
	}

	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)

	// Ԥ��Ӳ������
	ADPara.SampleMode			= ACTS1000_SAMPMODE_CONTINUOUS; // ���޵����
	ADPara.FreqDivision			= 1;	
	for (int nCH=0; nCH<ADMainInfo.nChannelCount; nCH++)
	{
		ADPara.bChannelArray[nCH]		= TRUE; // ����ͨ��0����
		ADPara.InputRange[nCH]		= InputRange;			// ģ�����������̷�Χ
		ADPara.CouplingType[nCH]		= ACTS1000_COUPLING_DC;	// ֱ�����
	}
	ADPara.M_Length				= 0;					// M����
	ADPara.N_Length				= 1024*32;			// N����
	ADPara.PFISel				= ACTS1000_PFISEL_TRIG_IN;
	ADPara.TriggerMode			= ACTS1000_TRIGMODE_POST; // �󴥷�
	ADPara.TriggerSource		= ACTS1000_TRIGMODE_SOFT;	// ��������
	ADPara.TriggerDir			= ACTS1000_TRIGDIR_NEGATIVE; // �½��ش���
	ADPara.TrigLevelVolt		= 0;
	ADPara.ReferenceClock			= ACTS1000_RECLK_ONBOARD;	// ʹ���ڲ�ʱ��
	ADPara.TimeBaseClock			= ACTS1000_TBCLK_IN;	
	ADPara.bMasterEn			= 0;
	ADPara.TrigCount			= 1;
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

	LONG ulTime = GetTickCount();
	double dResult;
	__int64 i64Cnt=0;
	LONG ulTmp=0;
	LONG ulSecCnt=0;

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

	ulTime = GetTickCount();
	ULONGLONG ulReadCount = 0;
	nRead = AD_DATA_LEN;
	if (ADPara.bSaveFile)	// �Զ�����
	{
		while(TRUE) 
		{
			if(!ACTS1000_GetDevStatusAD(hDevice, &ADStatus))
			{
				printf("GetDevStatusAD Error\n");
				goto ExitRead;
			}			
			if(_kbhit())
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
	else
	{

		while ( !_kbhit() )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		{			
			if(!ACTS1000_ReadDeviceAD(hDevice, ADBuffer, nRead, &nRetSizeWords, &nAvailSampsPoints,5.0))
			{
				printf("ReadDeviceDmaAD error...\n");
				_getch();
				goto ExitRead;
			}
			if (nRetSizeWords<=0)
			{
				continue;
			}
			ulReadCount+=nRetSizeWords;
#if 1
			ulTmp = GetTickCount()-ulTime;
			if (ulTmp/1000>ulSecCnt)
			{
				ulSecCnt = ulTmp/1000;
				dResult = ulReadCount*1.00/((ulTmp+1)/1000.0);
				dResult=dResult/1000000.0;
				printf("%dS ",ulSecCnt);
				printf("%.3fM��/��\n",dResult);
				ULONG nTChannel = 0;
				for(int Index=0; Index<8; Index++)
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
							case ACTS1000_INPUT_N1000_P1000mV: // -1000mV - +1000mV
								fVolt = (float)((2000.0/fLsbCount) * ADData - 1000.0);
								break;
							case ACTS1000_INPUT_N5000_P5000mV:	// -5000mV - +5000mV

								if (nRevisionID == 0xD0)
								{
									fVolt = (float)((20000.0/fLsbCount) * ADData - 10000.0);
								}
								else
								{
									fVolt = (float)((10000.0/fLsbCount) * ADData - 5000.0);

								}
								break;
							case ACTS1000_INPUT_N10000_P10000mV: // -10000mV - +10000mV
								fVolt = (float)((20000.0/fLsbCount) * ADData - 10000.0);
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
	}

ExitRead:
	ACTS1000_StopDeviceAD(hDevice); // ֹͣAD
	ACTS1000_ReleaseDeviceAD(hDevice); // �ͷ�AD
	ACTS1000_ReleaseDevice(hDevice); // �ͷ��豸����
	return 0;
}
