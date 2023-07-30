#include "windows.h"
#include <PCI8510.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 


#include "PCI8510.h"

int InputRange;
int SelectInputRange(void);

#define MAX_AD_CHANNELS 8 // 定义最大通道数
#define AD_DATA_LEN 1024*4 // 要读取和处理的AD数据长度（点或字）
ULONG ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)

int main (int argc, char *argv[]) 
{
	

	HANDLE hDevice;
	int DeviceID;
	PCI8510_PARA_AD ADPara; // 硬件参数
	LONG nReadSizeWords;   // 每次读取AD数据的长度(字)
	LONG nRetSizeWords;	
	int nChannel = 0, Index = 0;	
	float fVolt;
	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示
	
	int i;
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   

	DeviceID = 0;
	hDevice = PCI8510_CreateDevice(DeviceID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE) return 0; // 如果创建设备对象失败，则返回
	
	InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程
	memset(&ADPara, 0x00, sizeof(ADPara)); // 初始化成已知量(强烈建议)
	
	// 预置硬件参数
	ADPara.Gains[0]				= PCI8510_GAINS_1MULT;  // 模拟通道0输入量程
	ADPara.Gains[1]				= PCI8510_GAINS_1MULT;  // 模拟通道1输入量程
	ADPara.Gains[2]				= PCI8510_GAINS_1MULT;  // 模拟通道2输入量程
	ADPara.Gains[3]				= PCI8510_GAINS_1MULT;  // 模拟通道3输入量程	
	ADPara.Gains[4]				= PCI8510_GAINS_1MULT;  // 模拟通道4输入量程
	ADPara.Gains[5]				= PCI8510_GAINS_1MULT;  // 模拟通道5输入量程
	ADPara.Gains[6]				= PCI8510_GAINS_1MULT;  // 模拟通道6输入量程
	ADPara.Gains[7]				= PCI8510_GAINS_1MULT;  // 模拟通道7输入量程	
	ADPara.Frequency			= 10000; // 采样频率(Hz)
	ADPara.InputRange			= InputRange;
	ADPara.TriggerMode			= PCI8510_TRIGMODE_SOFT;
	ADPara.TriggerSource		= PCI8510_TRIGSRC_ATR; // 触发源
	ADPara.TriggerType			= PCI8510_TRIGTYPE_EDGE; // 触发类型为边沿触发	
	ADPara.TriggerDir			= PCI8510_TRIGDIR_NEGATIVE; // 触发方向为负向
	ADPara.TrigLevelVolt		= 0;    // 触发电平
	ADPara.ClockSource			= PCI8510_CLOCKSRC_IN; // 时钟源选用板内时钟源
	ADPara.bClockOutput			= FALSE; // 禁止时钟输出
	ADPara.OutTrigSource		= PCI8510_OUTTRIGSRC_TRIGGER0;
	ADPara.OutTrigSourceDir		= FALSE;	
	
	if(!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // 初始化硬件
	{
		printf("PCI8510_InitDeviceProAD...\n");
		getch();
		return 0;  
	}
	
	nReadSizeWords = AD_DATA_LEN; // 将读取数据长度
	
	if(!PCI8510_StartDeviceProAD(hDevice)) // 启动设备
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
				// 将原码转换为电压值
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
				printf("CH%02d=%6.2f\t", nChannel, fVolt); // 显示电压值
				if ((nChannel+1)%4 == 0)
				{
					printf("\n"); // 显示电压值
				}
				Index++;
			}			
		} // for(Index=0; Index<64; Index++)
	}
	
ExitRead0:
	PCI8510_ReleaseDeviceProAD( hDevice ); // 释放AD
	PCI8510_ReleaseDevice( hDevice ); // 释放设备对象
	return 0;
}

//////////////////////////////////////////////////////
// 获取用户选择的输入量程
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. 0V ～ +5V\n");
	printf("1. 0V ～ +10V\n");
	printf("2. -5V ～ +5V\n");
	printf("3. -10V ～ +10V\n");

	printf("Please Select Input Range[0-3]:");
	scanf("%d", &InputRange);
	if(InputRange<0 || InputRange>3) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return InputRange;
}
