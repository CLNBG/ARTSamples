
//#include <ansi_c.h>	 
#include "windows.h"
#include <PCI9757.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 

int InputRange;
int SelectInputRange(void);


#define MAX_AD_CHANNELS 4 // 定义最大通道数
#define AD_DATA_LEN 1024*8 // 要读取和处理的AD数据长度（点或字）
LONG ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)
//////////////////////////////////////////////////////
// 获取用户选择的输入量程
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V  ～ +10V\n");
	printf("1. -5V   ～ +5V\n");
	printf("2. -2.5V   ～ +2.5V\n");
	printf("3. 0V    ～ +10V\n");
	printf("4. 0V    ～ +5V\n");
	
	printf("Please Select Input Range[0-4]:");
	scanf("%d", &InputRange);
	if(InputRange<0 || InputRange>4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return InputRange;
}


int main (int argc, char *argv[])

{
HANDLE hDevice;
	int DeviceID;
	PCI9757_PARA_AD ADPara; // 硬件参数
	LONG nReadSizeWords;   // 每次读取AD数据的长度(字)
	LONG nRetSizeWords;
	int ChannelCount = 0;
	int nCurrentChannel = 0, nChannel = 0, Index = 0;
	WORD ADData;
	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示     
	float fVolt;
	ULONG nTChannel = 0;         
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   
DeviceID = 0;
	hDevice = PCI9757_CreateDevice(DeviceID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE) return 0; // 如果创建设备对象失败，则返回
	
	InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程
	
	memset(&ADPara, 0x00, sizeof(ADPara)); // 初始化成已知量(强烈建议)
	
	// 预置硬件参数
	ADPara.bChannelArray[0]		= TRUE; // 允许通道0采样
	ADPara.bChannelArray[1]		= TRUE; // 允许通道1采样
	ADPara.bChannelArray[2]		= TRUE; // 允许通道2采样
	ADPara.bChannelArray[3]		= TRUE; // 允许通道3采样
	
	ADPara.InputRange[0]		= InputRange;  // 模拟通道0输入量程
	ADPara.InputRange[1]		= InputRange;  // 模拟通道1输入量程
	ADPara.InputRange[2]		= InputRange;  // 模拟通道2输入量程
	ADPara.InputRange[3]		= InputRange;  // 模拟通道3输入量程
	
	ADPara.Frequency		= 10000; // 采样频率(Hz)
	ADPara.TriggerMode		= PCI9757_TRIGMODE_SOFT;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // 触发源
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // 触发类型为边沿触发	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // 触发方向为负向
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // 触发灵敏度	
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // 时钟源选用板内时钟源
	
	if(!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // 初始化硬件
	{
		printf("PCI9757_InitDeviceProAD...\n");
		getch();
		return 0;  
	}
	
	
	for(nChannel=0; nChannel<4; nChannel++)
	{
		if(ADPara.bChannelArray[nChannel] == TRUE)
		{
			ChannelCount++;
		}
	}
	
	nReadSizeWords = 4096 - 4096 %ChannelCount; // 将数据长度字转换为双字	
	
	if(!PCI9757_StartDeviceProAD(hDevice)) // 启动设备
	{
		printf("StartDeviceAD Error...\n");
		getch();
	}
	

	while ( !kbhit() )
	{			
		bFirstWait = TRUE;
		PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer, nReadSizeWords, &nRetSizeWords);
	
		for(Index=0; Index<64; Index++)
		{			
			for(nChannel=nTChannel; nChannel<4; nChannel++)
			{
				if(ADPara.bChannelArray[nChannel]  == TRUE) // 如果此通道被采样
				{
					nCurrentChannel = nChannel; // 记录此通道号
					nTChannel = nChannel + 1;
					
					
					if(kbhit()) goto ExitRead0;
					// 将原码高4位屏蔽掉
					ADData = (USHORT)(ADBuffer[Index]&0xFFFF);
					// 将原码转换为电压值
					switch (InputRange)
					{
					case 0: // -10V - +10V
						fVolt = (float)((20000.0/65536) * ADData - 10000.0);
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
					
					//	printf("%d   \t",ADData);
					printf("CH%02d=%6.2f\t", nCurrentChannel, fVolt); // 显示电压值	
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
	PCI9757_ReleaseDeviceProAD( hDevice ); // 释放AD
	PCI9757_ReleaseDevice( hDevice ); // 释放设备对象
	return 0;
}



