Module PCI8510



    '#################### AD硬件参数PCI8510_PARA_AD定义 #####################
    ' 用于AD采样的实际硬件参数
    Structure PCI8510_PARA_AD

        Dim Gains0 As Int32            ' 模拟量增益选择阵列，分别控制8个通道
        Dim Gains1 As Int32
        Dim Gains2 As Int32
        Dim Gains3 As Int32
        Dim Gains4 As Int32
        Dim Gains5 As Int32
        Dim Gains6 As Int32
        Dim Gains7 As Int32
        Dim Frequency As Int32          ' 采集频率, 单位为Hz, [1, 500000]
        Dim InputRange As Int32         ' 量程选择
        Dim TriggerMode As Int32        ' 触发模式选择
        Dim TriggerSource As Int32     ' 触发源选择
        Dim TriggerType As Int32       ' 触发类型选择(边沿触发/脉冲触发)
        Dim TriggerDir As Int32   ' 触发方向选择(正向/负向触发)
        Dim TrigLevelVolt As Int32      ' 触发电平(±10000mV)
        Dim ClockSource As Int32      ' 内外时钟选择
        Dim bClockOutput As Int32     ' 是否将时钟信号输出,=TRUE:允许输出, =FALSE:禁止输出
        Dim OutTrigSource As Int32     ' Trig信号输入/输出源
        Dim OutTrigSourceDir As Int32   ' Trig信号输入/输出方向选择，=TRUE:允许输出, =FALSE:允许输入
    End Structure
    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的Gains[x]模拟量输入范围所使用的选项
    Public Const PCI8510_GAINS_1MULT = &H0  ' 1倍增益
    Public Const PCI8510_GAINS_2MULT = &H1  ' 2倍增益
    Public Const PCI8510_GAINS_5MULT = &H2  ' 4倍增益
    Public Const PCI8510_GAINS_10MULT = &H3  ' 8倍增益

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的InputRange模拟量输入范围所使用的选项
    Public Const PCI8510_INPUT_0_P5000mV = &H0  ' 0～5000mV
    Public Const PCI8510_INPUT_0_P10000mV = &H1  ' 0～10000mV
    Public Const PCI8510_INPUT_N5000_P5000mV = &H2  ' ±5000mV
    Public Const PCI8510_INPUT_N10000_P10000mV = &H3  ' ±10000mV

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的TriggerMode成员变量所使用触发模式选项
    Public Const PCI8510_TRIGMODE_SOFT = &H0  ' 软件内触发
    Public Const PCI8510_TRIGMODE_POST = &H1  ' 硬件后触发

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的TriggerSource触发源信号所使用的选项
    Public Const PCI8510_TRIGSRC_ATR = &H0  ' 选择外部ATR触发源
    Public Const PCI8510_TRIGSRC_DTR = &H1  ' 选择外部DTR触发源

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的OutTrigSource时钟源所使用的选项
    Public Const PCI8510_OUTTRIGSRC_TRIGGER0 = &H0  ' 选择TRIG0输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER1 = &H1  ' 选择TRIG1输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER2 = &H2  ' 选择TRIG2输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER3 = &H3  ' 选择TRIG3输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER4 = &H4  ' 选择TRIG4输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER5 = &H5  ' 选择TRIG5输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER6 = &H6  ' 选择TRIG6输出
    Public Const PCI8510_OUTTRIGSRC_TRIGGER7 = &H7  ' 选择TRIG7输出
    Public Const PCI8510_OUTCLOCKSRC_CLKIN = &H8  ' 选择外部输入时钟源

    ' AD硬件参数PCI8510_PARA_AD中的TriggerType触发类型所使用的选项
    Public Const PCI8510_TRIGTYPE_EDGE = &H0  ' 边沿触发
    Public Const PCI8510_TRIGTYPE_LEVELVOLT = &H1  ' 电平触发

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的TriggerDir触发方向所使用的选项
    Public Const PCI8510_TRIGDIR_NEGATIVE = &H0  ' 负向触发(低电平/下降沿触发)
    Public Const PCI8510_TRIGDIR_POSITIVE = &H1  ' 正向触发(高电平/上升沿触发)
    Public Const PCI8510_TRIGDIR_POSIT_NEGAT = &H2  ' 正负向触发(高/低电平或上升/下降沿触发)

    '***********************************************************
    ' AD硬件参数PCI8510_PARA_AD中的ClockSource时钟源所使用的选项
    Public Const PCI8510_CLOCKSRC_IN = &H0  ' 内部时钟
    Public Const PCI8510_CLOCKSRC_OUT = &H1  ' 外部时钟

    '*************************************************************************************
    ' 用于AD采样的实际硬件参数
    Structure PCI8510_STATUS_AD


        Dim bNotEmpty As Int32      ' 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
        Dim bHalf As Int32         '  板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
        Dim bOverflow As Int32      ' 板载FIFO存储器的动态溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
    End Structure

    Public Const MAX_SEGMENT_COUNT = 64
    Public Const bSegmentSts = MAX_SEGMENT_COUNT
    Structure PCI8510_STATUS_DMA

        Dim iCurSegmentID As Int32                              ' 当前段缓冲ID,表示DMA正在传输的缓冲区段
        Dim bSegmentSts0 As Int32                                ' 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
        Dim bSegmentSts1 As Int32
        Dim bSegmentSts2 As Int32
        Dim bSegmentSts3 As Int32
        Dim bSegmentSts4 As Int32
        Dim bSegmentSts5 As Int32
        Dim bSegmentSts6 As Int32
        Dim bSegmentSts7 As Int32
        Dim bSegmentSts8 As Int32
        Dim bSegmentSts9 As Int32

        Dim bSegmentSts10 As Int32
        Dim bSegmentSts11 As Int32
        Dim bSegmentSts12 As Int32
        Dim bSegmentSts13 As Int32
        Dim bSegmentSts14 As Int32
        Dim bSegmentSts15 As Int32
        Dim bSegmentSts16 As Int32
        Dim bSegmentSts17 As Int32
        Dim bSegmentSts18 As Int32
        Dim bSegmentSts19 As Int32

        Dim bSegmentSts20 As Int32
        Dim bSegmentSts21 As Int32
        Dim bSegmentSts22 As Int32
        Dim bSegmentSts23 As Int32
        Dim bSegmentSts24 As Int32
        Dim bSegmentSts25 As Int32
        Dim bSegmentSts26 As Int32
        Dim bSegmentSts27 As Int32
        Dim bSegmentSts28 As Int32
        Dim bSegmentSts29 As Int32

        Dim bSegmentSts30 As Int32
        Dim bSegmentSts31 As Int32
        Dim bSegmentSts32 As Int32
        Dim bSegmentSts33 As Int32
        Dim bSegmentSts34 As Int32
        Dim bSegmentSts35 As Int32
        Dim bSegmentSts36 As Int32
        Dim bSegmentSts37 As Int32
        Dim bSegmentSts38 As Int32
        Dim bSegmentSts39 As Int32

        Dim bSegmentSts40 As Int32
        Dim bSegmentSts41 As Int32
        Dim bSegmentSts42 As Int32
        Dim bSegmentSts43 As Int32
        Dim bSegmentSts44 As Int32
        Dim bSegmentSts45 As Int32
        Dim bSegmentSts46 As Int32
        Dim bSegmentSts47 As Int32
        Dim bSegmentSts48 As Int32
        Dim bSegmentSts49 As Int32

        Dim bSegmentSts50 As Int32
        Dim bSegmentSts51 As Int32
        Dim bSegmentSts52 As Int32
        Dim bSegmentSts53 As Int32
        Dim bSegmentSts54 As Int32
        Dim bSegmentSts55 As Int32
        Dim bSegmentSts56 As Int32
        Dim bSegmentSts57 As Int32
        Dim bSegmentSts58 As Int32
        Dim bSegmentSts59 As Int32

        Dim bSegmentSts60 As Int32
        Dim bSegmentSts61 As Int32
        Dim bSegmentSts62 As Int32
        Dim bSegmentSts63 As Int32          ' 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
        Dim bBufferOverflow As Int32                            ' 返回溢出状态
    End Structure



    '######################## 设备对象管理函数 ##############################

    Declare Function PCI8510_CreateDevice Lib "PCI8510_32" (ByVal DeviceLgcID As Int32) As IntPtr      ' 用逻辑号创建设备对象
    Declare Function PCI8510_GetDeviceCount Lib "PCI8510_32" (ByVal hDevice As IntPtr) As Int32  ' 取得设备总台数
    Declare Function PCI8510_GetDeviceCurrentID Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef DeviceLgcID As Int32, ByRef DevicePhysID As Int32) As Boolean
    Declare Function PCI8510_ReleaseDevice Lib "PCI8510_32" (ByVal hDevice As IntPtr) As Boolean  ' 仅释放设备对象

    '####################### AD数据读取函数 #################################
    ' 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
    ' 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
    ' AD数据两个函数便能轻松高效地实现高速、连续的数据采集


    ' AD程序查询方式函数
    Declare Function PCI8510_InitDeviceProAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' 初始化设备，当返回TRUE后,设备即准备就绪.' 设备对象,它由CreateDevice函数创建' 硬件参数, 它仅在此函数中决定硬件状态

    Declare Function PCI8510_StartDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' 在初始化之后，启动设备' 设备对象句柄,它由CreateDevice函数创建

    Declare Function PCI8510_ReadDeviceProAD_Npt Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal ADBufferas() As UInt32, _
            ByVal nReadSizeWords As UInt32, _
            ByRef nRetSizeWords As Int32) As Boolean ' 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式) ' 设备句柄,它由CreateDevice函数创建' 接受原始AD数据的用户缓冲区 ' 相对于偏位点后读入的数据长度(字) ' 返回实际读取的长度(字)

    Declare Function PCI8510_GetDevStatusProAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pADStatus As PCI8510_STATUS_AD) As Boolean  ' 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功' 设备句柄,它由CreateDevice函数创建' AD的各种信息结构体

    Declare Function PCI8510_ReadDeviceProAD_Half Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal ADBuffer() As UInt32, _
            ByVal nReadSizeWords As UInt32, _
            ByRef nRetSizeWords As Int32) As Boolean        ' 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)' 设备句柄,它由CreateDevice函数创建' 接受原始AD数据的用户缓冲区' 相对于偏位点后读入的数据长度(字)' 返回实际读取的长度(字)

    Declare Function PCI8510_StopDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' 在启动设备之后，暂停设备' 设备对象句柄,它由CreateDevice函数创建

    Declare Function PCI8510_ReleaseDeviceProAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean         ' 关闭AD设备,禁止传输,且释放资源' 设备句柄,它由CreateDevice函数创建

    '##################### ' AD直接内存(DMA)方式函数) ###########################
    Declare Function PCI8510_InitDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal hDmaEvent As IntPtr, _
         ByVal ADBuffer(,) As UInt32, _
         ByVal nReadSizeWords As UInt32, _
         ByVal nSegmentCount As Int32, _
         ByVal nSegmentSizeWords As UInt32, _
         ByRef pADPara As PCI8510_PARA_AD) As Boolean

    Declare Function PCI8510_StartDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean
    ' 在初始化之后，启动设备 ' 设备对象句柄,它由CreateDevice函数创建
    Declare Function PCI8510_GetDevStatusDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByRef pDMAStatus As PCI8510_STATUS_DMA) As Boolean ' 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功 ' 设备句柄,它由CreateDevice函数创建 ' 获得的DMA工作状态

    Declare Function PCI8510_SetDevStatusDmaAD Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal iClrBufferID As Int32) As Boolean         ' 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功' 设备句柄,它由CreateDevice函数创建' 要清除的缓冲区ID,将其置为0

    Declare Function PCI8510_StopDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean        ' 在启动设备之后，暂停设备' 设备对象句柄,它由CreateDevice函数创建

    Declare Function PCI8510_ReleaseDeviceDmaAD Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean      ' 关闭AD设备,禁止传输,且释放资源' 设备句柄,它由CreateDevice函数创建

    Declare Function PCI8510_ADCalibration Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr) As Boolean  ' 设备校准函数' 设备对象句柄

    '##################### AD的硬件参数操作函数 ###########################
    Declare Function PCI8510_SaveParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' 将当前的AD采样参数保存至系统中
    Declare Function PCI8510_LoadParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' 将AD采样参数从系统中读出
    Declare Function PCI8510_ResetParaAD Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByRef pADPara As PCI8510_PARA_AD) As Boolean  ' 将AD采样参数恢复至出厂默认值


    '####################### 数字I/O输入输出函数 #################################
    ' 用户可以使用WriteRegisterULong和ReadRegisterULong等函数直接控制寄存器进行I/O
    ' 输入输出，但使用下面两个函数更省事，它不需要您关心寄存器分配和位操作等，而只
    ' 需象VB等语言的属性操作那么简单地实现各开关量通道的控制。
    Declare Function PCI8510_SetDeviceDO Lib "PCI8510_32" ( _
          ByVal hDevice As IntPtr, _
          ByVal bDOSts() As Byte) As Boolean    ' 输出开关量状态' 设备句柄	' 开关状态

    Declare Function PCI8510_GetDeviceDI Lib "PCI8510_32" ( _
             ByVal hDevice As IntPtr, _
             ByVal bDISts() As Byte) As Boolean    ' 取得开关量状态    ' 设备句柄		' 开关状态	

    Declare Function PCI8510_RetDeviceDO Lib "PCI8510_32" ( _
             ByVal hDevice As IntPtr, _
             ByVal bDOSts() As Byte) As Boolean    ' 回读开关量状态' 设备句柄	 ' 开关状态

    '################# 内存映射寄存器直接操作及读写函数 ########################
    ' 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
    ' 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
    Declare Function PCI8510_GetDeviceBar Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal pulPCIBar() As Int64) As Boolean  ' 取得指定的指定设备寄存器组BAR地址' 设备对象句柄,它由CreateDevice函数创建' 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书

    Declare Function PCI8510_GetDevVersion Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByRef pulFmwVersion As UInt32, _
            ByRef pulDriverVersion As UInt32) As Boolean ' 获取设备固件及程序版本' 设备对象句柄,它由CreateDevice函数创建 ' 固件版本' 驱动版本

    Declare Function PCI8510_WriteRegisterByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32, _
         ByVal Value As Byte) As Boolean            ' 往指定寄存器空间位置写入单节字数据' 设备对象句柄,它由CreateDevice函数创建' 指定寄存器的线性基地址,它等于GetDeviceAddr中的LinearAddr参数返回值' 相对于线性基地址基地址的偏移位置(字节)' 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）

    Declare Function PCI8510_WriteRegisterWord Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal LinearAddr As Int64, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt16) As Boolean ' 写双字节数据（其余同上）

    Declare Function PCI8510_WriteRegisterULong Lib "PCI8510_32" ( _
            ByVal hDevice As IntPtr, _
            ByVal LinearAddr As Int64, _
            ByVal OffsetBytes As UInt32, _
            ByVal Value As UInt32) As Boolean ' 写四节字数据（其余同上）

    Declare Function PCI8510_ReadRegisterByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As Byte ' 读入单字节数据（其余同上）

    Declare Function PCI8510_ReadRegisterWord Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As UInt16 ' 读入双字节数据（其余同上）

    Declare Function PCI8510_ReadRegisterULong Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal LinearAddr As Int64, _
         ByVal OffsetBytes As UInt32) As UInt32 ' 读入四字节数据（其余同上）

    '################# I/O端口直接操作及读写函数 ########################
    ' 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
    ' 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
    ' 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    Declare Function PCI8510_WritePortByte Lib "PCI8510_32" ( _
         ByVal hDevice As IntPtr, _
         ByVal nPort As Int64, _
         ByVal Value As Byte) As Boolean         ' 8位写端口' 设备句柄(由CreateDevice创建)	' 端口地址' 写出的8位整型数据
    Declare Function PCI8510_WritePortWord Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64, ByVal Value As UInt16) As Boolean
    Declare Function PCI8510_WritePortULong Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64, ByVal Value As UInt32) As Boolean

    Declare Function PCI8510_ReadPortByte Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As Byte
    Declare Function PCI8510_ReadPortWord Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As UInt16
    Declare Function PCI8510_ReadPortULong Lib "PCI8510_32" (ByVal hDevice As IntPtr, ByVal nPort As Int64) As UInt32
    ' 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数

  
    '########################### 线程操作函数 ######################################
    Declare Function PCI8510_CreateSystemEvent Lib "PCI8510_32" () As IntPtr  ' 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
    Declare Function PCI8510_ReleaseSystemEvent Lib "PCI8510_32" (ByVal hEvent As IntPtr) As Boolean  ' 释放内核事件对象
 

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
    Public Const PCI8510_REG_MEM_PLXCHIP = &H0  ' 0号寄存器对应PLX芯片所使用的内存模式基地址(使用LinearAddr)
    Public Const PCI8510_REG_IO_PLXCHIP = &H1  ' 1号寄存器对应PLX芯片所使用的IO模式基地址(使用PhysAddr)


End Module
