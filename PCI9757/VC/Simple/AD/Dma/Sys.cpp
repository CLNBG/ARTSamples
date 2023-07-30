// ˵���� ��������ʾ�������DMA��ʽ��ȡAD����

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI9757.h"

int InputRange;
int SelectInputRange(void);

#define SEGMENT_COUNT 64  // ���建����ҳ����Ϊ32��
#define HALF_SIZE_WORDS 4096 // ���建����ÿҳ�γ���Ϊ4096��

LONG ADBuffer[SEGMENT_COUNT][HALF_SIZE_WORDS]; // ���仺������(�洢ԭʼ����)
ULONG g_ulSegmentID = 0;

int main(int argc, char* argv[])
{
	HANDLE hDevice; // �豸��� 
	int DeviceLgcID;
	PCI9757_PARA_AD ADPara; // Ӳ������
	PCI9757_STATUS_DMA DMAStatus; // DMA״̬����
	
	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ	
	int nCurrentChannel = 0, nChannel = 0;
	WORD ADData;
	float fVolt;
	
	DeviceLgcID = 0; // ���ʵ�һ��PCI9757��(�ٶ�ϵͳ��ֻ��һ̨PCI9757��)
	hDevice = PCI9757_CreateDevice(DeviceLgcID); // �����豸����
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
	
	ADPara.Frequency		= 500000; // ����Ƶ��(Hz)
	ADPara.TriggerMode		= PCI9757_TRIGMODE_SOFT;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // ����Դ
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // ��������Ϊ���ش���	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // ��������Ϊ����
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // ����������	
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // ʱ��Դѡ�ð���ʱ��Դ
	
	HANDLE hDmaEvent = PCI9757_CreateSystemEvent();
	if(hDmaEvent == INVALID_HANDLE_VALUE)
	{
		printf("Create Dma Event Error\n");
		_getch();
		return 0;
	}

	if(!PCI9757_InitDeviceDmaAD( // ��ʼ��DMA����
		hDevice, // �豸���,������CreateDevice�����ɹ�����
		hDmaEvent, // DMA�¼����,������CreateSystemEvent�����ɹ�����
		&ADBuffer[0][0], // �û�������ҳ���׵�ַ
		HALF_SIZE_WORDS, // ÿ��װ���û�������ҳ�εĵ�����ֻ��С�ڻ����HALF_SIZE_WORDS
		SEGMENT_COUNT, // �û�������ҳ�θ���
		HALF_SIZE_WORDS, // �û�������ÿҳ�γ���
		&ADPara)) // ��ʼ���豸��Ӳ������
	{
		printf("PCI9757_InitDeviceAD Error...\n");
		_getch();
		goto ExitRead1;  
	}
	
	PCI9757_StartDeviceDmaAD(hDevice); // �����豸
	
	while(!_kbhit())
	{			
		bFirstWait = TRUE; 
		while(TRUE) // ��ѯ��ǰ�������������Ƿ���׼������
		{
			if(WaitForSingleObject (hDmaEvent, 250) == WAIT_OBJECT_0) break; // �ȴ�RAMƹ���л��ж��¼�
			else
			{
				if(bFirstWait)
				{
					printf("Wait...\n");
					bFirstWait = FALSE;
				}
				if(_kbhit()) goto ExitRead0;
			}			
		}
		
		// �ض�DMA�ĸ���״̬
		if(!PCI9757_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			printf("Get Device Status Error...\n");	
			goto ExitRead0;
		}		
		
		// ����û��������Ƿ��й����(��ѡ)
		if(DMAStatus.bBufferOverflow)
		{
			printf("DMA Overflow...\n");
		}
		
		// ���ݻض�״̬��������еĻ�����Ƿ���ã�Ȼ�����δ���
		for(int i=0; i<SEGMENT_COUNT && DMAStatus.bSegmentSts[g_ulSegmentID]; i++)
		{
			if(DMAStatus.bSegmentSts[g_ulSegmentID])
			{
				ULONG nTChannel = 0;
				for (int Index=0; Index<64; Index++)
				{	
					if(_kbhit()) goto ExitRead0;
					// ��ԭ���2λ���ε�
					ADData = (USHORT)(ADBuffer[g_ulSegmentID][Index]);
					switch(InputRange)
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
					printf("CH%02d=%6.2f\t", nChannel, fVolt); // ��ʾ��ѹֵ
					nChannel++;
					if(nChannel >= 4) 
					{
						nChannel = 0;
						printf("\n"); // ��ĩͨ��ʱ����
					}					
				}			
			// ���մ�����ɵĻ�����ҳ��־��λ��֪ͨӲ���������û�����ҳ�ѱ�����
			if(!PCI9757_SetDevStatusDmaAD(hDevice, g_ulSegmentID))
			{
				printf("SetDevStatusDmaAD Error...\n");
				_getch();
				goto ExitRead0;
			}
			g_ulSegmentID = (g_ulSegmentID + 1) % SEGMENT_COUNT;
		}
	}
}
	
ExitRead0:	
	_getch();
	PCI9757_ReleaseDeviceDmaAD(hDevice); // �ͷ�AD
	PCI9757_ReleaseSystemEvent(hDmaEvent); // �ͷ�DMA�¼����
ExitRead1:
	PCI9757_ReleaseDevice(hDevice); // �ͷ��豸����
	_getch();
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