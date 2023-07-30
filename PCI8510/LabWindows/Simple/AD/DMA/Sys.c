#include "windows.h"
#include <PCI8510.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 


#include "PCI8510.h"

int InputRange;
int SelectInputRange(void);

#define MAX_AD_CHANNELS 8 // �������ͨ����
#define SEGMENT_COUNT 32
#define HALF_SIZE_WORDS 4096

ULONG ADBuffer[SEGMENT_COUNT][HALF_SIZE_WORDS]; // ���仺����(�洢ԭʼ����)
ULONG g_ulSegmentID = 0;

int main (int argc, char *argv[]) 
{
	

	HANDLE hDevice;
	int DeviceLgcID;

	PCI8510_PARA_AD ADPara; // Ӳ������
	PCI8510_STATUS_DMA DMAStatus; // DMA״̬����

	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ

	int nChannel = 0;
	float fVolt;		
	HANDLE hDmaEvent;
	int i;
	int Index;
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   

	DeviceLgcID = 0;
	hDevice = PCI8510_CreateDevice(DeviceLgcID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		getch();
		return 0; // ��������豸����ʧ�ܣ��򷵻�
	}

	InputRange = SelectInputRange(); // Ҫ���û��Ӽ�����ѡ����������

	memset(&ADPara, 0x00, sizeof(ADPara)); // �����������λ��ȷ��ֵ0(ǿ�ҽ���)

	// Ԥ��Ӳ������
	ADPara.Gains[0]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��0��������
	ADPara.Gains[1]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��1��������
	ADPara.Gains[2]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��2��������
	ADPara.Gains[3]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��3��������	
	ADPara.Gains[4]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��4��������
	ADPara.Gains[5]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��5��������
	ADPara.Gains[6]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��6��������
	ADPara.Gains[7]				= PCI8510_GAINS_1MULT;  // ģ��ͨ��7��������	
	ADPara.Frequency			= 10000; // ����Ƶ��(Hz)
	ADPara.InputRange			= InputRange;
	ADPara.TriggerMode			= PCI8510_TRIGMODE_SOFT;
	ADPara.TriggerSource		= PCI8510_TRIGSRC_ATR; // ����Դ
	ADPara.TriggerType			= PCI8510_TRIGTYPE_EDGE; // ��������Ϊ���ش���	
	ADPara.TriggerDir			= PCI8510_TRIGDIR_NEGATIVE; // ��������Ϊ����
	ADPara.TrigLevelVolt		= 0;    // ������ƽ
	ADPara.ClockSource			= PCI8510_CLOCKSRC_IN; // ʱ��Դѡ�ð���ʱ��Դ
	ADPara.bClockOutput			= FALSE; // ��ֹʱ�����
	ADPara.OutTrigSource		= PCI8510_OUTTRIGSRC_TRIGGER0;
	ADPara.OutTrigSourceDir		= FALSE;	
	
	 hDmaEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!PCI8510_InitDeviceDmaAD( hDevice, hDmaEvent, &ADBuffer[0][0], 4096, SEGMENT_COUNT, HALF_SIZE_WORDS, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("InitDeviceDmaAD error...\n");
		getch();
		goto ExitRead1;  
	}

 	PCI8510_StartDeviceDmaAD(hDevice); // �����豸
  	while(!kbhit())
  	{			
		bFirstWait = TRUE; 
		while(TRUE) // ��ѯ��ǰ�������������Ƿ���׼������
		{
			if(WaitForSingleObject (hDmaEvent, 100)==WAIT_OBJECT_0) break; // �ȴ�DMA�¼�
			else
			{
				if(bFirstWait)
				{
					printf("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...\n");
					bFirstWait = FALSE;
				}
				if(kbhit()) goto ExitRead0;
			}			
		}
		
		if(!PCI8510_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			printf("Get Device Status Error...\n");	
			goto ExitRead0;
		}		

		if(DMAStatus.bBufferOverflow)
		{
			printf("DMA Overflow...\n");
			// Sleep(3000); // �����ʱ,Ϊ�����û��������,��ʱ3��
		}
		
		for( i=0; i<SEGMENT_COUNT && DMAStatus.bSegmentSts[g_ulSegmentID]; i++)
		{
			if(DMAStatus.bSegmentSts[g_ulSegmentID])
			{
				for( Index=0; Index<64;)
				{			
					for(nChannel=0; nChannel<MAX_AD_CHANNELS; nChannel++)
					{
						if(kbhit()) goto ExitRead0;
						// ��ԭ��ת��Ϊ��ѹֵ
						switch(InputRange)
						{
						case 0: // -5V - +5V
							fVolt = (float)((5000.0/65536) * (ADBuffer[g_ulSegmentID][Index]&0xFFFF));
							break;
						case 1: // -10V - +10V
							fVolt = (float)((10000.0/65536) * (ADBuffer[g_ulSegmentID][Index]&0xFFFF));
							break;
						case 2: // -5V - +5V
							fVolt = (float)((10000.0/65536) * (ADBuffer[g_ulSegmentID][Index]&0xFFFF) - 5000.0);
							break;
						case 3: // -10V - +10V
							fVolt = (float)((20000.0/65536) * (ADBuffer[g_ulSegmentID][Index]&0xFFFF) - 10000.0);
							break;
						default:
							break;
						}
						printf("CH%02d=%6.2f\t", nChannel, fVolt); // ��ʾ��ѹֵ
						if ((nChannel+1)%4 == 0)
						{
							printf("\n"); // ��ʾ��ѹֵ
						}
						Index++;
					}			
				} // for(Index=0; Index<64; Index++)
				
				if(!PCI8510_SetDevStatusDmaAD(hDevice, g_ulSegmentID))
				{
					printf("SetDevStatusDmaAD error...\n");
					getch();
					goto ExitRead0;
				}
				
				g_ulSegmentID = (g_ulSegmentID + 1) % SEGMENT_COUNT;
			} // for(Index=0; Index<64; Index++)
		} // end of for(SegmentID=0; SegmentID<SEGMENT_COUNT; SegmentID++)
	} // end of while(!kbhit())
ExitRead0:
	getch();
	PCI8510_ReleaseDeviceDmaAD(hDevice); // �ͷ�AD
	PCI8510_ReleaseSystemEvent(hDmaEvent);
ExitRead1:
	PCI8510_ReleaseDevice(hDevice); // �ͷ��豸����
	getch();
	
	return 0;
}

//////////////////////////////////////////////////////
// ��ȡ�û�ѡ�����������
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. 0V �� +5V\n");
	printf("1. 0V �� +10V\n");
	printf("2. -5V �� +5V\n");
	printf("3. -10V �� +10V\n");

	printf("Please Select Input Range[0-3]:");
	scanf("%d", &InputRange);
	if(InputRange<0 || InputRange>3) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
	return InputRange;
}
