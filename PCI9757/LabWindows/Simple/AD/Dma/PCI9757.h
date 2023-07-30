#ifndef _PCI9757_DEVICE_
#define _PCI9757_DEVICE_
//#################### AD硬件参数PCI9757_PARA_AD定义 #####################
// 用于AD采样的实际硬件参数
typedef struct _PCI9757_PARA_AD     
{
	LONG bChannelArray[4];	// 采样通道选择阵列，分别控制4个通道，=TRUE表示该通道采样，否则不采样
	LONG InputRange[4];		// 模拟量输入量程选择阵列，分别控制4个通道
	LONG Frequency;         // 采集频率, 单位为Hz, [3, 800000]
	LONG TriggerMode;		// 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TriggerType;		// 触发类型选择(边沿触发/脉冲触发)
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG TrigLevelVolt;		// 触发电平(0—10000mV)
	LONG TrigWindow;		// 触发灵敏窗[1, 65535], 单位25纳秒
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
} PCI9757_PARA_AD, *PPCI9757_PARA_AD;

//***********************************************************
// AD硬件参数PCI9757_PARA_AD中的InputRange[x]模拟量输入范围所使用的选项
const long PCI9757_INPUT_N10000_P10000mV= 0x00; // ±10000mV
const long PCI9757_INPUT_N5000_P5000mV	= 0x01; // ±5000mV
const long PCI9757_INPUT_N2500_P2500mV	= 0x02; // ±2500mV
const long PCI9757_INPUT_0_P10000mV		= 0x03; // 0～10000mV
const long PCI9757_INPUT_0_P5000mV		= 0x04; // 0～5000mV

//***********************************************************
// AD硬件参数PCI9757_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long PCI9757_TRIGMODE_SOFT		= 0x00; // 软件内触发
const long PCI9757_TRIGMODE_POST		= 0x01; // 硬件后触发

//***********************************************************
// AD硬件参数PCI9757_PARA_AD中的TriggerSource触发源信号所使用的选项
const long PCI9757_TRIGSRC_ATR			= 0x00; // 选择外部ATR触发源
const long PCI9757_TRIGSRC_DTR			= 0x01; // 选择外部DTR触发源
const long PCI9757_TRIGSRC_PCI_TRIG0	= 0x02; // CONVST_IN信号触发源

// AD硬件参数PCI9757_PARA_AD中的TriggerType触发类型所使用的选项
const long PCI9757_TRIGTYPE_EDGE		= 0x00; // 边沿触发
const long PCI9757_TRIGTYPE_PULSE		= 0x01; // 脉冲触发(电平)

//***********************************************************
// AD硬件参数PCI9757_PARA_AD中的TriggerDir触发方向所使用的选项
const long PCI9757_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低脉冲/下降沿触发)
const long PCI9757_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高脉冲/上升沿触发)
const long PCI9757_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低脉冲或上升/下降沿触发)

//***********************************************************
// AD硬件参数PCI9757_PARA_AD中的ClockSource时钟源所使用的选项
const long PCI9757_CLOCKSRC_IN			= 0x00; // 内部时钟
const long PCI9757_CLOCKSRC_OUT			= 0x01; // 外部时钟

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _PCI9757_STATUS_AD     
{
	LONG bNotEmpty;			// 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
	LONG bHalf;				// 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
	LONG bDynamic_Overflow; // 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
	LONG bStatic_Overflow;  // 板载FIFO存储器的静态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
	LONG bConverting;		// AD是否正在转换， =TRUE:表示正在转换， =FALS表示转换完成
	LONG bTriggerFlag;		// 触发标志, =TRUE表示触发事件发生，　=FALSE表示触发事件未发生
	LONG nTriggerPos;       // 触发位置
} PCI9757_STATUS_AD, *PPCI9757_STATUS_AD;

const int MAX_SEGMENT_COUNT = 128;
typedef struct _PCI9757_STATUS_DMA     
{
	LONG iCurSegmentID;								// 当前段缓冲ID,表示DMA正在传输的缓冲区段
	LONG bSegmentSts[128];			// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
	LONG bBufferOverflow;							// 返回溢出状态,=1表示溢出，否则未溢出
} PCI9757_STATUS_DMA, *PPCI9757_STATUS_DMA;

//***********************************************************
// 用户函数接口
#ifndef _PCI9757_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 设备对象管理函数 ##############################
	HANDLE DEVAPI FAR PASCAL PCI9757_CreateDevice(int DeviceLgcID );     // 用逻辑号创建设备对象
	int DEVAPI FAR PASCAL PCI9757_GetDeviceCount(HANDLE hDevice); // 取得设备总台数
	BOOL DEVAPI FAR PASCAL PCI9757_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
	BOOL DEVAPI FAR PASCAL PCI9757_ListDeviceDlg(HANDLE hDevice); // 以对话框窗体方式列表系统当中的所有的该PCI设备
    BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDevice(HANDLE hDevice); // 仅释放设备对象

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	// AD数据两个函数便能轻松高效地实现高速、连续的数据采集
	BOOL DEVAPI FAR PASCAL PCI9757_Calibration(HANDLE hDevice); // AD校准 
	// AD程序查询方式函数
	BOOL DEVAPI FAR PASCAL PCI9757_InitDeviceProAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									PPCI9757_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCI9757_StartDeviceProAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI9757_ReadDeviceProAD_Npt(		// 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									LONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									LONG nReadSizeWords,	// 相对于偏移点后读入的数据长度(字)
									PLONG nRetSizeWords);   // 返回实际读取的长度(字)

	BOOL DEVAPI FAR PASCAL PCI9757_GetDevStatusProAD(		// 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI9757_STATUS_AD pADStatus); // AD的各种信息结构体

	BOOL DEVAPI FAR PASCAL PCI9757_ReadDeviceProAD_Half(	// 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									LONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									LONG nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
									PLONG nRetSizeWords);	// 返回实际读取的长度(字)
		
    BOOL DEVAPI FAR PASCAL PCI9757_StopDeviceProAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDeviceProAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建

	//##################### AD数据读取函数(DMA方式)(上层用户函数) ###########################
	// AD直接内存(DMA)方式函数
	BOOL DEVAPI FAR PASCAL PCI9757_InitDeviceDmaAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									HANDLE hDmaEvent,		// DMA事件句柄,它由CreateSystemEvent创建
									LONG ADBuffer[],		// 用户缓冲区,最好为两维数组
									LONG nReadSizeWords,	// 每次DMA时,用户从指定缓冲应读取的实际长度(要小于或等于nSegmentSizeWords)
									LONG nSegmentCount,		// 缓冲分段的数量,取值范围为2-128
									LONG nSegmentSizeWords,	// 缓冲区分段的段长(必须等于FIFO半满长度)
									PPCI9757_PARA_AD pADPara);	// 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCI9757_StartDeviceDmaAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI9757_GetDevStatusDmaAD(		// 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI9757_STATUS_DMA pDMAStatus); // 获得的DMA工作状态

	BOOL DEVAPI FAR PASCAL PCI9757_SetDevStatusDmaAD(		// 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									LONG iClrBufferID);		// 要清除的缓冲区ID,将其置为0
		
    BOOL DEVAPI FAR PASCAL PCI9757_StopDeviceDmaAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseDeviceDmaAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建
	
	//##################### AD的硬件参数操作函数 ###########################
	BOOL DEVAPI FAR PASCAL PCI9757_SaveParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
    BOOL DEVAPI FAR PASCAL PCI9757_LoadParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // 将AD采样参数从系统中读出
    BOOL DEVAPI FAR PASCAL PCI9757_ResetParaAD(HANDLE hDevice, PPCI9757_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	BOOL DEVAPI FAR PASCAL PCI9757_GetDeviceBar(					// 取得指定的指定设备寄存器组BAR地址
											HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
											PUCHAR pbPCIBar[6]);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书

	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterByte(				// 往设备的映射寄存器空间指定端口写入单节字数据
											HANDLE hDevice,			// 设备对象
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes,		// 相对于基地址的偏移位置
											BYTE Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）
	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterWord(				// 写双字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes,  
											WORD Value);
	BOOL DEVAPI FAR PASCAL PCI9757_WriteRegisterULong(				// 写四节字数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes,  
											ULONG Value);
	BYTE DEVAPI FAR PASCAL PCI9757_ReadRegisterByte(				// 读入单字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes);
	WORD DEVAPI FAR PASCAL PCI9757_ReadRegisterWord(				// 读入双字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes);
	ULONG DEVAPI FAR PASCAL PCI9757_ReadRegisterULong(				// 读入四字节数据（其余同上）
											HANDLE hDevice, 
											PUCHAR pbLinearAddr,	// 指定映射寄存器的线性基地址
											ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortByte(HANDLE hDevice, PUCHAR pbPort, BYTE Value);
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortWord(HANDLE hDevice, PUCHAR pbPort, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI9757_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI9757_ReadPortByte(HANDLE hDevice, PUCHAR pbPort);
    WORD DEVAPI FAR PASCAL PCI9757_ReadPortWord(HANDLE hDevice, PUCHAR pbPort);
    ULONG DEVAPI FAR PASCAL PCI9757_ReadPortULong(HANDLE hDevice, PUCHAR pbPort);

	//########################### 附加操作函数 ######################################
	HANDLE DEVAPI FAR PASCAL PCI9757_CreateSystemEvent(void);			// 创建内核事件对象，供InitDeviceInt和VB子线程等函数使用
	BOOL DEVAPI FAR PASCAL PCI9757_ReleaseSystemEvent(HANDLE hEvent);	// 释放内核事件对象

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
const long PCI9757_REG_MEM_PLXCHIP	= 0x00; // 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
const long PCI9757_REG_IO_PLXCHIP	= 0x01; // 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)
const long PCI9757_REG_IO_CPLD		= 0x02; // 2号寄存器对应板上控制单元所使用的IO模式基地址(使用PhysAddr)
 

#endif // _PCI9757_DEVICE_
