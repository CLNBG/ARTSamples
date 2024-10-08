// 说明： 本程序演示了如何用程序查询方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "PCI8501/PCI8501.h"

int InputRange;
int SelectInputRange(void);

#define AD_DATA_LEN 1024 // 要读取和处理的AD数据长度（点或字）
USHORT ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)

int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;

	PCI8501_PARA_AD ADPara; // 硬件参数
	PCI8501_STATUS_AD ADStatus;
	LONG nReadSizeWords;   // 每次读取AD数据的长度(字)
	LONG nRetSizeWords;
	LONG nReadSize;

	int nCurrentChannel = 0, nADChannel = 0;
	WORD ADData;
	float fVolt;

	DeviceLgcID = 0;
	hDevice = PCI8501_CreateDevice(DeviceLgcID); // 创建设备对象
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	PCI8501_GetDDR2Length(hDevice, &ulDDR2Length);

	InputRange = SelectInputRange(); // 要求用户从键盘上选择输入量程

	memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)

	// 预置硬件参数
	ADPara.Frequency = 100000;				// 采样频率(Hz)
	for (int iChannel = 0; iChannel < 8; iChannel++)
	{
		ADPara.InputRange[iChannel] = InputRange;			// 模拟量输入量程范围
		ADPara.Gains[iChannel] = PCI8501_GAINS_1MULT;
	}
	ADPara.M_Length = 0;						// M长度
	ADPara.N_Length = 1024;				// N长度
	ADPara.TriggerMode = PCI8501_TRIGMODE_MIDL;	// 硬件中间触发(包括预触发、后触发功能)
	ADPara.TriggerSource = PCI8501_TRIGMODE_SOFT;	// 软件触发
	ADPara.TriggerDir = PCI8501_TRIGDIR_NEGATIVE; // 下降沿触发
	ADPara.TrigLevelVolt = 0;
	ADPara.ClockSource = PCI8501_CLOCKSRC_IN;		// 使用内部时钟
	ADPara.bClockSourceDir = FALSE;
	ADPara.OutClockSource = PCI8501_OUTCLOCKSRC_TRIGGER0;

	if (!PCI8501_InitDeviceAD(hDevice, &ADPara)) // 初始化硬件
	{
		printf("InitDeviceAD error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8; // M加N长度为读取数据的长度

	ULONG ulStartAddr = 0;
	BOOL bFirstWait = TRUE; // 为每次等待只显示一次提示
	while (!_kbhit())
	{
		if (!PCI8501_StartDeviceAD(hDevice)) // 启动设备
		{
			printf("StartDeviceAD Error...\n");
			_getch();
		}
		if (!PCI8501_SetDeviceTrigAD(hDevice)) // 触发AD
		{
			printf("SetDeviceTrigAD Error...\n");
			_getch();
		}
		ulStartAddr = 0;
		nReadSize = nReadSizeWords;
		bFirstWait = TRUE;
		while (TRUE) // 查询当前物理缓冲区数据是否已准备就绪
		{
			if (!PCI8501_GetDevStatusAD(hDevice, &ADStatus))
			{
				printf("GetDevStatusAD error...\n");
				_getch();
				goto ExitRead;
			}

			if (ADStatus.bComplete)
			{
				break;  // 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据
			}
			else
			{
				if (bFirstWait)
				{
					printf("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n");
					bFirstWait = FALSE;
				}
				if (_kbhit()) goto ExitRead; // 如果用户按键，则退出
			}
		}

		if (!PCI8501_StopDeviceAD(hDevice)) // 启动设备
		{
			printf("StartDeviceAD Error...\n");
			_getch();
		}

		LONG offsetAddr = ADStatus.lEndAddr - nReadSizeWords * sizeof(USHORT) + 4;
		if (offsetAddr < 0)
		{
			ulStartAddr = ulDDR2Length * 1024 * 1024 + (ADStatus.lEndAddr - nReadSizeWords * sizeof(USHORT) + 4);
		}
		else
			ulStartAddr = (ULONG)offsetAddr;

		while (nReadSize > 0)
		{
			if (!PCI8501_ReadDeviceAD(hDevice, ADBuffer, AD_DATA_LEN, ulStartAddr, &nRetSizeWords))
			{
				printf("ReadDeviceDmaAD error...\n");
				_getch();
				goto ExitRead;
			}
			nReadSize = nReadSize - AD_DATA_LEN;
			ulStartAddr = ulStartAddr + AD_DATA_LEN * sizeof(USHORT);

			for (int Index = 0; Index < 64;)
			{
				for (nADChannel = 0; nADChannel < 8; nADChannel++)
				{
					if (_kbhit()) goto ExitRead;

					ADData = ADBuffer[Index] & 0xFFFF;
					// 将原码转换为电压值
					switch (InputRange)
					{
					case PCI8501_INPUT_N10000_P10000mV: // -10000mV - +10000mV
						fVolt = (float)((20000.0 / 65536) * ADData - 10000.0);
						break;
					case PCI8501_INPUT_N5000_P5000mV:	// -5000mV - +5000mV
						fVolt = (float)((10000.0 / 65536) * ADData - 5000.0);
						break;
					case PCI8501_INPUT_N2500_P2500mV:	// -2500mV - +2500mV
						fVolt = (float)((5000.0 / 65536) * ADData - 2500.0);
						break;
					case PCI8501_INPUT_0_P10000mV:	// 0mV - +10000mV
						fVolt = (float)((10000.0 / 65536) * ADData);
						break;
					case PCI8501_INPUT_0_P5000mV:	// 0mV - +5000mV
						fVolt = (float)((5000.0 / 65536) * ADData);
						break;
					default:
						break;
					}

					printf("CH%02d=%6.2f\t", nADChannel, fVolt); // 显示电压值
					Index++;
				}
				printf("\n");
			} // for(Index=0; Index<64; Index++)
		}

	}

ExitRead:
	PCI8501_StopDeviceAD(hDevice); // 停止AD
	PCI8501_ReleaseDeviceAD(hDevice); // 释放AD
	PCI8501_ReleaseDevice(hDevice); // 释放设备对象
	return 0;
}


//////////////////////////////////////////////////////
// 获取用户选择的输入量程
int SelectInputRange(void)
{
	LONG InputRange;
Repeat:
	printf("\n");
	printf("0. -10V ～ +10V\n");
	printf("1. -5V ～ +5V\n");
	printf("2. -2500V ～ +2500V\n");
	printf("3. 0mV ～ +10000mV\n");
	printf("4. 0mV ～ +5000mV\n");

	printf("Please Select Input Range[0-4]:");
	scanf_s("%d", &InputRange);
	if (InputRange < 0 || InputRange>4) goto Repeat; // 判断用户选择的量程是否合法，不合法，则重新选择
	return InputRange;
}
