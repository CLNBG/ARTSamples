#ifndef _ACTS1000_DEVICE_
#define _ACTS1000_DEVICE_

#define ACTS1000_AD_MAX_CHANNELS	8 // 本程序最多支持8路模拟量输入通道的板卡
//######################## 支持板卡 #################################
// PXIE/PCIE 8502 4CH 12Bit 40Msps
// PXIE/PCIE 8504 4CH 14Bit 40Msps
// PXIE/PCIE 8512 4CH 12Bit 80Msps
// PXIE/PCIE 8514 4CH 14Bit 80Msps
// PXIE/PCIE 8912 2CH 12Bit 250Msps,支持阻抗选择
// PXIE/PCIE 8914 2CH 14Bit 250Msps,支持阻抗选择
// PXIE/PCIE 8916 2CH 16Bit 250Msps,支持阻抗选择
// PXIE/PCIE 8582 8CH 12Bit 100Msps,PFI选择(触发输出/触发输入),不支持外时钟,不支持时钟输出
// PXIE/PCIE 8584 8CH 14Bit 100Msps,PFI选择(触发输出/触发输入),不支持外时钟,不支持时钟输出
// PXIE/PCIE 8586 8CH 16Bit 100Msps,PFI选择(触发输出/触发输入),不支持外时钟,不支持时钟输出

//***********************************************************
// 用于AD采样的实际硬件参数
typedef struct _ACTS1000_PARA_AD     
{
	LONG bChannelArray[8];	// 采样通道选择阵列,分别控制8个通道,=TRUE表示该通道采样,否则不采样(只支持3种通道配置:0 01 0123)
	LONG InputRange[8];		// 模拟量输入量程选择
	LONG CouplingType[8];	// 耦合类型(直流耦合, 交流耦合)
	LONG InputImped[8];		// 输入阻抗控制阵列，分别控制8个通道(仅板卡8912/8914/8916支持)
	LONG FreqDivision;      // 分频数[1, 2147483647],外时钟:采样频率=外部时钟频率/分频数;其它时钟:采样频率=基准频率/分频数;
	LONG SampleMode;		// 采样模式(有限采样,连续采样)
	ULONG M_Length;         // M段长度(点),延时触发有效
	ULONG N_Length;         // N段长度(点),有限点采样有效
							// 中间触发M+N为有效读取点数,范围[512/使能通道数 , 内存大小(字节)/2/使能通道数],且为最小值整数倍;
							// 后触发M必须为0,N*使能通道数*触发次数必须为512的整数倍
							// 预触发M必须为0,N为有效读取点数,范围[512/使能通道数 , 内存大小(字节)/2/使能通道数],且为最小值整数倍;
							// 硬件延时,M表示延时点数范围[0, 4294967295],N*使能通道数*触发次数必须为512的整数倍
							
	LONG PFISel;			// PFI功能选择, 详见下面常量定义(仅板卡8582 8584支持)
	LONG TriggerMode;		// 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerDir;		// 触发方向选择(下降沿/上升沿/上下边沿均触发)
	LONG TrigLevelVolt;		// 触发电平(量程按模拟输入量程)
	LONG TrigWindow;		// 触发灵敏窗单位nS,步进为本卡最高采样率的采样周期;例如最高80M,步进即为12.5nS
	ULONG TrigCount;		// 触发次数设置,默认为1,为0时表示无限触发,此功能仅在有限采样(后触发和硬件延时触发)模式下有效
	LONG ReferenceClock;	// 参考时钟选择
	LONG TimeBaseClock;		// 采样时基选择
	LONG bMasterEn;			// 主设备使能  
							// =0：从设备,接收主设备发送的同步触发信号
							// =1：主设备,为从设备发送自身的触发信号
							// 注：在多模块同步系统中,只能设定其中一个设备为主设备,其余需设定为从设备,如果系统中只有一个设备或者有多个设备但是不要求同步,需将所有设备设定为从设备
	LONG SyncTrigSignal;	// 同步触发源
	LONG bClkOutEn;			// 时钟是否输出,TRUE输出
	LONG ClkOutSel;			// 时钟输出选择
	LONG bTrigOutEn;		// 触发是否输出,TRUE输出
	LONG TrigOutPolarity;	// 触发输出极性
	LONG TrigOutWidth;		// 触发脉冲输出宽度单位nS,[50, 50000]步进50nS
	BOOL bSaveFile;			// 是否自动存盘 =TRUE:存盘
	TCHAR chFileName[256];	// 存盘路径及名称,例:L"F:\\1.pci"
} ACTS1000_PARA_AD, *PACTS1000_PARA_AD;

//***********************************************************
// AD参数ACTS1000_PARA_AD中的InputRange[X]使用的硬件增益选项
const long ACTS1000_INPUT_N5000_P5000mV		= 0x00; // ±5000mV
const long ACTS1000_INPUT_N1000_P1000mV		= 0x01; // ±1000mV

// AD参数ACTS1000_PARA_AD中的CouplingType耦合类型所使用的选项
const long ACTS1000_COUPLING_DC				= 0x00;	// 直流耦合
const long ACTS1000_COUPLING_AC				= 0x01;	// 交流耦合

// AD参数ACTS1000_PARA_AD中的InputImped使用的输入阻抗选项
const long ACTS1000_IMPED_1M				= 0x00; // 1MΩ
const long ACTS1000_IMPED_50				= 0x01; // 75Ω

// AD参数ACTS1000_PARA_AD中的SampleMode采样模式所使用的选项
const long ACTS1000_SAMPMODE_FINITE			= 0x00;	// 有限点采样
const long ACTS1000_SAMPMODE_CONTINUOUS		= 0x01;	// 连续采样(后触发和硬件延时触发支持连续采样,触发次数不起作用,即触发一次后一直采样)


// AD参数ACTS1000_PARA_AD中的PFISel PFI功能选择所使用的选项(仅8582 8584板卡支持)
const long ACTS1000_PFISEL_TRIG_OUT			= 0x00;	// 触发输出
const long ACTS1000_PFISEL_TRIG_IN			= 0x01; // 触发输入

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long ACTS1000_TRIGMODE_MIDL			= 0x00; // 中间触发:M为触发前采集点数,N为触发后采集点数
const long ACTS1000_TRIGMODE_POST			= 0x01; // 后触发:M无效,N为触发后采集点数
const long ACTS1000_TRIGMODE_PRE			= 0x02; // 预触发:M无效,N为触发前采集点数
const long ACTS1000_TRIGMODE_DELAY			= 0x03; // 硬件延时触发:M为触发后延时点数,N为延时后采集点数

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的TriggerSource触发源信号所使用的选项
const long ACTS1000_TRIGMODE_SOFT			= 0x00; // 软件触发(缺省值)
const long ACTS1000_TRIGSRC_DTR				= 0x01; // 外部数字触发(DTR/PFI)
const long ACTS1000_TRIGSRC_TRIGGER			= 0x02; // 同步信号触发(用于多卡同步,数字信号,直接捕捉边沿)
const long ACTS1000_TRIGSRC_CH0				= 0x03; // 通道0触发
const long ACTS1000_TRIGSRC_CH1				= 0x04; // 通道1触发
const long ACTS1000_TRIGSRC_CH2				= 0x05; // 通道2触发
const long ACTS1000_TRIGSRC_CH3				= 0x06; // 通道3触发
const long ACTS1000_TRIGSRC_CH4				= 0x07; // 通道4触发
const long ACTS1000_TRIGSRC_CH5				= 0x08; // 通道5触发
const long ACTS1000_TRIGSRC_CH6				= 0x09; // 通道6触发
const long ACTS1000_TRIGSRC_CH7				= 0x0A; // 通道7触发

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的TriggerDir触发方向所使用的选项
const long ACTS1000_TRIGDIR_NEGATIVE		= 0x00; // 下降沿触发
const long ACTS1000_TRIGDIR_POSITIVE		= 0x01; // 上升沿触发
const long ACTS1000_TRIGDIR_NEGAT_POSIT		= 0x02; // 上下边沿均触发

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的ReferenceClock参考时钟选择所使用的选项
const long ACTS1000_RECLK_ONBOARD			= 0x00; // 板载时钟(默认)
const long ACTS1000_RECLK_EXT_10M			= 0x01;	// 外部10M（CLK_IN）(板卡8582 8584不支持)
const long ACTS1000_RECLK_PXI_10M			= 0x02;	// PXIe_CLK10M(PXIE)/主卡10M(PCIE)
const long ACTS1000_RECLK_PXIe_100M			= 0x03; // PXIe_CLK100M(PCIE总线板卡不支持)

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的TimeBaseClock采样时基选择所使用的选项
const long ACTS1000_TBCLK_IN				= 0x00; // 内部时钟(默认)
const long ACTS1000_RECLK_EXT				= 0x01;	// 外部时钟（CLK_IN）(板卡8582 8584不支持)

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的SyncTrigSignal同步触发源所使用的选项
const long ACTS1000_STS_TRIGGER0			= 0x00; // 同步触发信号TRIG0 
const long ACTS1000_STS_TRIGGER1			= 0x01; // 同步触发信号TRIG1 
const long ACTS1000_STS_TRIGGER2			= 0x02; // 同步触发信号TRIG2 
const long ACTS1000_STS_TRIGGER3			= 0x03; // 同步触发信号TRIG3 
const long ACTS1000_STS_TRIGGER4			= 0x04; // 同步触发信号TRIG4 
const long ACTS1000_STS_TRIGGER5			= 0x05; // 同步触发信号TRIG5 
const long ACTS1000_STS_TRIGGER6			= 0x06; // 同步触发信号TRIG6 
const long ACTS1000_STS_TRIGGER7			= 0x07; // 同步触发信号TRIG7 

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的ClkOutSel时钟输出选择所使用的选项
const long ACTS1000_CLKOUT_REFERENCE		= 0x00; // 输出参考时钟
const long ACTS1000_CLKOUT_TIMEBASE			= 0x01; // 输出基准时钟 

//***********************************************************
// AD硬件参数ACTS1000_PARA_AD中的TrigOutPolarity时钟输出选择所使用的选项
const long ACTS1000_TOP_NEGATIVE			= 0x00; // 负脉冲输出
const long ACTS1000_TOP_POSITIVE			= 0x01; // 正脉冲输出 



//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _ACTS1000_STATUS_AD     
{
	LONG bADEanble;		// AD是否已经使能,=TRUE:表示已使能,=FALSE:表示未使能
	LONG bTrigger;		// AD是否被触发,=TRUE:表示已被触发,=FALSE:表示未被触发
	LONG bComplete;		// 自动存盘单次有限点任务是否完成,=TRUE:表示已完成,=FALSE:表示未完成(有限采样有效)
	LONG bAheadTrig;	// AD是否溢出,=TRUE:表示已溢出,=FALSE:表示未溢出
	ULONGLONG lCanReadPoint;	// 可以读取的点数(有限采样有效)
	ULONGLONG lSavePoints;// 自动存盘大小(点)
} ACTS1000_STATUS_AD, *PACTS1000_STATUS_AD;


// #################### AD主要信息结构体(ACTS1000_AD_MAIN_INFO) #######################
typedef struct _ACTS1000_AD_MAIN_INFO
{
	LONG nDeviceType;			// 总线类型\设备类型0x20128514 20118512...2012(PXIE)2011(PCIE)
	LONG nChannelCount;			// AD通道数量
	LONG nDepthOfMemory;		// AD板载存储器深度(MB)
	LONG nSampResolution;		// AD采样分辨率(如=8表示8Bit; =12表示12Bit; =14表示14Bit; =16表示16Bit)
	LONG nSampCodeCount;		// AD采样编码数量(如256, 4096, 16384, 65536)
	LONG nTrigLvlResolution;	// 触发电平分辨率(如=8表示8Bit; =12表示12Bit; =16表示16Bit)
	LONG nTrigLvlCodeCount;		// 触发电平编码数量(如256, 4096)
	LONG nBaseRate;				// 基准频率Hz
	LONG nMaxRate;				// 最大频率Hz
	LONG nMinFreqDivision;		// 最小分频数
	LONG nSupportImped;			// 是否支持输入阻抗(0:不支持 1:支持)
	LONG nSupportPFI;			// 是否支持PFI触发输入输出选择(0:不支持 1:支持)
	LONG nSupportExtClk;		// 是否支持外时钟(0:不支持 1:支持)
	LONG nSupportPXIE100M;		// 是否支持PXIE100M(0:不支持 1:支持)
	LONG nSupportClkOut;		// 是否支持时钟输出(0:不支持 1:支持)

	LONG nReserved0;			// 保留字段(暂未定义)
	LONG nReserved1;			// 保留字段(暂未定义)
	LONG nReserved2;			// 保留字段(暂未定义)
	LONG nReserved3;			// 保留字段(暂未定义)
} ACTS1000_AD_MAIN_INFO, *PACTS1000_AD_MAIN_INFO;

//***********************************************************
// 用户函数接口
#ifndef _ACTS1000_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 常规通用函数 #################################
	// 适用于本设备的最基本操作
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateDevice(int DeviceID);		// 创建设备对象
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateDeviceEx(LONG DevicePhysID);		// 创建设备对象(该函数使用物理ID最大255)
	LONG DEVAPI FAR PASCAL ACTS1000_GetDeviceCount(HANDLE hDevice);			// 取得设备总台数
	BOOL DEVAPI FAR PASCAL ACTS1000_SetDevicePhysID(HANDLE hDevice, LONG DevicePhysID); // 设置当前设备的物理ID号[0:255]
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID); // 取得当前设备的逻辑ID号和物理ID号
	BOOL DEVAPI FAR PASCAL ACTS1000_ListDeviceDlg(HANDLE hDevice);			// 列表系统当中的所有的该PCI设备
    BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseDevice(HANDLE hDevice);			// 关闭设备,禁止传输,且释放资源

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户,这些接口最简单、最快捷、最可靠,让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程,仅用下面的初始化设备和读取
	// AD数据几个函数便能轻松高效地实现高速、连续的数据采集
	BOOL DEVAPI FAR PASCAL ACTS1000_GetMainInfo(								// 返回板载DDR2大小,单位为Mb
												HANDLE hDevice,					// 设备对象
												PACTS1000_AD_MAIN_INFO pMainInfo);		// 返回板卡信息
	BOOL DEVAPI FAR PASCAL ACTS1000_ADCalibration(								// AD校准(注意:仅支持输入阻抗为1M欧校准)
												HANDLE hDevice);				// 设备对象				
    BOOL DEVAPI FAR PASCAL ACTS1000_InitDeviceAD(								// 初始化设备,当返回TRUE后,设备即刻开始传输.
												HANDLE hDevice,					// 设备对象
												PACTS1000_PARA_AD pADPara);		// 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL ACTS1000_StartDeviceAD(								// 在初始化之后,启动设备
												HANDLE hDevice);				// 设备对象句柄

	BOOL DEVAPI FAR PASCAL ACTS1000_SetDeviceTrigAD(								// 当设备使能允许后,产生软件触发事件(只有触发源为软件触发时有效)
												HANDLE hDevice);				// 设备对象句柄
	
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDevStatusAD(
												HANDLE hDevice,					// 设备句柄
												PACTS1000_STATUS_AD pADStatus);	// AD状态

	BOOL DEVAPI FAR PASCAL ACTS1000_ReadDeviceAD(								// DMA方式读AD数据
												HANDLE hDevice,					// 设备句柄
												PWORD pADBuffer,				// 将用于接受数据的用户缓冲区(该区必须开辟大于M加N个字的空间)
												ULONG nReadSizeWords,			// 读取长度
												PULONG nRetSizeWords,			// 返回实际读取的数据长度
												PULONG pAvailSampsPoints,		// 剩余点数
												double fTimeout);				// 超时时间
								
    BOOL DEVAPI FAR PASCAL ACTS1000_StopDeviceAD(								// 在启动设备之后,暂停设备
												HANDLE hDevice);				// 设备对象句柄

	BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseDeviceAD(								// 关闭AD设备,禁止传输,且释放资源
												HANDLE hDevice);				// 设备句柄

	PVOID DEVAPI FAR PASCAL ACTS1000_AllocateBuffer(							// 申请内存
												ULONG ulRequsetSize);			// 申请内存大小，单位字节

	BOOL DEVAPI FAR PASCAL ACTS1000_FreeBuffer(									// 释放申请的内存
												PVOID pData);

	BOOL DEVAPI FAR PASCAL ACTS1000_SetDMAMemorySize(							// 设置DMA内存大小
												HANDLE hDevice,					// 设备句柄
												ULONG DMAMemSize);				// DMA内存大小,单位KB,[512,131072],默认4096KB
	//################# AD的硬件参数操作函数 ########################
	BOOL DEVAPI FAR PASCAL ACTS1000_SaveParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);  
    BOOL DEVAPI FAR PASCAL ACTS1000_LoadParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);
	BOOL DEVAPI FAR PASCAL ACTS1000_ResetParaAD(HANDLE hDevice, PACTS1000_PARA_AD pADPara);

	// ################################ 密码函数 ################################
	BOOL DEVAPI FAR PASCAL ACTS1000_SetPassword(ULONG nPassword);
	// ################################ 设备序列号函数 ################################
	BOOL DEVAPI FAR PASCAL ACTS1000_SetSerialNum(						// 设置序列号, 成功时返回TRUE,否则返回FALSE(ACTS1000_SetPassword成功后才可设置)
												HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
												ULONG nSerialNum);		// 序列号

	BOOL DEVAPI FAR PASCAL ACTS1000_GetSerialNum(						// 获得序列号, 成功时返回TRUE,否则返回FALSE
												HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
												ULONG* pSerialNum);		// 返回的序列号

	BOOL DEVAPI FAR PASCAL ACTS1000_GetBusInfo(							// 获得设备总线信息
												HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
												ULONG* pBusNumber,		// 返回总线号
												ULONG* pFunctionNumber,	// 返回功能号
												ULONG* pDeviceNumber);	// 返回设备号

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时,则用户可以使用这些接口予以实现。
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDeviceBar(					// 取得指定的指定设备寄存器组BAR地址
											HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
											__int64 pbPCIBar[6]);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书
	BOOL DEVAPI FAR PASCAL ACTS1000_GetDevVersion(					// 获取设备固件及程序版本
											HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
											PULONG pulFmwVersion,	// 固件版本
											PULONG pulDriverVersion);// 驱动版本
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterByte(				// 往设备的映射寄存器空间指定端口写入单节字数据
											HANDLE hDevice,			// 设备对象
											__int64 pbLinearAddr,		// 指定映射寄存器的线性基地址
											ULONG OffsetBytes,		// 相对于基地址的偏移位置
											BYTE Value);			// 往指定地址写入单字节数据(其地址由线性基地址和偏移位置决定)
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterWord(				// 写双字节数据(其余同上)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL ACTS1000_WriteRegisterULong(				// 写四节字数据(其余同上)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL ACTS1000_ReadRegisterByte(				// 读入单字节数据(其余同上)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL ACTS1000_ReadRegisterWord(				// 读入双字节数据(其余同上)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL ACTS1000_ReadRegisterULong(				// 读入四字节数据(其余同上)
											HANDLE hDevice, 
											__int64 pbLinearAddr, 
											ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时,则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备,如ISA总线、并口、串口等设备,不能用于本PCI设备
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortByte(HANDLE hDevice, __int64 pbPort, BYTE Value);
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortWord(HANDLE hDevice, __int64 pbPort, WORD Value);
    BOOL DEVAPI FAR PASCAL ACTS1000_WritePortULong(HANDLE hDevice, __int64 pbPort, ULONG Value);

    BYTE DEVAPI FAR PASCAL ACTS1000_ReadPortByte(HANDLE hDevice, __int64 pbPort);
    WORD DEVAPI FAR PASCAL ACTS1000_ReadPortWord(HANDLE hDevice, __int64 pbPort);
    ULONG DEVAPI FAR PASCAL ACTS1000_ReadPortULong(HANDLE hDevice, __int64 pbPort);

	//########################### 线程操作函数 ######################################
	HANDLE DEVAPI FAR PASCAL ACTS1000_CreateSystemEvent(void);			// 创建内核事件对象,供InitDeviceInt和VB子线程等函数使用
	BOOL DEVAPI FAR PASCAL ACTS1000_ReleaseSystemEvent(HANDLE hEvent);	// 释放内核事件对象

#ifdef __cplusplus
}
#endif

// 自动包含驱动函数导入库
#ifndef _ACTS1000_DRIVER_

	
#endif

#endif // _ACTS1000_DEVICE_
