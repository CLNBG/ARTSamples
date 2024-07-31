// 说明： 本程序演示了如何用程序查询方式读取AD数据

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "ACTS1000.h"


#define AD_DATA_LEN 512*1024 // 要读取和处理的AD数据长度（点或字）
USHORT ADBuffer[AD_DATA_LEN]; // 分配缓冲区(存储原始数据)
LONG ADCallBackFunc(HANDLE hDevice, ULONG nSamplesPerChan, ULONG nChannelCount, void *callbackData)
{
	
	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	if (nSamplesPerChan>(AD_DATA_LEN/nChannelCount))
	{
		nSamplesPerChan = AD_DATA_LEN/nChannelCount;
	}

	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	if(!ACTS1000_ReadDeviceAD(hDevice, ADBuffer, nSamplesPerChan*nChannelCount, &nRetSizeWords, &nAvailSampsPoints,0.1))
	{
		printf("ACTS1000_ReadDeviceAD error...\n");
		_getch();
		return 2;
	}
	if (nRetSizeWords!=(nSamplesPerChan*nChannelCount))
	{
		printf("nRetSizeWords error...\n");
		_getch();
		return 3;
	}
	printf("ADBuff[0]=0x%X\n", ADBuffer[0]);
	return 0;

}
int main(int argc, char* argv[])
{
	int DeviceLgcID;
	ULONG ulDDR2Length = 0;
	ULONGLONG ulDDR2LengthByte = 0;

	ACTS1000_PARA_AD ADPara; // 硬件参数

	int nCurrentChannel = 0,nADChannel = 0;
	ACTS1000_AD_MAIN_INFO ADMainInfo;
	WORD wMaxLSB = 0x3FFF;
	float fLsbCount = 16384.0;
	ULONG nSamplesPerChan = 0;
	HANDLE hDevice;
	int InputRange;



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

	wMaxLSB = (WORD)(ADMainInfo.nSampCodeCount - 1);
	fLsbCount = (float)ADMainInfo.nSampCodeCount;

	InputRange = ACTS1000_INPUT_N1000_P1000mV; // 要求用户从键盘上选择输入量程




	ULONG nRevisionID = 0;
	if(!ACTS1000_GetBusInfo(hDevice, NULL, NULL, NULL, &nRevisionID))
	{
		printf("ACTS1000_GetBusInfo error...\n");
	}

	memset(&ADPara, 0x00, sizeof(ADPara)); // 将各项参数复位至确定值0(强烈建议)

	// 预置硬件参数
	ADPara.SampleMode			= ACTS1000_SAMPMODE_CONTINUOUS; // 有限点采样
	ADPara.FreqDivision			= 1000;	
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
		ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
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
	ULONG nDMAMemSize = 0;
	if (!ACTS1000_GetDMAMemorySize(hDevice, &nDMAMemSize))
	{
		ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
		printf("ACTS1000_GetDMAMemorySize error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}
	nDMAMemSize *= 1024;

	nSamplesPerChan = nDMAMemSize/2/4/ChannelCount;// 一个点占两个字节，最大内存的4分之1做为N采样长度
	printf("nNSampleLen = %u\n", nSamplesPerChan);

	if (!ACTS1000_RegisterEveryNSamplesEvent(hDevice, nSamplesPerChan, 0, &ADCallBackFunc, NULL))
	{
		ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
		printf("ACTS1000_GetDMAMemorySize error...\n");
		_getch();
		return 0; // 如果创建设备对象失败，则返回
	}

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

	while ( !_kbhit() )                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	{			
		Sleep(1);
	}

	ACTS1000_StopDeviceAD(hDevice); // 停止AD
	ACTS1000_ReleaseDeviceAD(hDevice); // 释放AD
	ACTS1000_ReleaseDevice(hDevice); // 释放设备对象
	return 0;
}

