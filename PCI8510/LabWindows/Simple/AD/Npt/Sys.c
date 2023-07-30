#include "windows.h"
#include <PCI8510.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 


#include "PCI8510.h"

int InputRange;
int SelectInputRange(void);

#define MAX_AD_CHANNELS 8 // �������ͨ����
#define AD_DATA_LEN 1024*4 // Ҫ��ȡ�ʹ����AD���ݳ��ȣ�����֣�
ULONG ADBuffer[AD_DATA_LEN]; // ���仺����(�洢ԭʼ����)

int main (int argc, char *argv[]) 
{
	

	HANDLE hDevice;
	int DeviceID;
	PCI8510_PARA_AD ADPara; // Ӳ������
	LONG nReadSizeWords;   // ÿ�ζ�ȡAD���ݵĳ���(��)
	LONG nRetSizeWords;	
	int nChannel = 0, Index = 0;	
	float fVolt;
	BOOL bFirstWait = TRUE; // Ϊÿ�εȴ�ֻ��ʾһ����ʾ
	
	int i;
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   

	DeviceID = 0;
	hDevice = PCI8510_CreateDevice(DeviceID); // �����豸����
	if(hDevice == INVALID_HANDLE_VALUE) return 0; // ��������豸����ʧ�ܣ��򷵻�
	
	InputRange = SelectInputRange(); // Ҫ���û��Ӽ�����ѡ����������
	memset(&ADPara, 0x00, sizeof(ADPara)); // ��ʼ������֪��(ǿ�ҽ���)
	
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
	
	if(!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // ��ʼ��Ӳ��
	{
		printf("PCI8510_InitDeviceProAD...\n");
		getch();
		return 0;  
	}
	
	nReadSizeWords = AD_DATA_LEN; // ����ȡ���ݳ���
	
	if(!PCI8510_StartDeviceProAD(hDevice)) // �����豸
	{
		printf("StartDeviceAD Error...\n");
		getch();
	}
	
	while ( !kbhit() )
	{			
		bFirstWait = TRUE;
		PCI8510_ReadDeviceProAD_Npt(hDevice, ADBuffer, nReadSizeWords, &nRetSizeWords);
		for(Index=0; Index<64;)
		{			
			for(nChannel=0; nChannel<MAX_AD_CHANNELS; nChannel++)
			{
				if(kbhit()) goto ExitRead0;
				// ��ԭ��ת��Ϊ��ѹֵ
				switch(InputRange)
				{
				case 0: // -5V - +5V
					fVolt = (float)((5000.0/65536) * (ADBuffer[Index]&0xFFFF));
					break;
				case 1: // -10V - +10V
					fVolt = (float)((10000.0/65536) * (ADBuffer[Index]&0xFFFF));
					break;
				case 2: // -5V - +5V
					fVolt = (float)((10000.0/65536) * (ADBuffer[Index]&0xFFFF) - 5000.0);
					break;
				case 3: // -10V - +10V
					fVolt = (float)((20000.0/65536) * (ADBuffer[Index]&0xFFFF) - 10000.0);
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
	}
	
ExitRead0:
	PCI8510_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
	PCI8510_ReleaseDevice( hDevice ); // �ͷ��豸����
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
