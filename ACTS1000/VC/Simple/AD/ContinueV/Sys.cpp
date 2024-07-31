// 说明： 本程序演示了如何用程序查询方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"

int InputRange;

#define AD_DATA_LEN 256*1024 // 要读取和处理的AD数据长度（点或字）
float fADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)
int main(int argc, char* argv[])
{
	HANDLE hDevice;
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;
	ULONGLONG ulDDR2LengthByte = 0;

	ACTS1000_PARA_AD ADPara; // 硬件参数
	ACTS1000_STATUS_AD ADStatus;
	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	ULONG nRead;

	int nCurrentChannel = 0,nADChannel = 0;
	ACTS1000_AD_MAIN_INFO ADMainInfo;


	DeviceLgcID = 0;
	printf("请输入逻辑ID:");
	scanf_s("%d", &DeviceLgcID);
	hDevice = ACTS1000_CreateDevice(DeviceLgcID); // 创建设备对象
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		printf("CreateDevice error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	ACTS1000_GetMainInfo(hDevice,&ADMainInfo);			// DDR2的长度(单位：MB)

	switch (ADMainInfo.nDeviceType>>16)
	{
	case 0x2012:
		printf("PXIE%04X\n",ADMainInfo.nDeviceType&0xFFFF);
		break;
	case 0x2111:
		printf("PCIE%04X\n",ADMainInfo.nDeviceType&0xFFFF);
		break;
	default:
		printf("ACTS1000-%04X\n",ADMainInfo.nDeviceType);
	}

	ulDDR2LengthByte = ADMainInfo.nDepthOfMemory;


	InputRange = ACTS1000_INPUT_N1000_P1000mV; // 要求用户从键盘上选择输入量程

	memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)

	// 预置硬件参数
	ADPara.SampleMode			= ACTS1000_SAMPMODE_CONTINUOUS; // 有限点采样
	ADPara.FreqDivision			= 1;	
	for (int nCH=0; nCH<ADMainInfo.nChannelCount; nCH++)
	{
		ADPara.bChannelArray[nCH]		= TRUE; // 允许通道0采样
		ADPara.InputRange[nCH]		= InputRange;			// 模拟量输入量程范围
		ADPara.CouplingType[nCH]		= ACTS1000_COUPLING_DC;	// 直流耦合
	}
	ADPara.M_Length				= 0;					// M长度
	ADPara.N_Length				= 1024*32;			// N长度
	ADPara.PFISel				= ACTS1000_PFISEL_TRIG_IN;
	ADPara.TriggerMode			= ACTS1000_TRIGMODE_POST; // 后触发
	ADPara.TriggerSource		= ACTS1000_TRIGMODE_SOFT;	// 软件触发
	ADPara.TriggerDir			= ACTS1000_TRIGDIR_NEGATIVE; // 下降沿触发
	ADPara.TrigLevelVolt		= 0;
	ADPara.ReferenceClock			= ACTS1000_RECLK_ONBOARD;	// 使用内部时钟
	ADPara.TimeBaseClock			= ACTS1000_TBCLK_IN;	
	ADPara.bMasterEn			= 0;
	ADPara.TrigCount			= 1;
	ADPara.SyncTrigSignal		= ACTS1000_STS_TRIGGER0;
	ADPara.bClkOutEn			= 0;
	ADPara.ClkOutSel			= ACTS1000_CLKOUT_REFERENCE;
	ADPara.bTrigOutEn			= 0;
	ADPara.TrigOutPolarity		= ACTS1000_TOP_POSITIVE;
	ADPara.TrigOutWidth			= 50;
	ADPara.bSaveFile			= 0;
	_stprintf_s(ADPara.chFileName, 256, _T("%s"), L"I:\\3.pci");

	if(!ACTS1000_InitDeviceAD(hDevice, &ADPara)) // 初始化硬件
	{
		printf("InitDeviceAD error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

	int ChannelCount = 0;	
	for(nADChannel=0; nADChannel<ADMainInfo.nChannelCount; nADChannel++)
	{
		if(ADPara.bChannelArray[nADChannel] == TRUE)
		{
			ChannelCount++;
		}
	}

	LONG ulTime = GetTickCount();
	double dResult;
	__int64 i64Cnt=0;
	LONG ulTmp=0;
	LONG ulSecCnt=0;

	if(!ACTS1000_StartDeviceAD(hDevice)) // 启动设备
	{
		printf("StartDeviceAD Error...\n");
		_getch();
	}
	if(!ACTS1000_SetDeviceTrigAD(hDevice)) // 触发AD
	{
		printf("SetDeviceTrigAD Error...\n");
		_getch();
	}

	ulTime = GetTickCount();
	ULONGLONG ulReadCount = 0;
	nRead = AD_DATA_LEN;
	if (ADPara.bSaveFile)	// 自动存盘
	{
		while(TRUE) 
		{
			if(!ACTS1000_GetDevStatusAD(hDevice, &ADStatus))
			{
				printf("GetDevStatusAD Error\n");
				goto ExitRead;
			}			
			if(_kbhit())
				goto ExitRead; // 如果用户按键，则退出
			Sleep(1);

			ulTmp = GetTickCount()-ulTime;
			if (ulTmp/1000>ulSecCnt)
			{
				ulSecCnt = ulTmp/1000;
				dResult = ADStatus.lSavePoints*1.00/((ulTmp+1)/1000.0);
				dResult=dResult/1000000.0;
				printf("%dS ",ulSecCnt);
				printf("%.3fM点/秒\n",dResult);
			}
		}
	}
	else
	{

		while ( !_kbhit() )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		{			
			if(!ACTS1000_ReadAnalog(hDevice, fADBuffer, nRead, &nRetSizeWords, &nAvailSampsPoints,5.0))
			{
				printf("ReadDeviceDmaAD error...\n");
				_getch();
				goto ExitRead;
			}
			if (nRetSizeWords<=0)
			{
				continue;
			}
			ulReadCount+=nRetSizeWords;
#if 1
			ulTmp = GetTickCount()-ulTime;
			if (ulTmp/1000>ulSecCnt)
			{
				ulSecCnt = ulTmp/1000;
				dResult = ulReadCount*1.00/((ulTmp+1)/1000.0);
				dResult=dResult/1000000.0;
				printf("%dS ",ulSecCnt);
				printf("%.3fM点/秒\n",dResult);
				ULONG nTChannel = 0;
				for(int Index=0; Index<8; Index++)
				{			
					for(nADChannel=nTChannel; nADChannel<ChannelCount; nADChannel++)
					{
						if(ADPara.bChannelArray[nADChannel]  == TRUE) // 如果此通道被采样
						{

							nTChannel = nADChannel + 1;
							printf("CH%02d=%6.2f\t", nADChannel, fADBuffer[Index]); // 显示电压值
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
#endif

		}
	}

ExitRead:
	ACTS1000_StopDeviceAD(hDevice); // 停止AD
	ACTS1000_ReleaseDeviceAD(hDevice); // 释放AD
	ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
	return 0;
}

