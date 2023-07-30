using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    #pragma warning disable 0649
    class PCI9757
    {

        //#################### AD硬件参数PCI9757_PARA_AD定义 #####################
        // 用于AD采样的实际硬件参数
        public struct PCI9757_PARA_AD     
        {
            public Int32 bChannelArray0;	// 采样通道选择阵列，分别控制8个通道，=TRUE表示该通道采样，否则不采样
            public Int32 bChannelArray1;
            public Int32 bChannelArray2;
            public Int32 bChannelArray3;
            public Int32 InputRange0;		// 模拟量输入量程选择
            public Int32 InputRange1;
            public Int32 InputRange2;
            public Int32 InputRange3;
	        public Int32 Frequency;         // 采集频率, 单位为Hz
	        public Int32 TriggerMode;		// 触发模式选择
	        public Int32 TriggerSource;	// 触发源选择
	        public Int32 TriggerType;		// 触发类型选择(边沿触发/脉冲触发)
	        public Int32 TriggerDir;		// 触发方向选择(正向/负向触发)
	        public Int32 TrigLevelVolt;		// 触发电平(0—10000mV)
            public Int32 TrigWindow;		// 触发灵敏窗[1, 65535], 单位25纳秒
            public Int32 ClockSource;		// 时钟源选择(内/外时钟源)	      
        }

        //***********************************************************
        // AD硬件参数PCI9757_PARA_AD中的InputRange[x]模拟量输入范围所使用的选项
        public const Int32 PCI9757_INPUT_N10000_P10000mV = 0x00; // ±10000mV
        public const Int32 PCI9757_INPUT_N5000_P5000mV = 0x01; // ±5000mV
        public const Int32 PCI9757_INPUT_N2500_P2500mV = 0x02; // ±2500mV
        public const Int32 PCI9757_INPUT_0_P10000mV = 0x03; // 0～10000mV
        public const Int32 PCI9757_INPUT_0_P5000mV = 0x04; // 0～5000mV


        //***********************************************************
        // AD硬件参数PCI9757_PARA_AD中的TriggerMode成员变量所使用触发模式选项
        public const Int32 PCI9757_TRIGMODE_SOFT = 0x00; // 软件内触发
        public const Int32 PCI9757_TRIGMODE_POST = 0x01; // 硬件后触发

        //***********************************************************
        // AD硬件参数PCI9757_PARA_AD中的TriggerSource触发源信号所使用的选项
        public const Int32 PCI9757_TRIGSRC_ATR = 0x00; // 选择外部ATR触发源
        public const Int32 PCI9757_TRIGSRC_DTR = 0x01; // 选择外部DTR触发源
        public const Int32 PCI9757_TRIGSRC_PCI_TRIG0 = 0x02; // CONVST_IN信号触发源

        // AD硬件参数PCI9757_PARA_AD中的TriggerType触发类型所使用的选项
        public const Int32 PCI9757_TRIGTYPE_EDGE = 0x00; // 边沿触发
        public const Int32 PCI9757_TRIGTYPE_PULSE = 0x01; // 脉冲触发(电平)

        //***********************************************************
        // AD硬件参数PCI9757_PARA_AD中的TriggerDir触发方向所使用的选项
        public const Int32 PCI9757_TRIGDIR_NEGATIVE = 0x00; // 负向触发(低脉冲/下降沿触发)
        public const Int32 PCI9757_TRIGDIR_POSITIVE = 0x01; // 正向触发(高脉冲/上升沿触发)
        public const Int32 PCI9757_TRIGDIR_POSIT_NEGAT = 0x02; // 正负向触发(高/低脉冲或上升/下降沿触发)

        //***********************************************************
        // AD硬件参数PCI9757_PARA_AD中的ClockSource时钟源所使用的选项
        public const Int32 PCI9757_CLOCKSRC_IN = 0x00; // 内部时钟
        public const Int32 PCI9757_CLOCKSRC_OUT = 0x01; // 外部时钟

        //*************************************************************************************
        // 用于AD采样的实际硬件参数
        public struct PCI9757_STATUS_AD     
        {
	        public Int32 bNotEmpty;			// 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
	        public Int32 bHalf;				// 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
	        public Int32 bDynamic_Overflow; // 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
	        public Int32 bStatic_Overflow;  // 板载FIFO存储器的静态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
	        public Int32 bConverting;		// AD是否正在转换， =TRUE:表示正在转换， =FALS表示转换完成
	        public Int32 bTriggerFlag;		// 触发标志, =TRUE表示触发事件发生，　=FALSE表示触发事件未发生
            public Int32 nTriggerPos;       // 触发位置
        }

        const int MAX_SEGMENT_COUNT = 128;
        public struct PCI9757_STATUS_DMA     
        {
	        public Int32 iCurSegmentID;								// 当前段缓冲ID,表示DMA正在传输的缓冲区段
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128, ArraySubType = UnmanagedType.Struct)]           
            public Int32[] bSegmentSts;			// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧          
	        public Int32 bBufferOverflow;							// 返回溢出状态
        }

        //***********************************************************
        // 用户函数接口

	    //######################## 设备对象管理函数 ##############################
            [DllImport("PCI9757_32.DLL")]
	    public static extern IntPtr PCI9757_CreateDevice(Int32 DeviceLgcID);     // 用逻辑号创建设备对象
            [DllImport("PCI9757_32.DLL")]
	    public static extern Int32 PCI9757_GetDeviceCount(IntPtr hDevice); // 取得设备总台数
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_GetDeviceCurrentID(IntPtr hDevice, ref Int32 DeviceLgcID, ref Int32 DevicePhysID);
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ListDeviceDlg(IntPtr hDevice); // 以对话框窗体方式列表系统当中的所有的该PCI设备
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_ReleaseDevice(IntPtr hDevice); // 仅释放设备对象

	    //####################### AD数据读取函数 #################################
	    // 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	    // 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	    // AD数据两个函数便能轻松高效地实现高速、连续的数据采集

	    // AD通用函数
            [DllImport("PCI9757_32.DLL")]
        public static extern Int32 PCI9757_Calibration(	IntPtr hDevice);		// AD校准 
	    // AD程序查询方式函数
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_InitDeviceProAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									    IntPtr hDevice,			// 设备对象,它由CreateDevice函数创建
									    ref PCI9757_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态
            
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_StartDeviceProAD(		// 在初始化之后，启动设备
									    IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ReadDeviceProAD_Npt(		// 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)
									    IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									    Int32[] ADBuffer,		// 接受原始AD数据的用户缓冲区
									    Int32 nReadSizeWords,	// 相对于偏移点后读入的数据长度(字)
									    ref Int32 nRetSizeWords);   // 返回实际读取的长度(字)
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_GetDevStatusProAD(		// 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
									    IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									    ref PCI9757_STATUS_AD pADStatus); // AD的各种信息结构体
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ReadDeviceProAD_Half(	// 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)
									    IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									    Int32[] ADBuffe,		// 接受原始AD数据的用户缓冲区
									    Int32 nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
									    ref Int32 nRetSizeWords);	// 返回实际读取的长度(字)
    		
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_StopDeviceProAD(			// 在启动设备之后，暂停设备
									    IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ReleaseDeviceProAD(		// 关闭AD设备,禁止传输,且释放资源
									    IntPtr hDevice);		// 设备句柄,它由CreateDevice函数创建

	  	    //##################### AD数据读取函数(DMA方式)(上层用户函数) ###########################
	    // AD直接内存(DMA)方式函数
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_InitDeviceDmaAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									    IntPtr hDevice,			// 设备对象,它由CreateDevice函数创建
									    IntPtr hDmaEvent,		// DMA事件句柄,它由CreateSystemEvent创建
									    Int32[,] ADBuffer,		// 用户缓冲区,最好为两维数组
									    Int32 nReadSizeWords,	// 每次DMA时,用户从指定缓冲应读取的实际长度(要小于或等于nSegmentSizeWords)
									    Int32 nSegmentCount,		// 缓冲分段的数量,取值范围为2-128
									    Int32 nSegmentSizeWords,	// 缓冲区分段的段长(必须等于FIFO半满长度)
									    ref PCI9757_PARA_AD pADPara);	// 硬件参数, 它仅在此函数中决定硬件状态
            
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_StartDeviceDmaAD(		// 在初始化之后，启动设备
									    IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_GetDevStatusDmaAD(		// 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功
									    IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									    ref PCI9757_STATUS_DMA pDMAStatus); // 获得的DMA工作状态
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_SetDevStatusDmaAD(		// 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功
									    IntPtr hDevice,			// 设备句柄,它由CreateDevice函数创建
									    Int32 iClrBufferID);		// 要清除的缓冲区ID,将其置为0
    		
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_StopDeviceDmaAD(			// 在启动设备之后，暂停设备
									    IntPtr hDevice);		// 设备对象句柄,它由CreateDevice函数创建
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ReleaseDeviceDmaAD(		// 关闭AD设备,禁止传输,且释放资源
									    IntPtr hDevice);		// 设备句柄,它由CreateDevice函数创建
    	
	    //##################### AD的硬件参数操作函数 ###########################
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_SaveParaAD(IntPtr hDevice, ref PCI9757_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_LoadParaAD(IntPtr hDevice, ref PCI9757_PARA_AD pADPara); // 将AD采样参数从系统中读出
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_ResetParaAD(IntPtr hDevice, ref PCI9757_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	    //################# 内存映射寄存器直接操作及读写函数 ########################
	    // 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	    // 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_GetDeviceBar(			// 取得指定的指定设备寄存器组BAR地址
										    IntPtr hDevice,			// 设备对象句柄,它由CreateDevice函数创建
										    Int32[]  pulPCIBar);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书
            
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_WriteRegisterByte(		// 往指定寄存器空间位置写入单节字数据
									    IntPtr hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									    UInt32 LinearAddr,		// 指定寄存器的线性基地址,它等于GetDeviceAddr中的LinearAddr参数返回值
									    UInt32 OffsetBytes,		// 相对于线性基地址基地址的偏移位置(字节)
									    Byte Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_WriteRegisterWord(		// 写双字节数据（其余同上）
									    IntPtr hDevice, 
									    UInt32 LinearAddr, 
									    UInt32 OffsetBytes,  
									    UInt16 Value);
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_WriteRegisterULong(		// 写四节字数据（其余同上）
									    IntPtr hDevice, 
									    UInt32 LinearAddr, 
									    UInt32 OffsetBytes,  
									    UInt32 Value);
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern Byte PCI9757_ReadRegisterByte(		// 读入单字节数据（其余同上）
									    IntPtr hDevice, 
									    UInt32 LinearAddr, 
									    UInt32 OffsetBytes);
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern UInt16 PCI9757_ReadRegisterWord(		// 读入双字节数据（其余同上）
									    IntPtr hDevice, 
									    UInt32 LinearAddr, 
									    UInt32 OffsetBytes);
            
            [DllImport("PCI9757_32.DLL")]
	    public static extern UInt32 PCI9757_ReadRegisterULong(		// 读入四字节数据（其余同上）
									    IntPtr hDevice, 
									    UInt32 LinearAddr, 
									    UInt32 OffsetBytes);

	    //################# I/O端口直接操作及读写函数 ########################
	    // 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	    // 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	    // 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_WritePortByte(			// 8位写端口
									    IntPtr hDevice,			// 设备句柄(由CreateDevice创建)
									    UInt32 nPort,				// 端口地址
									    Byte Value);			// 写出的8位整型数据
            
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_WritePortWord(IntPtr hDevice, UInt32 nPort, UInt16 Value);
            [DllImport("PCI9757_32.DLL")]
        public static extern bool PCI9757_WritePortULong(IntPtr hDevice, UInt32 nPort, UInt32 Value);
            
            [DllImport("PCI9757_32.DLL")]
        public static extern Byte PCI9757_ReadPortByte(IntPtr hDevice, UInt32 nPort);
            [DllImport("PCI9757_32.DLL")]
        public static extern UInt16 PCI9757_ReadPortWord(IntPtr hDevice, UInt32 nPort);
            [DllImport("PCI9757_32.DLL")]
        public static extern UInt32 PCI9757_ReadPortULong(IntPtr hDevice, UInt32 nPort);
	    // 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数

   	    //########################### 线程操作函数 ######################################
            [DllImport("PCI9757_32.DLL")]
	    public static extern IntPtr PCI9757_CreateSystemEvent(); // 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
            [DllImport("PCI9757_32.DLL")]
	    public static extern bool PCI9757_ReleaseSystemEvent(IntPtr hEvent); // 释放内核事件对象
          
        //***********************************************************
        // 各种FIFO存储器的长度(点)
        public const Int32 FIFO_IDT7202_LENGTH			= 1024;
        public const Int32 FIFO_IDT7203_LENGTH			= 2048;
        public const Int32 FIFO_IDT7204_LENGTH			= 4096;
        public const Int32 FIFO_IDT7205_LENGTH			= 8192;
        public const Int32 FIFO_IDT7206_LENGTH			= 16384;
        public const Int32 FIFO_IDT7207_LENGTH			= 32768;

        //***********************************************************
        // 设备地址获取函数GetDeviceAddr的参数RegisterID所使用的选项(有效部分)
        public const Int32 PCI9757_REG_MEM_PLXCHIP	= 0x00; // 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
        public const Int32 PCI9757_REG_IO_PLXCHIP	= 0x01; // 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)
        public const Int32 PCI9757_REG_IO_CPLD		= 0x02; // 3号寄存器对应板上AD缓冲区所使用的IO模式基地址(使用PhysAddr)



    }
}
