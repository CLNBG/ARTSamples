
//#include <ansi_c.h>	 
#include "windows.h"
#include <PCI9757.H> 
#include <stdio.h>  

int InputRange;
int SelectInputRange(void);   

#define MAX_AD_CHANNELS 4 // �������ͨ����
#define AD_DATA_LEN 1024*4 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
LONG ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)

static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	
	
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
	scanf("%d", &InputRange);
	if(InputRange<0 || InputRange>4) goto Repeat; // �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��
	return InputRange;
}



int main (int argc, char *argv[])

{
  	PCI9757_STATUS_AD ADStatus;
	HANDLE hDevice;
	int DeviceID;
	PCI9757_PARA_AD ADPara; // Ӳ������
	LONG nReadSizeWords;   // ÿ�ζ�ȡAD���ݵĳ���(��)
	LONG nRetSizeWords;
	int Index;
	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ    
	int nCurrentChannel = 0, nChannel = 0;
	WORD ADData;
	float fVolt;
	ULONG nTChannel = 0;      
		
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   
	
DeviceID = 0;
	hDevice = PCI9757_CreateDevice(DeviceID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Create Error...\n");
		getch();
		return 0;
	}
	
	InputRange = SelectInputRange(); // Ҫ���û��Ӽ�����ѡ����������
	
	memset(&ADPara, 0x00, sizeof(ADPara)); // ��ʼ������֪��(ǿ�ҽ���)
	
	ADPara.bChannelArray[0]		= TRUE; // ����ͨ��0����
	ADPara.bChannelArray[1]		= TRUE; // ����ͨ��1����
	ADPara.bChannelArray[2]		= TRUE; // ����ͨ��2����
	ADPara.bChannelArray[3]		= TRUE; // ����ͨ��3����
	
	ADPara.InputRange[0]		= InputRange;  // ģ��ͨ��0��������
	ADPara.InputRange[1]		= InputRange;  // ģ��ͨ��1��������
	ADPara.InputRange[2]		= InputRange;  // ģ��ͨ��2��������
	ADPara.InputRange[3]		= InputRange;  // ģ��ͨ��3��������
	
	ADPara.Frequency		= 10000; // ����Ƶ��(Hz)
	ADPara.TriggerMode		= 0;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // ����Դ
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // ��������Ϊ���ش���	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // ��������Ϊ����
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // ����������
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // ʱ��Դѡ�ð���ʱ��Դ
	
	if(!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("PCI9757_InitDeviceProAD...\n");
		getch();
		return 0;  
	}
	
	nReadSizeWords = 4096; // �����ݳ�����ת��Ϊ˫��	
	
	if(!PCI9757_StartDeviceProAD(hDevice)) // �����豸
	{
		printf("StartDeviceAD Error...\n");
		getch();
	}
		

	while ( !kbhit() )
	{			
		bFirstWait = TRUE; 
		while(TRUE) // ��ѯ��ǰ�������������Ƿ���׼������
		{
			if(!PCI9757_GetDevStatusProAD(hDevice, &ADStatus))
			{
				printf("Get Device Status Error...\n");
				goto ExitRead0;
			}
			
			if(ADStatus.bHalf)
			{
				break;  // ������FIFO�洢���������ﵽ�������ϣ����˳�״̬��ѯ,��ʼ��ȡ��������
			}
			else		
			{
				if(bFirstWait)
				{
					printf("\nWait...\n");	
					bFirstWait = FALSE;
				}	
			}
		}
		
		PCI9757_ReadDeviceProAD_Half(hDevice, ADBuffer, nReadSizeWords, &nRetSizeWords);
		
	
		for(Index=0; Index<64; Index++)
		{					
			if(kbhit()) goto ExitRead0;
			// ��ԭ���2λ���ε�
			ADData = (WORD)((ADBuffer[Index])&0xfFFF);
			switch (InputRange)
			{
			case 0: // -10V - +10V
				fVolt = (float)((20000.0/65536) * ADData-10000.0);
				break;
			case 1: // -5V - +5V
				fVolt = (float)((10000.0/65536) * ADData - 5000.0);
				break;
			case 2: // -5V - +5V
				fVolt = (float)((5000.0/65536) * ADData - 2500.0);
				break;
			case 3: // -1V - +1V
				fVolt = (float)((10000.0/65536) * ADData);
				break;
			case 4: // -1V - +1V
				fVolt = (float)((5000.0/65536) * ADData);
				break;
			default:
				break;
			}
			printf("CH%02d=%6.2f\t", nTChannel, fVolt); // ��ʾ��ѹֵ
			nTChannel++;
			if(nTChannel >= 4) 
			{
				nTChannel = 0;
				printf("\n"); // ��ĩͨ��ʱ����
			}
			
		}
	}
	
	
ExitRead0:
	PCI9757_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
	PCI9757_ReleaseDevice( hDevice ); // �ͷ��豸����
	return 0;
}



