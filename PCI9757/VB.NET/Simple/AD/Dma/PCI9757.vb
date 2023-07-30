Module PCI9757

    '#################### AD硬件参数PCI9757_PARA_AD定义 #####################

    Structure PCI9757_PARA_AD
        Dim bChannelArray0 As Int32     ' 采样通道选择阵列，分别控制4个通道，=TRUE表示该通道采样，否则不采样
        Dim bChannelArray1 As Int32
        Dim bChannelArray2 As Int32
        Dim bChannelArray3 As Int32
        Dim InputRange0 As Int32        ' 模拟量输入量程选择阵列，分别控制4个通道
        Dim InputRange1 As Int32
        Dim InputRange2 As Int32
        Dim InputRange3 As Int32
        Dim Frequency As Int32         ' 采集频率, 单位为Hz
        Dim TriggerMode As Int32        ' 触发模式选择
        Dim TriggerSource As Int32      ' 触发源选择
        Dim TriggerType As Int32        ' 触发类型选择(边沿触发/脉冲触发)
        Dim TriggerDir As Int32     ' 触发方向选择(正向/负向触发)
        Dim TrigLevelVolt As Int32  ' 触发电平(0—10000mV)
        Dim TrigWindow As Int32     ' 触发灵敏窗[1, 65535], 单位25纳秒
        Dim ClockSource As Int32        ' 时钟源选择(内/外时钟源)

    End Structure

    '***********************************************************
    ' AD硬件参数PCI9757_PARA_AD中的InputRange[x]模拟量输入范围所使用的选项
    Public Const PCI9757_INPUT_N10000_P10000mV = &H0 ' ±10000mV
    Public Const PCI9757_INPUT_N5000_P5000mV = &H1 ' ±5000mV
    Public Const PCI9757_INPUT_N2500_P2500mV = &H2 ' ±2500mV
    Public Const PCI9757_INPUT_0_P10000mV = &H3 ' 0～10000mV
    Public Const PCI9757_INPUT_0_P5000mV = &H4 ' 0～5000mV

    '***********************************************************
    ' AD硬件参数PCI9757_PARA_AD中的TriggerMode成员变量所使用触发模式选项
    Public Const PCI9757_TRIGMODE_SOFT = &H0 ' 软件内触发
    Public Const PCI9757_TRIGMODE_POST = &H1 ' 硬件后触发

    '***********************************************************
    ' AD硬件参数PCI9757_PARA_AD中的TriggerSource触发源信号所使用的选项
    Public Const PCI9757_TRIGSRC_ATR = &H0 ' 选择外部ATR触发源
    Public Const PCI9757_TRIGSRC_DTR = &H1 ' 选择外部DTR触发源
    Public Const PCI9757_TRIGSRC_PCI_TRIG0 = &H2 ' CONVST_IN信号触发源

    ' AD硬件参数PCI9757_PARA_AD中的TriggerType触发类型所使用的选项
    Public Const PCI9757_TRIGTYPE_EDGE = &H0 ' 边沿触发
    Public Const PCI9757_TRIGTYPE_PULSE = &H1 ' 脉冲触发(电平)

    '***********************************************************
    ' AD硬件参数PCI9757_PARA_AD中的TriggerDir触发方向所使用的选项
    Public Const PCI9757_TRIGDIR_NEGATIVE = &H0 ' 负向触发(低脉冲/下降沿触发)
    Public Const PCI9757_TRIGDIR_POSITIVE = &H1 ' 正向触发(高脉冲/上升沿触发)
    Public Const PCI9757_TRIGDIR_POSIT_NEGAT = &H2 ' 正负向触发(高/低脉冲或上升/下降沿触发)

    '***********************************************************
    ' AD硬件参数PCI9757_PARA_AD中的ClockSource时钟源所使用的选项
    Public Const PCI9757_CLOCKSRC_IN = &H0 ' 内部时钟
    Public Const PCI9757_CLOCKSRC_OUT = &H1 ' 外部时钟

    '*************************************************************************************
    ' 用于AD采样的实际硬件参数
    Structure PCI9757_STATUS_AD
        Dim bNotEmpty As Int32          ' 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
        Dim bHalf As Int32              ' 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
        Dim bDynamic_Overflow As Int32 ' 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
        Dim bStatic_Overflow As Int32  ' 板载FIFO存储器的静态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
        Dim bConverting As Int32        ' AD是否正在转换， =TRUE:表示正在转换， =FALS表示转换完成
        Dim bTriggerFlag As Int32       ' 触发标志, =TRUE表示触发事件发生，　=FALSE表示触发事件未发生
        Dim nTriggerPos As Int32      ' 触发位置
    End Structure

    Public Const MAX_SEGMENT_COUNT = 128
    Structure PCI9757_STATUS_DMA
        Dim iCurSegmentID As Int32                              ' 当前段缓冲ID,表示DMA正在传输的缓冲区段
        <VBFixedString(128), System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.ByValArray, SizeConst:=128)> Public bSegmentSts As Int32()
        ' 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
        Dim bBufferOverflow As Int32                            ' 返回溢出状态
    End Structure

    '***********************************************************
    ' 用户函数接口

    '######################## 设备对象管理函数 ##############################
    Declare Function PCI9757_CreateDevice Lib "PCI9757_32" (Optional ByVal DeviceLgcID As Int32 = 0) As Int32     ' 用逻辑号创建设备对象
    Declare Function PCI9757_GetDeviceCount Lib "PCI9757_32" (ByVal hDevice As Int32) As Int32 ' 取得设备总台数
    Declare Function PCI9757_GetDeviceCurrentID Lib "PCI9757_32" (ByVal hDevice As Int32, ByRef DeviceLgcID As Int32, ByRef DevicePhysID As Int32) As Boolean
    Declare Function PCI9757_ListDeviceDlg Lib "PCI9757_32" (ByVal hDevice As Int32) As Boolean ' 以对话框窗体方式列表系统当中的所有的该PCI设备
    Declare Function PCI9757_ReleaseDevice Lib "PCI9757_32" (ByVal hDevice As Int32) As Boolean ' 仅释放设备对象

    '####################### AD数据读取函数 #################################
    ' 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
    ' 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
    ' AD数据两个函数便能轻松高效地实现高速、连续的数据采集

    ' AD通用函数
    Declare Function PCI9757_Calibration Lib "PCI9757_32" ( _
           ByVal hDevice As Int32) As Boolean   'AD校准

    ' AD程序查询方式函数
    Declare Function PCI9757_InitDeviceProAD Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByRef pADPara As PCI9757_PARA_AD) As Boolean ' 初始化设备，当返回TRUE后,设备即准备就绪.

    Declare Function PCI9757_StartDeviceProAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 在初始化之后，启动设备

    Declare Function PCI9757_ReadDeviceProAD_Npt Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal ADBuffer() As Int32, _
            ByVal nReadSizeWords As Int32, _
            ByRef nRetSizeWords As Int32) As Boolean   ' 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)

    Declare Function PCI9757_GetDevStatusProAD Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByRef pADStatus As PCI9757_STATUS_AD) As Boolean ' 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功

    Declare Function PCI9757_ReadDeviceProAD_Half Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal ADBuffer() As Int32, _
            ByVal nReadSizeWords As Int32, _
            ByRef nRetSizeWords As Int32) As Boolean ' 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)

    Declare Function PCI9757_StopDeviceProAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 在启动设备之后，暂停设备

    Declare Function PCI9757_ReleaseDeviceProAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 关闭AD设备,禁止传输,且释放资源


    '##################### AD数据读取函数(DMA方式)(上层用户函数) ###########################
    ' AD直接内存(DMA)方式函数
    Declare Function PCI9757_InitDeviceDmaAD Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal hDmaEvent As Int32, _
            ByVal ADBuffer(,) As Int32, _
            ByVal nReadSizeWords As Int32, _
            ByVal nSegmentCount As Int32, _
            ByVal nSegmentSizeWords As Int32, _
            ByRef pADPara As PCI9757_PARA_AD) As Boolean ' 初始化设备，当返回TRUE后,设备即准备就绪.

    Declare Function PCI9757_StartDeviceDmaAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 在初始化之后，启动设备

    Declare Function PCI9757_GetDevStatusDmaAD Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByRef pDMAStatus As PCI9757_STATUS_DMA) As Boolean   ' 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功

    Declare Function PCI9757_SetDevStatusDmaAD Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal iClrBufferID As Int32) As Boolean      ' 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功

    Declare Function PCI9757_StopDeviceDmaAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 在启动设备之后，暂停设备

    Declare Function PCI9757_ReleaseDeviceDmaAD Lib "PCI9757_32" ( _
         ByVal hDevice As Int32) As Boolean       ' 关闭AD设备,禁止传输,且释放资源

    '##################### AD的硬件参数操作函数 ###########################
    Declare Function PCI9757_SaveParaAD Lib "PCI9757_32" (ByVal hDevice As Int32, ByRef pADPara As PCI9757_PARA_AD) As Boolean ' 将当前的AD采样参数保存至系统中
    Declare Function PCI9757_LoadParaAD Lib "PCI9757_32" (ByVal hDevice As Int32, ByRef pADPara As PCI9757_PARA_AD) As Boolean ' 将AD采样参数从系统中读出
    Declare Function PCI9757_ResetParaAD Lib "PCI9757_32" (ByVal hDevice As Int32, ByRef pADPara As PCI9757_PARA_AD) As Boolean ' 将AD采样参数恢复至出厂默认值

    '################# 内存映射寄存器直接操作及读写函数 ########################
    ' 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
    ' 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
    Declare Function PCI9757_GetDeviceBar Lib "PCI9757_32" ( _
             ByVal hDevice As Int32, _
             ByRef pulPCIBar As UInt32) As Boolean   ' 取得指定的指定设备寄存器组BAR地址

    Declare Function PCI9757_WriteRegisterByte Lib "PCI9757_32" ( _
         ByVal hDevice As Int32, _
         ByVal LinearAddr As UInt32, _
         ByVal OffsetBytes As UInt32, _
         ByVal Value As Byte) As Boolean          ' 往指定寄存器空间位置写入单节字数据

    Declare Function PCI9757_WriteRegisterWord Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal LinearAddr As UInt32, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt16) As Boolean           ' 写双字节数据（其余同上）

    Declare Function PCI9757_WriteRegisterULong Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal LinearAddr As UInt32, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt32) As Boolean          ' 写四节字数据（其余同上）

    Declare Function PCI9757_ReadRegisterByte Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal LinearAddr As UInt32, _
            ByVal OffsetBytes As UInt32) As Byte       ' 读入单字节数据（其余同上）

    Declare Function PCI9757_ReadRegisterWord Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal LinearAddr As UInt32, _
            ByVal OffsetBytes As UInt32) As UInt16     ' 读入双字节数据（其余同上）

    Declare Function PCI9757_ReadRegisterULong Lib "PCI9757_32" ( _
            ByVal hDevice As Int32, _
            ByVal LinearAddr As UInt32, _
            ByVal OffsetBytes As UInt32) As UInt32     ' 读入四字节数据（其余同上）

    '################# I/O端口直接操作及读写函数 ########################
    ' 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
    ' 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
    ' 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    Declare Function PCI9757_WritePortByte Lib "PCI9757_32" ( _
         ByVal hDevice As Int32, _
         ByVal nPort As UInt32, _
         ByVal Value As Byte) As Boolean          ' 8位写端口

    Declare Function PCI9757_WritePortWord Lib "PCI9757_32" (ByVal hDevice As Int32, ByVal nPort As UInt32, ByVal Value As UInt16) As Boolean
    Declare Function PCI9757_WritePortULong Lib "PCI9757_32" (ByVal hDevice As Int32, ByVal nPort As UInt32, ByVal Value As UInt32) As Boolean

    Declare Function PCI9757_ReadPortByte Lib "PCI9757_32" (ByVal hDevice As Int32, ByVal nPort As UInt32) As Byte
    Declare Function PCI9757_ReadPortWord Lib "PCI9757_32" (ByVal hDevice As Int32, ByVal nPort As UInt32) As UInt16
    Declare Function PCI9757_ReadPortULong Lib "PCI9757_32" (ByVal hDevice As Int32, ByVal nPort As UInt32) As UInt32
    ' 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数


    '########################### 线程操作函数 ######################################
    Declare Function PCI9757_CreateSystemEvent Lib "PCI9757_32" () As Int32 ' 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
    Declare Function PCI9757_ReleaseSystemEvent Lib "PCI9757_32" (ByVal hEvent As Int32) As Boolean ' 释放内核事件对象

    '***********************************************************
    ' 各种FIFO存储器的长度(点)
    Public Const FIFO_IDT7202_LENGTH = 1024
    Public Const FIFO_IDT7203_LENGTH = 2048
    Public Const FIFO_IDT7204_LENGTH = 4096
    Public Const FIFO_IDT7205_LENGTH = 8192
    Public Const FIFO_IDT7206_LENGTH = 16384
    Public Const FIFO_IDT7207_LENGTH = 32768

    '***********************************************************
    ' 设备地址获取函数GetDeviceAddr的参数RegisterID所使用的选项(有效部分)
    Public Const PCI9757_REG_MEM_PLXCHIP = &H0 ' 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
    Public Const PCI9757_REG_IO_PLXCHIP = &H1 ' 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)
    Public Const PCI9757_REG_IO_CPLD = &H2 ' 3号寄存器对应板上AD缓冲区所使用的IO模式基地址(使用PhysAddr)

End Module
