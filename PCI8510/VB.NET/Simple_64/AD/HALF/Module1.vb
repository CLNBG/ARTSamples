Module Module1

    Public Const MAX_AD_CHANNEL = 8
    Public Const AD_DATA_LEN = 4096
    Dim InputRange As Int32
    Dim ADBuffer(0 To 4095) As UInt32  ' 分配缓冲区(存储原始数据)
    Dim hDevice As IntPtr
    Dim DeviceID As Int32
    Dim ADPara As PCI8510_PARA_AD  ' 硬件参数
    Dim nReadSizeWords As Int32    ' 每次读取AD数据的长度(字)
    Dim nRetSizeWords As Int32
    Dim nChannel As Int32 = 0
    Dim Index As Int32 = 0
    Dim fVolt As Single
    Dim bFirstWait As Boolean
    Dim ADStatus As PCI8510_STATUS_AD


    Sub Main()

       

        DeviceID = 0
        hDevice = PCI8510_CreateDevice(DeviceID)  ' 创建设备对象
        If (hDevice = INVALID_HANDLE_VALUE) Then

            Console.WriteLine("PCI8510_CreateDevice Error...")
            _getch()
            Return
        End If
        InputRange = SelectInputRange()  ' 要求用户从键盘上选择输入量程



        ' 预置硬件参数
        ADPara.Gains0 = PCI8510_GAINS_1MULT   ' 模拟通道0输入量程
        ADPara.Gains1 = PCI8510_GAINS_1MULT   ' 模拟通道1输入量程
        ADPara.Gains2 = PCI8510_GAINS_1MULT   ' 模拟通道2输入量程
        ADPara.Gains3 = PCI8510_GAINS_1MULT   ' 模拟通道3输入量程	
        ADPara.Gains4 = PCI8510_GAINS_1MULT   ' 模拟通道4输入量程
        ADPara.Gains5 = PCI8510_GAINS_1MULT   ' 模拟通道5输入量程
        ADPara.Gains6 = PCI8510_GAINS_1MULT   ' 模拟通道6输入量程
        ADPara.Gains7 = PCI8510_GAINS_1MULT   ' 模拟通道7输入量程	
        ADPara.Frequency = 10000  ' 采样频率(Hz)
        ADPara.InputRange = InputRange
        ADPara.TriggerMode = PCI8510_TRIGMODE_SOFT
        ADPara.TriggerSource = PCI8510_TRIGSRC_ATR  ' 触发源
        ADPara.TriggerType = PCI8510_TRIGTYPE_EDGE  ' 触发类型为边沿触发	
        ADPara.TriggerDir = PCI8510_TRIGDIR_NEGATIVE  ' 触发方向为负向
        ADPara.TrigLevelVolt = 0     ' 触发电平
        ADPara.ClockSource = PCI8510_CLOCKSRC_IN  ' 时钟源选用板内时钟源
        ADPara.bClockOutput = False  ' 禁止时钟输出
        ADPara.OutTrigSource = PCI8510_OUTTRIGSRC_TRIGGER0
        ADPara.OutTrigSourceDir = False

        If (PCI8510_InitDeviceProAD(hDevice, ADPara) = False) Then ' 初始化硬件

            Console.WriteLine("PCI8510_InitDeviceProAD...")
            _getch()
            Return
        End If

        nReadSizeWords = AD_DATA_LEN  ' 将读取数据长度

        If (PCI8510_StartDeviceProAD(hDevice) = False) Then ' 启动设备

            Console.WriteLine("StartDeviceAD Error...\n")
            _getch()
        End If

        bFirstWait = True ' 为每次等待只显示一次提示
        While (_kbhit() = 0)

            bFirstWait = True
            While (True) ' 查询当前物理缓冲区数据是否已准备就绪

                If (PCI8510_GetDevStatusProAD(hDevice, ADStatus) = False) Then

                    Console.WriteLine("Get Device Status Error...\n")
                    GoTo ExitRead0
                End If

                If (ADStatus.bHalf) Then

                    GoTo Read ' 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据

                Else

                    If (bFirstWait) Then

                        Console.WriteLine("\nWait...\n")
                        bFirstWait = False
                    End If
                End If
		END WHILE
Read:
            PCI8510_ReadDeviceProAD_Half(hDevice, ADBuffer, nReadSizeWords, nRetSizeWords)

            For Index = 0 To 63 Step 1

                For nChannel = 0 To 7 Step 1

                    If (_kbhit() <> 0) Then
                        GoTo ExitRead0
                    End If

                    '将原码转换为电压值
                    Select Case (InputRange)

                        Case PCI8510_INPUT_0_P5000mV ' -5V - +5V
                            fVolt = ((5000.0 / 65536) * (ADBuffer(Index) And &HFFFF))

                        Case PCI8510_INPUT_0_P10000mV ' -10V - +10V
                            fVolt = ((10000.0 / 65536) * (ADBuffer(Index) And &HFFFF))

                        Case PCI8510_INPUT_N5000_P5000mV ' -5V - +5V
                            fVolt = ((10000.0 / 65536) * (ADBuffer(Index) And &HFFFF) - 5000.0)

                        Case PCI8510_INPUT_N10000_P10000mV ' -10V - +10V
                            fVolt = ((20000.0 / 65536) * (ADBuffer(Index) And &HFFFF) - 10000.0)
                    End Select
                    Console.Write("CH{0} =  {1}  ", nChannel, fVolt.ToString("0.## ")) ' 显示电压值
                    If ((nChannel + 1) Mod 4 = 0) Then

                        Console.WriteLine("") ' 显示电压值
                    End If
                    Index = Index + 1
                Next nChannel
                Index = Index - 1
            Next Index ' for(Index=0  Index<64  Index++)
        End While

     

ExitRead0:
        PCI8510_ReleaseDeviceProAD(hDevice)  ' 释放AD
        PCI8510_ReleaseDevice(hDevice)  ' 释放设备对象
        Return


        '''''''''''''''''''''''''''
        ' 获取用户选择的输入量程
    End Sub
    Function SelectInputRange() As Int32
        Dim InputRange As Int32
Repeat:
        Console.WriteLine("")
        Console.WriteLine("0. 0V ～ +5V")
        Console.WriteLine("1. 0V ～ +10V")
        Console.WriteLine("2. -5V ～ +5V")
        Console.WriteLine("3. -10V ～ +10V")

        Console.WriteLine("Please Select Input Range(0-3):")
        InputRange = Console.ReadLine()
        If (InputRange < 0 Or InputRange > 3) Then
            GoTo Repeat  ' 判断用户选择的量程是否合法，不合法，则重新选择
        End If

        Return InputRange


    End Function
End Module