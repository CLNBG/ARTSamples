// ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI8501.h"

int InputRange;
int SelectInputRange(void);

#define AD_DATA_LEN 1024 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
USHORT ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;

	PCI8501_PARA_AD ADPara; // Ӳ������
	PCI8501_STATUS_AD ADStatus;
	LONG nReadSizeWords;   // ÿ�ζ�ȡAD���ݵĳ���(��)
	LONG nRetSizeWords;
	LONG nReadSize;
	
	int nCurrentChannel = 0,nADChannel = 0;
	WORD ADData;
	float fVolt;
	
	DeviceLgcID = 0;
	hDevice = PCI8501_CreateDevice(DeviceLgcID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	PCI8501_GetDDR2Length(hDevice, &ulDDR2Length);

	InputRange = SelectInputRange(); // Ҫ���û��Ӽ�����ѡ����������

	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)

	// Ԥ��Ӳ������
	ADPara.Frequency			= 100000;				// ����Ƶ��(Hz)
	for (int iChannel=0; iChannel<8; iChannel++)
	{
		ADPara.InputRange[iChannel]	= InputRange;			// ģ�����������̷�Χ
		ADPara.Gains[iChannel]	= PCI8501_GAINS_1MULT;
	}
	ADPara.M_Length				= 0;						// M����
	ADPara.N_Length				= 1024;				// N����
	ADPara.TriggerMode			= PCI8501_TRIGMODE_MIDL;	// Ӳ���м䴥��(����Ԥ�������󴥷�����)
	ADPara.TriggerSource		= PCI8501_TRIGMODE_SOFT;	// �������
	ADPara.TriggerDir			= PCI8501_TRIGDIR_NEGATIVE; // �½��ش���
	ADPara.TrigLevelVolt		= 0;
	ADPara.ClockSource			= PCI8501_CLOCKSRC_IN;		// ʹ���ڲ�ʱ��
	ADPara.bClockSourceDir		= FALSE;
	ADPara.OutClockSource		= PCI8501_OUTCLOCKSRC_TRIGGER0;

	if(!PCI8501_InitDeviceAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("InitDeviceAD error...\n");
		_getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}
	
	nReadSizeWords = (ADPara.M_Length + ADPara.N_Length)*8; // M��N����Ϊ��ȡ���ݵĳ���

	ULONG ulStartAddr=0;
	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ
	while ( !_kbhit() )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	{			
		if(!PCI8501_StartDeviceAD(hDevice)) // �����豸
		{
			printf("StartDeviceAD Error...\n");
			_getch();
		}
		if(!PCI8501_SetDeviceTrigAD(hDevice)) // ����AD
		{
			printf("SetDeviceTrigAD Error...\n");
			_getch();
		}
		ulStartAddr=0;
		nReadSize = nReadSizeWords;
		bFirstWait = TRUE; 
		while(TRUE) // ��ѯ��ǰ�������������Ƿ���׼������
		{
			if(!PCI8501_GetDevStatusAD(hDevice, &ADStatus))
			{
				printf("GetDevStatusAD error...\n");
				_getch();
				goto ExitRead;
			}
			
			if(ADStatus.bComplete)
			{
				break;  // ������FIFO�洢���������ﵽ�������ϣ����˳�״̬��ѯ,��ʼ��ȡ��������
			}
			else		
			{
				if(bFirstWait)
				{
					printf("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");
					bFirstWait = FALSE;
				}
				if(_kbhit()) goto ExitRead; // ����û����������˳�
			}
		}
		
		if(!PCI8501_StopDeviceAD(hDevice)) // �����豸
		{
			printf("StartDeviceAD Error...\n");
			_getch();
		}
		
		LONG offsetAddr	= ADStatus.lEndAddr - nReadSizeWords*sizeof(USHORT) + 4;
		if (offsetAddr<0)
		{
			ulStartAddr = ulDDR2Length*1024*1024 + (ADStatus.lEndAddr - nReadSizeWords*sizeof(USHORT) + 4);
		}
		else
			ulStartAddr = (ULONG)offsetAddr;
		
		while (nReadSize>0)
		{
			if(!PCI8501_ReadDeviceAD(hDevice, ADBuffer, AD_DATA_LEN, ulStartAddr, &nRetSizeWords))
			{
				printf("ReadDeviceDmaAD error...\n");
				_getch();
				goto ExitRead;
			}
 			nReadSize = nReadSize-AD_DATA_LEN;
			ulStartAddr = ulStartAddr + AD_DATA_LEN*sizeof(USHORT);
			
			for(int Index=0; Index<64;)
			{			
				for(nADChannel=0; nADChannel<8; nADChannel++)
				{
					if(_kbhit()) goto ExitRead;
					
					ADData = ADBuffer[Index]&0xFFFF;
					// ��ԭ��ת��Ϊ��ѹֵ
					switch(InputRange)
					{
					case PCI8501_INPUT_N10000_P10000mV: // -10000mV - +10000mV
						fVolt = (float)((20000.0/65536) * ADData - 10000.0);
						break;
					case PCI8501_INPUT_N5000_P5000mV:	// -5000mV - +5000mV
						fVolt = (float)((10000.0/65536) * ADData - 5000.0);
						break;
					case PCI8501_INPUT_N2500_P2500mV:	// -2500mV - +2500mV
						fVolt = (float)((5000.0/65536) * ADData - 2500.0);
						break;
					case PCI8501_INPUT_0_P10000mV:	// 0mV - +10000mV
						fVolt = (float)((10000.0/65536) * ADData);
						break;
					case PCI8501_INPUT_0_P5000mV:	// 0mV - +5000mV
						fVolt = (float)((5000.0/65536) * ADData);
						break;
					default:
						break;
					}
					
					printf("CH%02d=%6.2f\t", nADChannel, fVolt); // ��ʾ��ѹֵ
					Index++;
				}
				printf("\n");
			} // for(Index=0; Index<64; Index++)
		}
		
	}
	
ExitRead:
	PCI8501_StopDeviceAD(hDevice); // ֹͣAD
	PCI8501_ReleaseDeviceAD(hDevice); // �ͷ�AD
	PCI8501_ReleaseDevice(hDevice); // �ͷ��豸����
	return 0;
}


//////////////////////////////////////////////////////
// ��ȡ�û�ѡ�����������
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V �� +10V\n");
	printf("1. -5V �� +5V\n");
	printf("2. -2500V �� +2500V\n");
	printf("3. 0mV �� +10000mV\n");
	printf("4. 0mV �� +5000mV\n");

	printf("Please Select Input Range[0-4]:");
	scanf_s("%d", &InputRange);
	if(InputRange<0 || InputRange>4) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
	return InputRange;
}