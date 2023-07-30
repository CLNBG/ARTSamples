using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI8510
    {
        //#################### AD硬件参数PCI8510_PARA_AD定义 #####################
        // 用于AD采样的实际硬件参数
        public struct PCI8510_PARA_AD
        {
            public Int32 Gains0;			// 模拟量增益选择阵列，分别控制8个通道
            public Int32 Gains1;
            public Int32 Gains2;
            public Int32 Gains3;
            public Int32 Gains4;
            public Int32 Gains5;
            public Int32 Gains6;
            public Int32 Gains7;
	 public Int32 Frequency;         // 采集频率, 单位为Hz, [1, 500000]
	 public Int32 InputRange;		// 量程选择
	 public Int32 TriggerMode;		// 触发模式选择
	 public Int32 TriggerSource;		// 触发源选择
	 public Int32 TriggerType;		// 触发类型选择(边沿触发/脉冲触发)
	 public Int32 TriggerDir;		// 触发方向选择(正向/负向触发)
	 public Int32 TrigLevelVolt;		// 触发电平(±10000mV)
	 public Int32 ClockSource;		// 内外时钟选择
	 public Int32 bClockOutput;      // 是否将时钟信号输出,=TRUE:允许输出, =FALSE:禁止输出
	 public Int32 OutTrigSource;		// Trig信号输入/输出源
	 public Int32 OutTrigSourceDir;	// Trig信号输入/输出方向选择，=TRUE:允许输出, =FALSE:允许输入

        }
        
//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的Gains[x]模拟量输入范围所使用的选项
public const Int32 PCI8510_GAINS_1MULT			= 0x00; // 1倍增益
public const Int32 PCI8510_GAINS_2MULT			= 0x01; // 2倍增益
public const Int32 PCI8510_GAINS_5MULT			= 0x02; // 4倍增益
public const Int32 PCI8510_GAINS_10MULT			= 0x03; // 8倍增益

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的InputRange模拟量输入范围所使用的选项
public const Int32 PCI8510_INPUT_0_P5000mV		= 0x00; // 0～5000mV
public const Int32 PCI8510_INPUT_0_P10000mV		= 0x01; // 0～10000mV
public const Int32 PCI8510_INPUT_N5000_P5000mV	= 0x02; // ±5000mV
public const Int32 PCI8510_INPUT_N10000_P10000mV= 0x03; // ±10000mV

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerMode成员变量所使用触发模式选项
public const Int32 PCI8510_TRIGMODE_SOFT		= 0x00; // 软件内触发
public const Int32 PCI8510_TRIGMODE_POST		= 0x01; // 硬件后触发

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerSource触发源信号所使用的选项
public const Int32 PCI8510_TRIGSRC_ATR			= 0x00; // 选择外部ATR触发源
public const Int32 PCI8510_TRIGSRC_DTR			= 0x01; // 选择外部DTR触发源

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的OutTrigSource时钟源所使用的选项
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER0	= 0x00; // 选择TRIG0输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER1	= 0x01; // 选择TRIG1输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER2	= 0x02; // 选择TRIG2输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER3	= 0x03; // 选择TRIG3输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER4	= 0x04; // 选择TRIG4输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER5	= 0x05; // 选择TRIG5输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER6	= 0x06; // 选择TRIG6输出
public const Int32 PCI8510_OUTTRIGSRC_TRIGGER7	= 0x07; // 选择TRIG7输出
public const Int32 PCI8510_OUTCLOCKSRC_CLKIN	= 0x08; // 选择外部输入时钟源

// AD硬件参数PCI8510_PARA_AD中的TriggerType触发类型所使用的选项
public const Int32 PCI8510_TRIGTYPE_EDGE		= 0x00; // 边沿触发
public const Int32 PCI8510_TRIGTYPE_LEVELVOLT	= 0x01; // 电平触发

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的TriggerDir触发方向所使用的选项
public const Int32 PCI8510_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
public const Int32 PCI8510_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
public const Int32 PCI8510_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD硬件参数PCI8510_PARA_AD中的ClockSource时钟源所使用的选项
public const Int32 PCI8510_CLOCKSRC_IN			= 0x00; // 内部时钟
public const Int32 PCI8510_CLOCKSRC_OUT			= 0x01; // 外部时钟

//*************************************************************************************
// 用于AD采样的实际硬件参数
public struct PCI8510_STATUS_AD     
{
	public Int32 bNotEmpty;		// 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
	public Int32 bHalf;			// 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
	public Int32 bOverflow;		// 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
} 

public const Int32 MAX_SEGMENT_COUNT = 64;
public struct PCI8510_STATUS_DMA     
{
	public Int32 iCurSegmentID;								// 当前段缓冲ID,表示DMA正在传输的缓冲区段
    public Int32 bSegmentSts0;			// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
    public Int32 bSegmentSts1;
    public Int32 bSegmentSts2;
    public Int32 bSegmentSts3;
    public Int32 bSegmentSts4;
    public Int32 bSegmentSts5;
    public Int32 bSegmentSts6;
    public Int32 bSegmentSts7;
    public Int32 bSegmentSts8;
    public Int32 bSegmentSts9;
    public Int32 bSegmentSts10;
    public Int32 bSegmentSts11;
    public Int32 bSegmentSts12;
    public Int32 bSegmentSts13;
    public Int32 bSegmentSts14;
    public Int32 bSegmentSts15;
    public Int32 bSegmentSts16;
    public Int32 bSegmentSts17;
    public Int32 bSegmentSts18;
    public Int32 bSegmentSts19;
    public Int32 bSegmentSts20;
    public Int32 bSegmentSts21;
    public Int32 bSegmentSts22;
    public Int32 bSegmentSts23;
    public Int32 bSegmentSts24;
    public Int32 bSegmentSts25;
    public Int32 bSegmentSts26;
    public Int32 bSegmentSts27;
    public Int32 bSegmentSts28;
    public Int32 bSegmentSts29;
    public Int32 bSegmentSts30;
    public Int32 bSegmentSts31;
    public Int32 bSegmentSts32;
    public Int32 bSegmentSts33;
    public Int32 bSegmentSts34;
    public Int32 bSegmentSts35;
    public Int32 bSegmentSts36;
    public Int32 bSegmentSts37;
    public Int32 bSegmentSts38;
    public Int32 bSegmentSts39;
    public Int32 bSegmentSts40;
    public Int32 bSegmentSts41;
    public Int32 bSegmentSts42;
    public Int32 bSegmentSts43;
    public Int32 bSegmentSts44;
    public Int32 bSegmentSts45;
    public Int32 bSegmentSts46;
    public Int32 bSegmentSts47;
    public Int32 bSegmentSts48;
    public Int32 bSegmentSts49;
    public Int32 bSegmentSts50;
    public Int32 bSegmentSts51;
    public Int32 bSegmentSts52;
    public Int32 bSegmentSts53;
    public Int32 bSegmentSts54;
    public Int32 bSegmentSts55;
    public Int32 bSegmentSts56;
    public Int32 bSegmentSts57;
    public Int32 bSegmentSts58;
    public Int32 bSegmentSts59;
    public Int32 bSegmentSts60;
    public Int32 bSegmentSts61;
    public Int32 bSegmentSts62;
    public Int32 bSegmentSts63;				// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
	public Int32 bBufferOverflow;							// 返回溢出状态
} 

//***********************************************************
// CreateFileObject所用的文件操作方式控制字(可通过或指令实现多种方式并操作)
public const Int32 PCI8510_modeRead					= 0x0000;	// 只读文件方式
public const Int32 PCI8510_modeWrite				= 0x0001;	// 只写文件方式
public const Int32 PCI8510_modeReadWrite			= 0x0002;	// 既读又写文件方式
public const Int32 PCI8510_modeCreate				= 0x1000;	// 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0
public const Int32 PCI8510_typeText					= 0x4000;	// 以文本方式操作文件




        

        //######################## 设备对象管理函数 ##############################

        [DllImport("PCI8510_32.DLL")]
        public static extern IntPtr PCI8510_CreateDevice(Int32 DeviceLgcID);     // 用逻辑号创建设备对象
        [DllImport("PCI8510_32.DLL")]
        public static extern Int32 PCI8510_GetDeviceCount(IntPtr hDevice); // 取得设备总台数
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceCurrentID(IntPtr hDevice, ref Int32 DeviceLgcID, ref Int32 DevicePhysID);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDevice(IntPtr hDevice); // 仅释放设备对象

        //####################### AD数据读取函数 #################################
        // 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
        // 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
        // AD数据两个函数便能轻松高效地实现高速、连续的数据采集


        // AD程序查询方式函数
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_InitDeviceProAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
                                        IntPtr hDevice,			// 设备对象,它由CreateDevice函数创建
                                         ref PCI8510_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StartDeviceProAD(		// 在初始化之后，启动设备
                                        IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReadDeviceProAD_Npt(		// 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)
                                        IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
                                        UInt32[] ADBuffer,		// 接受原始AD数据的用户缓冲区
                                        UInt32 nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
                                        ref Int32 nRetSizeWords);   // 返回实际读取的长度(字)
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevStatusProAD(		// 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
                                        IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
                                        ref PCI8510_STATUS_AD pADStatus); // AD的各种信息结构体
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReadDeviceProAD_Half(	// 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)
                                        IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
                                        UInt32[] ADBuffer,		// 接受原始AD数据的用户缓冲区
                                        UInt32 nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
                                        ref Int32 nRetSizeWords);	// 返回实际读取的长度(字)
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StopDeviceProAD(			// 在启动设备之后，暂停设备
                                        IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDeviceProAD(		// 关闭AD设备,禁止传输,且释放资源
                                        IntPtr hDevice);		// 设备句柄,它由CreateDevice函数创建

        //##################### AD数据读取函数(中断方式)(上层用户函数) ###########################
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_InitDeviceDmaAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									IntPtr hDevice,			// 设备对象,它由CreateDevice函数创建
									IntPtr hDmaEvent,		// DMA事件句柄,它由CreateSystemEvent创建
									UInt32[,] ADBuffer,		// 用户缓冲区,最好为两维数组
									UInt32 nReadSizeWords,	// 每次DMA时,用户从指定缓冲应读取的实际长度(要小于或等于nSegmentSizeWords)
									Int32 nSegmentCount,		// 缓冲分段的数量,取值范围为2-128
									UInt32 nSegmentSizeWords,// 缓冲区分段的段长(必须等于FIFO半满长度)
									ref PCI8510_PARA_AD  pADPara );	
        
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StartDeviceDmaAD(		// 在初始化之后，启动设备
                                        IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevStatusDmaAD(		// 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功
									IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									ref PCI8510_STATUS_DMA pDMAStatus); // 获得的DMA工作状态
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SetDevStatusDmaAD(		// 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功
									IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									Int32 iClrBufferID);  
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_StopDeviceDmaAD(			// 在启动设备之后，暂停设备
                                        IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseDeviceDmaAD(		// 关闭AD设备,禁止传输,且释放资源
                                        IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ADCalibration(			// 设备校准函数
									IntPtr hDevice);

        //##################### AD的硬件参数操作函数 ###########################
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SaveParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_LoadParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // 将AD采样参数从系统中读出
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ResetParaAD(IntPtr hDevice, ref PCI8510_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值



        //####################### 数字I/O输入输出函数 #################################
        // 用户可以使用WriteRegisterULong和ReadRegisterULong等函数直接控制寄存器进行I/O
        // 输入输出，但使用下面两个函数更省事，它不需要您关心寄存器分配和位操作等，而只
        // 需象VB等语言的属性操作那么简单地实现各开关量通道的控制。
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_SetDeviceDO(				// 输出开关量状态
                                            IntPtr hDevice,		// 设备句柄								        
                                           Byte[] bDOSts);	// 开关状态
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceDI(				// 取得开关量状态     
                                            IntPtr hDevice,		// 设备句柄								        
                                           Byte[] bDISts);	// 开关状态	
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_RetDeviceDO(				// 回读开关量状态
                                            IntPtr hDevice,		// 设备句柄	 
                                           Byte[] bDOSts);	// 开关状态

        //################# 内存映射寄存器直接操作及读写函数 ########################
        // 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
        // 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
       [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDeviceBar(			// 取得指定的指定设备寄存器组BAR地址
                                        IntPtr hDevice,			// 设备对象句柄,它由CreateDevice函数创建
                                        Int64[] pulPCIBar);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_GetDevVersion(			// 获取设备固件及程序版本
                                        IntPtr hDevice,			// 设备对象句柄,它由CreateDevice函数创建
                                        ref UInt32 pulFmwVersion,	// 固件版本
                                        ref UInt32 pulDriverVersion);// 驱动版本
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterByte(		// 往指定寄存器空间位置写入单节字数据
                                        IntPtr hDevice,			// 设备对象句柄,它由CreateDevice函数创建
                                        Int64 LinearAddr,		// 指定寄存器的线性基地址,它等于GetDeviceAddr中的LinearAddr参数返回值
                                        UInt32 OffsetBytes,		// 相对于线性基地址基地址的偏移位置(字节)
                                        Byte Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterWord(		// 写双字节数据（其余同上）
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes,
                                        UInt16 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WriteRegisterULong(		// 写四节字数据（其余同上）
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes,
                                        UInt32 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Byte PCI8510_ReadRegisterByte(		// 读入单字节数据（其余同上）
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt16 PCI8510_ReadRegisterWord(		// 读入双字节数据（其余同上）
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt32 PCI8510_ReadRegisterULong(		// 读入四字节数据（其余同上）
                                        IntPtr hDevice,
                                        Int64 LinearAddr,
                                        UInt32 OffsetBytes);

        //################# I/O端口直接操作及读写函数 ########################
        // 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
        // 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
        // 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortByte(			// 8位写端口
                                        IntPtr hDevice,			// 设备句柄(由CreateDevice创建)
                                        Int64 nPort,				// 端口地址
                                        Byte Value);			// 写出的8位整型数据
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortWord(IntPtr hDevice, Int64 nPort, UInt16 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_WritePortULong(IntPtr hDevice, Int64 nPort, UInt32 Value);
        [DllImport("PCI8510_32.DLL")]
        public static extern Byte PCI8510_ReadPortByte(IntPtr hDevice, Int64 nPort);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt16 PCI8510_ReadPortWord(IntPtr hDevice, Int64 nPort);
        [DllImport("PCI8510_32.DLL")]
        public static extern UInt32 PCI8510_ReadPortULong(IntPtr hDevice, Int64 nPort);
        // 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数

  
        //########################### 线程操作函数 ######################################
        [DllImport("PCI8510_32.DLL")]
        public static extern IntPtr PCI8510_CreateSystemEvent(); // 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
        [DllImport("PCI8510_32.DLL")]
        public static extern Boolean PCI8510_ReleaseSystemEvent(IntPtr hEvent); // 释放内核事件对象
       

   

        //***********************************************************
        // 各种FIFO存储器的长度(点)
        public const Int32 FIFO_IDT7202_LENGTH = 1024;
        public const Int32 FIFO_IDT7203_LENGTH = 2048;
        public const Int32 FIFO_IDT7204_LENGTH = 4096;
        public const Int32 FIFO_IDT7205_LENGTH = 8192;
        public const Int32 FIFO_IDT7206_LENGTH = 16384;
        public const Int32 FIFO_IDT7207_LENGTH = 32768;

       

        //***********************************************************
        // 设备地址获取函数GetDeviceAddr的参数RegisterID所使用的选项(有效部分)
        public const Int32 PCI8510_REG_MEM_PLXCHIP = 0x00; // 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
        public const Int32 PCI8510_REG_IO_PLXCHIP = 0x01; // 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)

    }
}