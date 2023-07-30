#ifndef _PCI8510_DEVICE_
#define _PCI8510_DEVICE_
//#################### AD硬件参数PCI8510_PARA_AD定义 #####################
// 用于AD采样的实际硬件参数
typedef struct _PCI8510_PARA_AD     
{	
	LONG Gains[8];			// 模拟量增益选择阵列，分别控制8个通道
	LONG Frequency;         // 采集频率, 单位为Hz, [1, 500000]
	LONG InputRange;		// 量程选择
	LONG TriggerMode;		// 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerType;		// 触发类型选择(边沿触发/脉冲触发)
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigLevelVolt;		// 触发电平(±10000mV)
	LONG ClockSource;		// 内外时钟选择
	LONG bClockOutput;      // 是否将时钟信号输出,=TRUE:允许输出, =FALSE:禁止输出
	LONG OutTrigSource;		// Trig信号输入/输出源
	LONG OutTrigSourceDir;	// Trig信号输入/输出方向选择，=TRUE:允许输出, =FALSE:允许输入
}PCI8510_PARA_AD,*PPCI8510_PARA_AD;

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的Gains[x]模拟量输入范围所使用的选项
const long PCI8510_GAINS_1MULT			= 0x00; // 1倍增益
const long PCI8510_GAINS_2MULT			= 0x01; // 2倍增益
const long PCI8510_GAINS_5MULT			= 0x02; // 4倍增益
const long PCI8510_GAINS_10MULT			= 0x03; // 8倍增益

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的InputRange模拟量输入范围所使用的选项
const long PCI8510_INPUT_0_P5000mV		= 0x00; // 0～5000mV
const long PCI8510_INPUT_0_P10000mV		= 0x01; // 0～10000mV
const long PCI8510_INPUT_N5000_P5000mV	= 0x02; // ±5000mV
const long PCI8510_INPUT_N10000_P10000mV= 0x03; // ±10000mV

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long PCI8510_TRIGMODE_SOFT		= 0x00; // 软件内触发
const long PCI8510_TRIGMODE_POST		= 0x01; // 硬件后触发

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerSource触发源信号所使用的选项
const long PCI8510_TRIGSRC_ATR			= 0x00; // 选择外部ATR触发源
const long PCI8510_TRIGSRC_DTR			= 0x01; // 选择外部DTR触发源

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的OutTrigSource时钟源所使用的选项
const long PCI8510_OUTTRIGSRC_TRIGGER0	= 0x00; // 选择TRIG0输出
const long PCI8510_OUTTRIGSRC_TRIGGER1	= 0x01; // 选择TRIG1输出
const long PCI8510_OUTTRIGSRC_TRIGGER2	= 0x02; // 选择TRIG2输出
const long PCI8510_OUTTRIGSRC_TRIGGER3	= 0x03; // 选择TRIG3输出
const long PCI8510_OUTTRIGSRC_TRIGGER4	= 0x04; // 选择TRIG4输出
const long PCI8510_OUTTRIGSRC_TRIGGER5	= 0x05; // 选择TRIG5输出
const long PCI8510_OUTTRIGSRC_TRIGGER6	= 0x06; // 选择TRIG6输出
const long PCI8510_OUTTRIGSRC_TRIGGER7	= 0x07; // 选择TRIG7输出
const long PCI8510_OUTCLOCKSRC_CLKIN	= 0x08; // 选择外部输入时钟源

// AD硬件参数PCI8510_PARA_AD中的TriggerType触发类型所使用的选项
const long PCI8510_TRIGTYPE_EDGE		= 0x00; // 边沿触发
const long PCI8510_TRIGTYPE_LEVELVOLT	= 0x01; // 电平触发

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerDir触发方向所使用的选项
const long PCI8510_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long PCI8510_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long PCI8510_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的ClockSource时钟源所使用的选项
const long PCI8510_CLOCKSRC_IN			= 0x00; // 内部时钟
const long PCI8510_CLOCKSRC_OUT			= 0x01; // 外部时钟

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _PCI8510_STATUS_AD     
{
	LONG bNotEmpty;		// 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
	LONG bHalf;			// 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
	LONG bOverflow;		// 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
} PCI8510_STATUS_AD, *PPCI8510_STATUS_AD;

const int MAX_SEGMENT_COUNT = 64;
typedef struct _PCI8510_STATUS_DMA     
{
	LONG iCurSegmentID;								// 当前段缓冲ID,表示DMA正在传输的缓冲区段
	LONG bSegmentSts[MAX_SEGMENT_COUNT];			// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
	LONG bBufferOverflow;							// 返回溢出状态
} PCI8510_STATUS_DMA, *PPCI8510_STATUS_DMA;

//***********************************************************
// CreateFileObject所用的文件操作方式控制字(可通过或指令实现多种方式并操作)
const long PCI8510_modeRead					= 0x0000;	// 只读文件方式
const long PCI8510_modeWrite				= 0x0001;	// 只写文件方式
const long PCI8510_modeReadWrite			= 0x0002;	// 既读又写文件方式
const long PCI8510_modeCreate				= 0x1000;	// 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0
const long PCI8510_typeText					= 0x4000;	// 以文本方式操作文件

//***********************************************************
// 用户函数接口
#ifndef _PCI8510_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 设备对象管理函数 ##############################
	HANDLE DEVAPI FAR PASCAL PCI8510_CreateDevice(int DeviceLgcID = 0);     // 用逻辑号创建设备对象
	int DEVAPI FAR PASCAL PCI8510_GetDeviceCount(HANDLE hDevice); // 取得设备总台数
	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
    BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDevice(HANDLE hDevice); // 仅释放设备对象

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	// AD数据两个函数便能轻松高效地实现高速、连续的数据采集

	// AD程序查询方式函数
	BOOL DEVAPI FAR PASCAL PCI8510_InitDeviceProAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									PPCI8510_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCI8510_StartDeviceProAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8510_ReadDeviceProAD_Npt(		// 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									ULONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									ULONG nReadSizeWords,	// 相对于偏移点后读入的数据长度(字)
									PLONG nRetSizeWords);   // 返回实际读取的长度(字)

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevStatusProAD(		// 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI8510_STATUS_AD pADStatus); // AD的各种信息结构体

	BOOL DEVAPI FAR PASCAL PCI8510_ReadDeviceProAD_Half(	// 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									ULONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									ULONG nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
									PLONG nRetSizeWords);	// 返回实际读取的长度(字)
		
    BOOL DEVAPI FAR PASCAL PCI8510_StopDeviceProAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDeviceProAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建

	// AD直接内存(DMA)方式函数
	BOOL DEVAPI FAR PASCAL PCI8510_InitDeviceDmaAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									HANDLE hDmaEvent,		// DMA事件句柄,它由CreateSystemEvent创建
									ULONG ADBuffer[],		// 用户缓冲区,最好为两维数组
									ULONG nReadSizeWords,	// 每次DMA时,用户从指定缓冲应读取的实际长度(要小于或等于nSegmentSizeWords)
									LONG nSegmentCount,		// 缓冲分段的数量,取值范围为2-128
									ULONG nSegmentSizeWords,// 缓冲区分段的段长(必须等于FIFO半满长度)
									PPCI8510_PARA_AD pADPara);	// 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCI8510_StartDeviceDmaAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevStatusDmaAD(		// 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI8510_STATUS_DMA pDMAStatus); // 获得的DMA工作状态

	BOOL DEVAPI FAR PASCAL PCI8510_SetDevStatusDmaAD(		// 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									LONG iClrBufferID);		// 要清除的缓冲区ID,将其置为0
		
    BOOL DEVAPI FAR PASCAL PCI8510_StopDeviceDmaAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseDeviceDmaAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8510_ADCalibration(			// 设备校准函数
									HANDLE hDevice);		// 设备对象句柄
		
	//##################### AD的硬件参数操作函数 ###########################
	BOOL DEVAPI FAR PASCAL PCI8510_SaveParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
    BOOL DEVAPI FAR PASCAL PCI8510_LoadParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // 将AD采样参数从系统中读出
    BOOL DEVAPI FAR PASCAL PCI8510_ResetParaAD(HANDLE hDevice, PPCI8510_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//####################### 数字I/O输入输出函数 #################################
	// 用户可以使用WriteRegisterULong和ReadRegisterULong等函数直接控制寄存器进行I/O
	// 输入输出，但使用下面两个函数更省事，它不需要您关心寄存器分配和位操作等，而只
	// 需象VB等语言的属性操作那么简单地实现各开关量通道的控制。
    BOOL DEVAPI FAR PASCAL PCI8510_SetDeviceDO(				// 输出开关量状态
										HANDLE hDevice,		// 设备句柄								        
										BYTE bDOSts[8]);	// 开关状态

	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceDI(				// 取得开关量状态     
										HANDLE hDevice,		// 设备句柄								        
										BYTE bDISts[8]);	// 开关状态	
	
	BOOL DEVAPI FAR PASCAL PCI8510_RetDeviceDO(				// 回读开关量状态
										HANDLE hDevice,		// 设备句柄	 
										BYTE bDOSts[8]);	// 开关状态

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	BOOL DEVAPI FAR PASCAL PCI8510_GetDeviceBar(			// 取得指定的指定设备寄存器组BAR地址
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									__int64  pbPCIBar[6]);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书

	BOOL DEVAPI FAR PASCAL PCI8510_GetDevVersion(			// 获取设备固件及程序版本
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									PULONG pulFmwVersion,	// 固件版本
									PULONG pulDriverVersion);// 驱动版本

    BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterByte(		// 往指定寄存器空间位置写入单节字数据
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									__int64 pbLinearAddr,	// 指定寄存器的线性基地址,它等于GetDeviceAddr中的LinearAddr参数返回值
									ULONG OffsetBytes,		// 相对于线性基地址基地址的偏移位置(字节)
									BYTE Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）

	BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterWord(		// 写双字节数据（其余同上）
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes,  
									WORD Value);

	BOOL DEVAPI FAR PASCAL PCI8510_WriteRegisterULong(		// 写四节字数据（其余同上）
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes,  
									ULONG Value);

	BYTE DEVAPI FAR PASCAL PCI8510_ReadRegisterByte(		// 读入单字节数据（其余同上）
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	WORD DEVAPI FAR PASCAL PCI8510_ReadRegisterWord(		// 读入双字节数据（其余同上）
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	ULONG DEVAPI FAR PASCAL PCI8510_ReadRegisterULong(		// 读入四字节数据（其余同上）
									HANDLE hDevice, 
									__int64 pbLinearAddr, 
									ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortByte(			// 8位写端口
									HANDLE hDevice,			// 设备句柄(由CreateDevice创建)
									__int64 pbPort,			// 指定寄存器的物理基地址
									ULONG OffsetBytes,		// 相对于物理基地址的偏移位置(字节)
									BYTE Value);			// 写出的8位整型数据
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortWord(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI8510_WritePortULong(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI8510_ReadPortByte(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
    WORD DEVAPI FAR PASCAL PCI8510_ReadPortWord(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
    ULONG DEVAPI FAR PASCAL PCI8510_ReadPortULong(HANDLE hDevice, __int64 pbPort, ULONG OffsetBytes);
	// 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数

	//########################### 线程操作函数 ######################################
	HANDLE DEVAPI FAR PASCAL PCI8510_CreateSystemEvent(void); // 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
	BOOL DEVAPI FAR PASCAL PCI8510_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象
#ifdef __cplusplus
}
#endif

//***********************************************************
// 各种FIFO存储器的长度(点)
const long FIFO_IDT7202_LENGTH			= 1024;
const long FIFO_IDT7203_LENGTH			= 2048;
const long FIFO_IDT7204_LENGTH			= 4096;
const long FIFO_IDT7205_LENGTH			= 8192;
const long FIFO_IDT7206_LENGTH			= 16384;
const long FIFO_IDT7207_LENGTH			= 32768;

//***********************************************************
// 设备地址获取函数GetDeviceAddr的参数RegisterID所使用的选项(有效部分)
const long PCI8510_REG_MEM_PLXCHIP		= 0x00; // 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
const long PCI8510_REG_IO_PLXCHIP		= 0x01; // 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)

// 自动包含驱动函数导入库
#ifndef _PCI8510_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "PCI8510_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8510_32.dll...")
#pragma message("======== Successfully linked with PCI8510_32.dll")
#else
#pragma comment(lib, "PCI8510_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8510_64.dll...")
#pragma message("======== Successfully linked with PCI8510_64.dll")
#endif	
#endif

#endif // _PCI8510_DEVICE_