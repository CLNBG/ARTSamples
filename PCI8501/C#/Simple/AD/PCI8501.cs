using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public partial class PCI8501
    {
        //***********************************************************
        // 用于AD采集的实际硬件参数
        public struct PCI8501_PARA_AD
        {

            public Int32 Frequency;         // 采集频率, 单位为Hz, 最高采样率4000000Hz
            public Int32 InputRange0;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange1;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange2;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange3;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange4;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange5;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange6;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 InputRange7;		// 模拟量输入量程选择阵列，分别控制4个通道
            public Int32 Gains0;	// 增益选择 
            public Int32 Gains1;	// 增益选择
            public Int32 Gains2;	// 增益选择
            public Int32 Gains3;	// 增益选择
            public Int32 Gains4;	// 增益选择 
            public Int32 Gains5;	// 增益选择
            public Int32 Gains6;	// 增益选择
            public Int32 Gains7;	// 增益选择
            public Int32 M_Length;          // M段长度(字), 总的取值范围1-32M, 但是M加N长度不能大于32M
            public Int32 N_Length;          // N段长度(字), 总的取值范围1-32M, 但是M加N长度不能大于32M            
            public Int32 TriggerMode;		// 触发模式选择
            public Int32 TriggerSource;		// 触发源选择            
            public Int32 TriggerDir;		// 触发方向选择(正向/负向触发)
          public Int32 TrigLevelVolt;		// 触发电平(±10000mV)            
            public Int32 ClockSource;		// 时钟源选择(内/外时钟源)
            public Int32 OutClockSource;	// 时钟输入输出源
            public Int32 bClockSourceDir;   // 是否将时钟信号输出到PXI总线,=TRUE:允许输出, =FALSE:允许输入
        }
        // AD硬件参数PCI8501_PARA_AD中的InputRange[x]模拟量输入范围所使用的选项
        public const Int32 PCI8501_INPUT_N10000_P10000mV = 0x00; // ±10000mV
        public const Int32 PCI8501_INPUT_N5000_P5000mV = 0x01; // ±5000mV
        public const Int32 PCI8501_INPUT_N2500_P2500mV = 0x02; // ±2500mV
        public const Int32 PCI8501_INPUT_0_P10000mV = 0x03; // 0～10000mV
        public const Int32 PCI8501_INPUT_0_P5000mV = 0x04; // 0～5000mV

        //***********************************************************
        // AD参数PCI8501_PARA_AD中的Gains[x]使用的硬件增益选项
        public const Int32 PCI8501_GAINS_1MULT = 0x00; // 1倍增益
        public const Int32 PCI8501_GAINS_2MULT = 0x01; // 2倍增益
        public const Int32 PCI8501_GAINS_5MULT = 0x02; // 4倍增益
        public const Int32 PCI8501_GAINS_10MULT = 0x03; // 8倍增益

        // AD硬件参数PCI8501_PARA_AD中的TriggerMode成员变量所使用触发模式选项
        public const Int32 PCI8501_TRIGMODE_MIDL = 0x00; // 中间触发
        public const Int32 PCI8501_TRIGMODE_POST = 0x01; // 后触发
        public const Int32 PCI8501_TRIGMODE_PRE = 0x02; // 预触发
        public const Int32 PCI8501_TRIGMODE_DELAY = 0x03; // 硬件延时触发

        //***********************************************************
        // AD硬件参数PCI8501_PARA_AD中的TriggerSource触发源信号所使用的选项
        public const Int32 PCI8501_TRIGMODE_SOFT = 0x00; // 软件触发
        public const Int32 PCI8501_TRIGSRC_DTR = 0x01; // 选择DTR作为触发源
        public const Int32 PCI8501_TRIGSRC_ATR = 0x02; // 选择ATR作为触发源
        public const Int32 PCI8501_TRIGSRC_TRIGGER = 0x03; // Trigger信号触发（用于多卡同步）

        //***********************************************************
        // AD硬件参数PCI8501_PARA_AD中的TriggerDir触发方向所使用的选项
        public const Int32 PCI8501_TRIGDIR_NEGATIVE = 0x00; // 下降沿触发
        public const Int32 PCI8501_TRIGDIR_POSITIVE = 0x01; // 上升沿触发
        public const Int32 PCI8501_TRIGDIR_NEGAT_POSIT = 0x02; // 上下边沿均触发

        //***********************************************************
        // AD硬件参数PCI8501_PARA_AD中的ClockSource时钟源所使用的选项
        public const Int32 PCI8501_CLOCKSRC_IN = 0x00; // 使用内部时钟
        public const Int32 PCI8501_CLOCKSRC_OUT = 0x01; // 使用外部时钟

        //***********************************************************
        // AD硬件参数PCI8501_PARA_AD中的OutClockSource时钟源所使用的选项
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER0 = 0x00; // 选择TRIG0输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER1 = 0x01; // 选择TRIG1输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER2 = 0x02; // 选择TRIG2输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER3 = 0x03; // 选择TRIG3输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER4 = 0x04; // 选择TRIG4输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER5 = 0x05; // 选择TRIG5输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER6 = 0x06; // 选择TRIG6输入/输出时钟
        public const Int32 PCI8501_OUTCLOCKSRC_TRIGGER7 = 0x07; // 选择TRIG7输入/输出时钟

        //*************************************************************************************
        // 用于AD采样的实际硬件参数
        public struct PCI8501_STATUS_AD      
       {
           public Int32 bADEanble;	// AD是否已经使能，=TRUE:表示已使能，=FALSE:表示未使能
           public Int32 bTrigger;  // AD是否被触发，=TRUE:表示已被触发，=FALSE:表示未被触发
           public Int32 bComplete; // AD是否整个转换过程是否结束，=TRUE:表示已结束，=FALSE:表示未结束
           public Int32 bAheadTrig;// AD触发点是否提前，=TRUE:表示触发点提前，=FALSE:表示触发点未提前
           public Int32 lEndAddr;	// 数据完成的结束地址
        }       
    
        //######################## 设备对象管理函数 #################################

        [DllImport("PCI8501_32.DLL")]
        public static extern IntPtr PCI8501_CreateDevice(Int32 DeviceID ); // 创建设备对象(该函数使用系统内逻辑设备ID）
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceCount(IntPtr hDevice);      //  取得设备总台数
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ListDeviceDlg(IntPtr hDevice); // 用对话框列表系统当中的所有PCI设备
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseDevice(IntPtr hDevice);    // 仅释放设备对象

        //####################### AD数据读取函数 #################################
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDDR2Length(IntPtr hDevice, ref UInt32 ulDDR2Length);    // 设备校准
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ADCalibration(IntPtr hDevice, Int32 InputRange);    // 设备校准
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_InitDeviceAD(				// 初始化设备,当返回TRUE后,设备即刻开始传输.
					                IntPtr hDevice,				// 设备句柄,它应由CreateDevice函数创建
					                ref PCI8501_PARA_AD pADPara);  // 硬件参数, 它仅在此函数中决定硬件状态
		[DllImport("PCI8501_32.DLL")]
		public static extern Int32 PCI8501_StartDeviceAD(IntPtr hDevice);   //在初始化之后，启动设备
			
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SetDeviceTrigAD(IntPtr hDevice);	  // 当设备使能允许后，产生软件触发事件（只有触发源为软件触发时有效）
					                
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDevStatusAD(           // 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
                                    IntPtr hDevice,                      // 设备句柄,它由CreateDevice函数创建
                                    ref PCI8501_STATUS_AD pADStatus);       // AD的各种信息结构体
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadDeviceAD(       // 当AD标志有效时，用此函数读取设备上的AD数据(程序方式
                                    IntPtr hDevice,                     // 设备句柄,它由CreateDevice函数创建
                                    UInt16[] ADBuffer,                  // 接受原始AD数据的用户缓冲区
                                    Int32 nReadSizeWords,               // 相对于偏位点后读入的数据长度(字)
                                    Int32 ulStartAddr,                // 数据在RAM中的起始地址
                                    ref Int32 nRetSizeWords);           // 返回实际读取的长度(字)

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_StopDeviceAD( IntPtr hDevice); // 在启动设备之后，暂停设备

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseDeviceAD( IntPtr hDevice); // 关闭AD设备,禁止传输,且释放资源

      
        //################# AD的硬件参数操作函数 ########################	

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SaveParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_LoadParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // 将AD采样参数从系统中读出
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ResetParaAD(IntPtr hDevice, ref PCI8501_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值       

      
       //####################### 数字I/O输入输出函数 #################################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceDI(					// 取得开关量状态     
                                   IntPtr hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
                                   Byte[] bDISts);			// 开关输入状态(注意: 必须定义为8个字节元素的数组)
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_SetDeviceDO(					// 输出开关量状态
                                   IntPtr hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
                                   Byte[] bDOSts);			// 开关输出状态(注意: 必须定义为8个字节元素的数组)

        //################# 内存映射寄存器直接操作及读写函数 ########################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDeviceBar(					// 取得指定的指定设备寄存器组BAR地址    
                                   IntPtr hDevice,				// 设备句柄,它应由CreateDevice函数创建								        
                                   UInt16[] pbPCIBar);			// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_GetDevVersion(					// 往指定寄存器空间位置写入单节字数据    
                                   IntPtr hDevice,				// 设备句柄,它应由CreateDevice函数创建
                                   UInt32 pulFmwVersion,          //  固件版本
                                   UInt32 pulDriverVersion);      // 驱动版本                                   			
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterByte(					// 写单字节数据（其余同上）   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   UInt32 OffsetBytes,
                                   Byte Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterWord(					// 写双字节数据（其余同上）   
                                   IntPtr hDevice,				
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes,
                                   Byte Value);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WriteRegisterULong(					// 写四字节数据（其余同上）   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes,
                                   Byte Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterByte(					// 读入单字节数据（其余同上）   
                                   IntPtr hDevice,
                                   UInt16 pbLinearAddr,
                                   Int32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterWord(					// 读入双字节数据（其余同上）   
                                   IntPtr hDevice,
                                   Int32 pbLinearAddr,
                                   Int32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadRegisterULong(					// 读入四字节数据（其余同上）   
                                   IntPtr hDevice,
                                   Int32 pbLinearAddr,
                                   Int32 OffsetBytes);
        //################# I/O端口直接操作及读写函数 ########################

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortByte(					// 8位写端口   
                                   IntPtr hDevice,                          // 设备句柄(由CreateDevice创建)
                                   Int32 pPort,                             // 端口地址
                                   UInt32 OffsetBytes,
                                   Byte Value);                             // 写出的8位整型数据
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortWord(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes, UInt16 Value);
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_WritePortULong(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes, UInt32 Value);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortByte(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortWord(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);

        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReadPortULong(IntPtr hDevice, UInt16 pPort, UInt32 OffsetBytes);
        // 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数
        //########################### 附加操作函数 ######################################

        [DllImport("PCI8501_32.DLL")]
        public static extern IntPtr PCI8501_CreateSystemEvent(); // 取得指定文件长度（字节）// 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
        [DllImport("PCI8501_32.DLL")]
        public static extern Int32 PCI8501_ReleaseSystemEvent(IntPtr hEvent);// 释放内核事件对象
        //***********************************************************
        // 各种FIFO存储器的长度(点)
        public const Int32 FIFO_IDT7202_LENGTH = 1024;
        public const Int32 FIFO_IDT7203_LENGTH = 2048;
        public const Int32 FIFO_IDT7204_LENGTH = 4096;
        public const Int32 FIFO_IDT7205_LENGTH = 8192;
        public const Int32 FIFO_IDT7206_LENGTH = 16384;
        public const Int32 FIFO_IDT7207_LENGTH = 32768;

    }
}
