// 说明： 本程序演示了如何用程序查询方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI9757.h"

int InputRange;
int SelectInputRange(void);


#define MAX_AD_CHANNELS 4 // 定义最大通道数
#define AD_DATA_LEN 1024*4 // 要读取和处理的AD数据长度（点或字）
LONG ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)


int main(int argc, char* argv[])
{
	PCI9757_STATUS_AD ADStatus;
	HANDLE hDevice;
	int DeviceID;
	PCI9757_PARA_AD ADPara; // 硬件参数
	LONG nReadSizeWords;   // 每次读取AD数据的长度(字)
	LONG nRetSizeWords;
	int Index;
	
	int nCurrentChannel = 0, nChannel = 0;
	WORD ADData;
	float fVolt;
	
	DeviceID = 0;
	hDevice = PCI9757_CreateDevice(DeviceID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("Create Error...\n");
		_getch();
		return 0;
	}
	
	InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程
	
	memset(&ADPara, 0x00, sizeof(ADPara)); // 初始化成已知量(强烈建议)
	
	ADPara.bChannelArray[0]		= TRUE; // 允许通道0采样
	ADPara.bChannelArray[1]		= TRUE; // 允许通道1采样
	ADPara.bChannelArray[2]		= TRUE; // 允许通道2采样
	ADPara.bChannelArray[3]		= TRUE; // 允许通道3采样
	
	ADPara.InputRange[0]		= InputRange;  // 模拟通道0输入量程
	ADPara.InputRange[1]		= InputRange;  // 模拟通道1输入量程
	ADPara.InputRange[2]		= InputRange;  // 模拟通道2输入量程
	ADPara.InputRange[3]		= InputRange;  // 模拟通道3输入量程
	
	ADPara.Frequency		= 10000; // 采样频率(Hz)
	ADPara.TriggerMode		= 0;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // 触发源
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // 触发类型为边沿触发	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // 触发方向为负向
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // 触发灵敏度
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // 时钟源选用板内时钟源
	
	if(!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // 初始化硬件
	{
		printf("PCI9757_InitDeviceProAD...\n");
		_getch();
		return 0;  
	}
	
	nReadSizeWords = 4096; // 将数据长度字转换为双字	
	
	if(!PCI9757_StartDeviceProAD(hDevice)) // 启动设备
	{
		printf("StartDeviceAD Error...\n");
		_getch();
	}
		
	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示
	while ( !_kbhit() )
	{			
		bFirstWait = TRUE; 
		while(TRUE) // 查询当前物理缓冲区数据是否已准备就绪
		{
			if(!PCI9757_GetDevStatusProAD(hDevice, &ADStatus))
			{
				printf("Get Device Status Error...\n");
				goto ExitRead0;
			}
			
			if(ADStatus.bHalf)
			{
				break;  // 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据
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
		
		ULONG nTChannel = 0;
		for(Index=0; Index<64; Index++)
		{					
			if(_kbhit()) goto ExitRead0;
			// 将原码高2位屏蔽掉
			ADData = (WORD)((ADBuffer[Index])&0xfFFF);
			switch (InputRange)
			{
			case PCI9757_INPUT_N10000_P10000mV: // -10V - +10V
				fVolt = (float)((20000.0/65536) * ADData-10000.0);
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
			printf("CH%02d=%6.2f\t", nTChannel, fVolt); // 显示电压值
			nTChannel++;
			if(nTChannel >= 4) 
			{
				nTChannel = 0;
				printf("\n"); // 到末通道时换行
			}
			
		}
	}
	
	
ExitRead0:
	PCI9757_ReleaseDeviceProAD( hDevice ); // 释放AD
	PCI9757_ReleaseDevice( hDevice ); // 释放设备对象
	return 0;
}


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
	scanf_s("%d", &InputRange);
	if(InputRange<0 || InputRange>4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return InputRange;
}
