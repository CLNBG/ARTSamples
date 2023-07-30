// ˵���� ��������ʾ������ó����ѯ��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI9757.h"

int InputRange;
int SelectInputRange(void);


#define MAX_AD_CHANNELS 4 // �������ͨ����
#define AD_DATA_LEN 1024*8 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
LONG ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceID;
	PCI9757_PARA_AD ADPara; // Ӳ������
	LONG nReadSizeWords;   // ÿ�ζ�ȡAD���ݵĳ���(��)
	LONG nRetSizeWords;
	
	int nCurrentChannel = 0, nChannel = 0, Index = 0;
	WORD ADData;
	
	float fVolt;
	
	DeviceID = 0;
	hDevice = PCI9757_CreateDevice(DeviceID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE) return 0; // ��������豸����ʧ�ܣ��򷵻�
	
	InputRange = SelectInputRange(); // Ҫ���û��Ӽ�����ѡ����������
	
	memset(&ADPara, 0x00, sizeof(ADPara)); // ��ʼ������֪��(ǿ�ҽ���)
	
	// Ԥ��Ӳ������
	ADPara.bChannelArray[0]		= TRUE; // ����ͨ��0����
	ADPara.bChannelArray[1]		= TRUE; // ����ͨ��1����
	ADPara.bChannelArray[2]		= TRUE; // ����ͨ��2����
	ADPara.bChannelArray[3]		= TRUE; // ����ͨ��3����
	
	ADPara.InputRange[0]		= InputRange;  // ģ��ͨ��0��������
	ADPara.InputRange[1]		= InputRange;  // ģ��ͨ��1��������
	ADPara.InputRange[2]		= InputRange;  // ģ��ͨ��2��������
	ADPara.InputRange[3]		= InputRange;  // ģ��ͨ��3��������
	
	ADPara.Frequency		= 10000; // ����Ƶ��(Hz)
	ADPara.TriggerMode		= PCI9757_TRIGMODE_SOFT;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // ����Դ
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // ��������Ϊ���ش���	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // ��������Ϊ����
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // ����������	
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // ʱ��Դѡ�ð���ʱ��Դ
	
	if(!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("PCI9757_InitDeviceProAD...\n");
		_getch();
		return 0;  
	}
	
	int ChannelCount = 0;	
	for(nChannel=0; nChannel<4; nChannel++)
	{
		if(ADPara.bChannelArray[nChannel] == TRUE)
		{
			ChannelCount++;
		}
	}
	
	nReadSizeWords = 4096 - 4096 %ChannelCount; // �����ݳ�����ת��Ϊ˫��	
	
	if(!PCI9757_StartDeviceProAD(hDevice)) // �����豸
	{
		printf("StartDeviceAD Error...\n");
		_getch();
	}
	
	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ
	while ( !_kbhit() )
	{			
		bFirstWait = TRUE;
		PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer, nReadSizeWords, &nRetSizeWords);
		ULONG nTChannel = 0;
		for(Index=0; Index<64; Index++)
		{			
			for(nChannel=nTChannel; nChannel<4; nChannel++)
			{
				if(ADPara.bChannelArray[nChannel]  == TRUE) // �����ͨ��������
				{
					nCurrentChannel = nChannel; // ��¼��ͨ����
					nTChannel = nChannel + 1;
					
					
					if(_kbhit()) goto ExitRead0;
					// ��ԭ���4λ���ε�
					ADData = (USHORT)(ADBuffer[Index]&0xFFFF);
					// ��ԭ��ת��Ϊ��ѹֵ
					switch (InputRange)
					{
					case PCI9757_INPUT_N10000_P10000mV: // -10V - +10V
						fVolt = (float)((20000.0/65536) * ADData - 10000.0);
						break;
					case PCI9757_INPUT_N5000_P5000mV: // -5V - +5V
						fVolt = (float)((10000.0/65536) * ADData - 5000.0);
						break;
					case PCI9757_INPUT_N2500_P2500mV: // -5V - +5V
						fVolt = (float)((5000.0/65536) * ADData - 2500.0);
						break;
					case PCI9757_INPUT_0_P10000mV: // -1V - +1V
						fVolt = (float)((10000.0/65536) * ADData);
						break;
					case PCI9757_INPUT_0_P5000mV: // -1V - +1V
						fVolt = (float)((5000.0/65536) * ADData);
						break;
					default:
						break;
					}
					
					//	printf("%d   \t",ADData);
					printf("CH%02d=%6.2f\t", nCurrentChannel, fVolt); // ��ʾ��ѹֵ	
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
	
ExitRead0:
	PCI9757_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
	PCI9757_ReleaseDevice( hDevice ); // �ͷ��豸����
	return 0;
}


//////////////////////////////////////////////////////
// ��ȡ�û�ѡ�����������
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V  �� +10V\n");
	printf("1. -5V   �� +5V\n");
	printf("2. -2.5V   �� +2.5V\n");
	printf("3. 0V    �� +10V\n");
	printf("4. 0V    �� +5V\n");
	
	printf("Please Select Input Range[0-4]:");
	scanf_s("%d", &InputRange);
	if(InputRange<0 || InputRange>4) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
	return InputRange;
}
