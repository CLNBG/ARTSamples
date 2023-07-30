#include "windows.h"
#include <PCI8510.H> 
#include <stdio.h>  				 
							  
static int (__stdcall *getch)(void);  
static int (__stdcall *kbhit)(void); 	 


#include "PCI8510.h"

int InputRange;
int SelectInputRange(void);

#define MAX_AD_CHANNELS 8 // 定义最大通道数
#define SEGMENT_COUNT 32
#define HALF_SIZE_WORDS 4096

ULONG ADBuffer[SEGMENT_COUNT][HALF_SIZE_WORDS]; // 分配缓冲区(存储原始数据)
ULONG g_ulSegmentID = 0;

int main (int argc, char *argv[]) 
{
	

	HANDLE hDevice;
	int DeviceLgcID;

	PCI8510_PARA_AD ADPara; // 硬件参数
	PCI8510_STATUS_DMA DMAStatus; // DMA状态参数

	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示

	int nChannel = 0;
	float fVolt;		
	HANDLE hDmaEvent;
	int i;
	int Index;
	
	HINSTANCE hInst = LoadLibrary("MSVCRT.DLL"); 
	getch = GetProcAddress(hInst, "_getch");   
	kbhit = GetProcAddress(hInst, "_kbhit");   

	DeviceLgcID = 0;
	hDevice = PCI8510_CreateDevice(DeviceLgcID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程

	memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)

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
	
	 hDmaEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!PCI8510_InitDeviceDmaAD( hDevice, hDmaEvent, &ADBuffer[0][0], 4096, SEGMENT_COUNT, HALF_SIZE_WORDS, &ADPara)) // 初始化硬件
	{
		printf("InitDeviceDmaAD error...\n");
		getch();
		goto ExitRead1;  
	}

 	PCI8510_StartDeviceDmaAD(hDevice); // 启动设备
  	while(!kbhit())
  	{			
		bFirstWait = TRUE; 
		while(TRUE) // 查询当前物理缓冲区数据是否已准备就绪
		{
			if(WaitForSingleObject (hDmaEvent, 100)==WAIT_OBJECT_0) break; // 等待DMA事件
			else
			{
				if(bFirstWait)
				{
					printf("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
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
			// Sleep(3000); // 当溢出时,为了让用户看得清楚,延时3秒
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
						// 将原码转换为电压值
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
						printf("CH%02d=%6.2f\t", nChannel, fVolt); // 显示电压值
						if ((nChannel+1)%4 == 0)
						{
							printf("\n"); // 显示电压值
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
	PCI8510_ReleaseDeviceDmaAD(hDevice); // 释放AD
	PCI8510_ReleaseSystemEvent(hDmaEvent);
ExitRead1:
	PCI8510_ReleaseDevice(hDevice); // 释放设备对象
	getch();
	
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
