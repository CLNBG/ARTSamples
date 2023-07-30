Module PCI8501

    '***********************************************************
    ' 用于AD采集的参数结构
    Structure PCI8501_PARA_AD
        Dim Frequency As Int32          ' 采集频率,单位为Hz,最高采样率4000000Hz        
        Dim InputRange0 As Int32
        Dim InputRange1 As Int32
        Dim InputRange2 As Int32
        Dim InputRange3 As Int32
        Dim InputRange4 As Int32
        Dim InputRange5 As Int32
        Dim InputRange6 As Int32
        Dim InputRange7 As Int32
        Dim Gains0 As Int32
        Dim Gains1 As Int32
        Dim Gains2 As Int32
        Dim Gains3 As Int32
        Dim Gains4 As Int32
        Dim Gains5 As Int32
        Dim Gains6 As Int32
        Dim Gains7 As Int32
        Dim M_Length As Int32
        Dim N_Length As Int32
        Dim TriggerMode As Int32        ' 触发模式选择(软件触发、后触发)
        Dim TriggerSource As Int32        ' 触发源选择
        Dim TriggerDir As Int32         ' 触发方向选择(正向/负向触发)
        Dim TrigLevelVolt As Int32       '  触发电平(±10000mV)
        Dim ClockSource As Int32        ' 时钟源选择(内/外时钟源)
        Dim OutClockSource As Int32        '时钟输入输出源
        Dim bClockSourceDir As Int32     '是否将时钟信号输出到PXI总线,=TRUE:允许输出, =FALSE:允许输入

    End Structure

    ' AD硬件参数PCI8501_PARA_AD中的InputRange[x]模拟量输入范围所使用的选项
    Public Const PCI8501_INPUT_N10000_P10000mV = &H0    ' ±10000mV
    Public Const PCI8501_INPUT_N5000_P5000mV = &H1    ' ±5000mV
    Public Const PCI8501_INPUT_N2500_P2500mV = &H2    ' ±2500mV
    Public Const PCI8501_INPUT_0_P10000mV = &H3   ' 0～10000mV
    Public Const PCI8501_INPUT_0_P5000mV = &H4     '0～5000mV

    '***********************************************************
    'AD参数PCI8501_PARA_AD中的Gains[x]使用的硬件增益选项
    Public Const PCI8501_GAINS_1MULT = &H0    '1倍增益
    Public Const PCI8501_GAINS_2MULT = &H1    '2倍增益
    Public Const PCI8501_GAINS_5MULT = &H2    '4倍增益
    Public Const PCI8501_GAINS_10MULT = &H3   '8倍增益

    '***********************************************************
    ' AD硬件参数PCI8501_PARA_AD中的TriggerMode成员变量所使用AD触发模式选项
    Public Const PCI8501_TRIGMODE_MIDL = &H0 ' 中间触发
    Public Const PCI8501_TRIGMODE_POST = &H1 ' 后触发
    Public Const PCI8501_TRIGMODE_PRE = &H2 ' 预触发
    Public Const PCI8501_TRIGMODE_DELAY = &H3 ' 硬件后触发(属于外触发)
    '***********************************************************
    ' AD硬件参数PCI8501_PARA_AD中的TriggerSource触发源信号所使用的选项
    Public Const PCI8501_TRIGMODE_SOFT = &H0            ' 软件触发
    Public Const PCI8501_TRIGSRC_DTR = &H1            ' 选择外部DTR触发源
    Public Const PCI8501_TRIGSRC_ATR = &H2            ' 选择外部ATR触发源
    Public Const PCI8501_TRIGSRC_TRIGGER = &H3      ' Trigger信号触发（用于多卡同步）

    '***********************************************************
    ' AD硬件参数PCI8501_PARA_AD中的TriggerDir触发方向所使用的选项
    Public Const PCI8501_TRIGDIR_NEGATIVE = &H0   '下降沿触发
    Public Const PCI8501_TRIGDIR_POSITIVE = &H1   ' 上升沿触发
    Public Const PCI8501_TRIGDIR_NEGAT_POSIT = &H2   '上下边沿均触发

    '***********************************************************
    'AD硬件参数PCI8501_PARA_AD中的ClockSource时钟源所使用的选项
    Public Const PCI8501_CLOCKSRC_IN = &H0 ' 使用内部时钟
    Public Const PCI8501_CLOCKSRC_OUT = &H1 ' 使用外部时钟

    '***********************************************************
    ' AD硬件参数PCI8501_PARA_AD中的OutClockSource同步信号触发源所使用的选项
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER0 = &H0       ' 选择同步TRIG0信号触发
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER1 = &H1      ' 选择同步TRIG1信号触发
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER2 = &H2      ' 选择同步TRIG2信号触发		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER3 = &H3      ' 选择同步TRIG3信号触发		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER4 = &H4      ' 选择同步TRIG4信号触发		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER5 = &H5      ' 选择同步TRIG5信号触发		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER6 = &H6      ' 选择同步TRIG6信号触发		
    Public Const PCI8501_OUTCLOCKSRC_TRIGGER7 = &H7      ' 选择同步TRIG7信号触发	
    ' *************************************************************************************
    ' 用于AD采样的实际硬件参数
    Structure PCI8501_STATUS_AD

        Dim bADEanble As Int32              'AD是否已经使能，=TRUE:表示已使能，=FALSE:表示未使能
        Dim bTrigger As Int32                  'AD是否被触发，=TRUE:表示已被触发，=FALSE:表示未被触发
        Dim bComplete As Int32      'AD是否整个转换过程是否结束，=TRUE:表示已结束，=FALSE:表示未结束
        Dim bAheadTrig As Int32       'AD触发点是否提前，=TRUE:表示触发点提前，=FALSE:表示触发点未提前
        Dim lEndAddr As Int32           '数据完成的结束地址

    End Structure


    '######################## 设备对象管理函数 ##############################
    Declare Function PCI8501_CreateDevice Lib "PCI8501_64" (Optional ByVal DeviceLgcID As Int32 = 0) As Int32     ' 创建设备对象(使用设备逻辑号)
    Declare Function PCI8501_GetDeviceCount Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32             ' 取得PCI8501在系统中的设备数量
    Declare Function PCI8501_ListDeviceDlg Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32                               ' 用对话框列表系统当中的所有USB2815设备
    Declare Function PCI8501_ReleaseDevice Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32              ' 设备句柄

    '####################### AD数据读取函数 #################################
    Declare Function PCI8501_GetDDR2Length Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pulDDR2Length As UInt32) As Boolean            ' 获取DDR2的存储长度
    Declare Function PCI8501_ADCalibration Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal InputRange As Int32) As Int32
    Declare Function PCI8501_InitDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32  '  初始化设备,当返回TRUE后,设备即刻开始传输.					
    Declare Function PCI8501_StartDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_SetDeviceTrigAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_GetDevStatusAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_STATUS_AD) As Int32

    Declare Function PCI8501_ReadDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal ADBuffer() As UInt16, ByVal nReadSizeWords As Int32, ByVal ulStartAddr As Int32, ByRef nRetSizeWords As Int32) As Int32 'DMA方式读AD数据
    Declare Function PCI8501_StopDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32
    Declare Function PCI8501_ReleaseDeviceAD Lib "PCI8501_64" (ByVal hDevice As Int32) As Int32

    '################# AD的硬件参数操作函数 ########################	

    Declare Function PCI8501_SaveParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32
    Declare Function PCI8501_LoadParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32
    Declare Function PCI8501_ResetParaAD Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pADPara As PCI8501_PARA_AD) As Int32 ' 将AD采样参数恢复至出厂默认值


    '####################### 数字I/O输入输出函数 #################################

    Declare Function PCI8501_GetDeviceDI Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal bDISts() As Byte) As Int32          ' 开关输入状态(注意: 必须定义为8个字节元素的数组)

    Declare Function PCI8501_SetDeviceDO Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal bDOSts() As Byte) As Int32          ' 开关输出状态(注意: 必须定义为8个字节元素的数组)


    '############################################################################
    '################# 内存映射寄存器直接操作及读写函数 ########################
    Declare Function PCI8501_GetDeviceBar Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal pbPCIBar() As UInt16) As Int32
    Declare Function PCI8501_GetDevVersion Lib "PCI8501_64" (ByVal hDevice As Int32, ByVal pulFmwVersion As UInt32, ByVal pulDriverVersion As UInt32) As Int32
    Declare Function PCI8501_WriteRegisterByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As Byte) As Int32
    Declare Function PCI8501_WriteRegisterWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As UInt16) As Int32
    Declare Function PCI8501_WriteRegisterULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32, ByVal Value As Int32) As Int32
    Declare Function PCI8501_ReadRegisterByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadRegisterWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadRegisterULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pbLinearAddr As UInt32, ByRef OffsetBytes As UInt32) As Int32
    '################# I/O端口直接操作及读写函数 ########################
    Declare Function PCI8501_WritePortByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As Byte) As Int32
    Declare Function PCI8501_WritePortWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As UInt16) As Int32
    Declare Function PCI8501_WritePortULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt16, ByRef OffsetBytes As UInt32, ByVal Value As Int32) As Int32
    Declare Function PCI8501_ReadPortByte Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadPortWord Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    Declare Function PCI8501_ReadPortULong Lib "PCI8501_64" (ByVal hDevice As Int32, ByRef pPort As UInt32, ByRef OffsetBytes As UInt32) As Int32
    '########################### 附加操作函数 ######################################
    Declare Function PCI8501_CreateSystemEvent Lib "PCI8501_64" () As Int32     '创建内核系统事件对象
    Declare Function PCI8501_ReleaseSystemEvent Lib "PCI8501_64" (ByVal hEvent As Int32) As Boolean           '释放内核事件对象

    '***********************************************************

    ' 本卡可以支持的各种FIFO存储器的长度(点)
    Public Const FIFO_IDT7202_LENGTH = 1024
    Public Const FIFO_IDT7203_LENGTH = 2048
    Public Const FIFO_IDT7204_LENGTH = 4096
    Public Const FIFO_IDT7205_LENGTH = 8192
    Public Const FIFO_IDT7206_LENGTH = 16384
    Public Const FIFO_IDT7207_LENGTH = 32768

End Module
