// 说明： 本程序演示了如何用DMA方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI9757.h"

int InputRange;
int SelectInputRange(void);

#define SEGMENT_COUNT 64  // 定义缓冲区页段数为32个
#define HALF_SIZE_WORDS 4096 // 定义缓冲区每页段长度为4096点

LONG ADBuffer[SEGMENT_COUNT][HALF_SIZE_WORDS]; // 分配缓冲区链(存储原始数据)
ULONG g_ulSegmentID = 0;

int main(int argc, char* argv[])
{
	HANDLE hDevice; // 设备句柄 
	int DeviceLgcID;
	PCI9757_PARA_AD ADPara; // 硬件参数
	PCI9757_STATUS_DMA DMAStatus; // DMA状态参数
	
	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示	
	int nCurrentChannel = 0, nChannel = 0;
	WORD ADData;
	float fVolt;
	
	DeviceLgcID = 0; // 访问第一个PCI9757卡(假定系统中只有一台PCI9757卡)
	hDevice = PCI9757_CreateDevice(DeviceLgcID); // 创建设备对象
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
	
	ADPara.Frequency		= 500000; // 采样频率(Hz)
	ADPara.TriggerMode		= PCI9757_TRIGMODE_SOFT;
	ADPara.TriggerSource	= PCI9757_TRIGSRC_ATR; // 触发源
	ADPara.TriggerType		= PCI9757_TRIGTYPE_EDGE; // 触发类型为边沿触发	
	ADPara.TriggerDir		= PCI9757_TRIGDIR_NEGATIVE; // 触发方向为负向
	ADPara.TrigLevelVolt	= 2000;
	ADPara.TrigWindow		= 40;    // 触发灵敏度	
	ADPara.ClockSource		= PCI9757_CLOCKSRC_IN; // 时钟源选用板内时钟源
	
	HANDLE hDmaEvent = PCI9757_CreateSystemEvent();
	if(hDmaEvent == INVALID_HANDLE_VALUE)
	{
		printf("Create Dma Event Error\n");
		_getch();
		return 0;
	}

	if(!PCI9757_InitDeviceDmaAD( // 初始化DMA对象
		hDevice, // 设备句柄,必须由CreateDevice函数成功创建
		hDmaEvent, // DMA事件句柄,必须由CreateSystemEvent函数成功创建
		&ADBuffer[0][0], // 用户缓冲区页链首地址
		HALF_SIZE_WORDS, // 每次装入用户缓冲区页段的点数，只能小于或等于HALF_SIZE_WORDS
		SEGMENT_COUNT, // 用户缓冲区页段个数
		HALF_SIZE_WORDS, // 用户缓冲区每页段长度
		&ADPara)) // 初始化设备的硬件参数
	{
		printf("PCI9757_InitDeviceAD Error...\n");
		_getch();
		goto ExitRead1;  
	}
	
	PCI9757_StartDeviceDmaAD(hDevice); // 启动设备
	
	while(!_kbhit())
	{			
		bFirstWait = TRUE; 
		while(TRUE) // 查询当前物理缓冲区数据是否已准备就绪
		{
			if(WaitForSingleObject (hDmaEvent, 250) == WAIT_OBJECT_0) break; // 等待RAM乒乓切换中断事件
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
		
		// 回读DMA的各种状态
		if(!PCI9757_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			printf("Get Device Status Error...\n");	
			goto ExitRead0;
		}		
		
		// 检查用户缓冲区是否有过溢出(可选)
		if(DMAStatus.bBufferOverflow)
		{
			printf("DMA Overflow...\n");
		}
		
		// 根据回读状态，检查所有的缓冲段是否可用，然后依次处理
		for(int i=0; i<SEGMENT_COUNT && DMAStatus.bSegmentSts[g_ulSegmentID]; i++)
		{
			if(DMAStatus.bSegmentSts[g_ulSegmentID])
			{
				ULONG nTChannel = 0;
				for (int Index=0; Index<64; Index++)
				{	
					if(_kbhit()) goto ExitRead0;
					// 将原码高2位屏蔽掉
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
					printf("CH%02d=%6.2f\t", nChannel, fVolt); // 显示电压值
					nChannel++;
					if(nChannel >= 4) 
					{
						nChannel = 0;
						printf("\n"); // 到末通道时换行
					}					
				}			
			// 将刚处理完成的缓冲区页标志复位，通知硬件及驱动该缓冲区页已被处理
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
	PCI9757_ReleaseDeviceDmaAD(hDevice); // 释放AD
	PCI9757_ReleaseSystemEvent(hDmaEvent); // 释放DMA事件句柄
ExitRead1:
	PCI9757_ReleaseDevice(hDevice); // 释放设备对象
	_getch();
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