Module Module1

    ' 获取用户选择的输入量程
    Public Function SelectInputRange() As Int32
        Dim InputRange As Int32
Repeat:
        Console.WriteLine()
        Console.WriteLine("0.-10V  ～ +10V")
        Console.WriteLine("1.-5V    ～ +5V")
        Console.WriteLine("2.-2.5V    ～ +2.5V")
        Console.WriteLine("3.0    ～ +10V")
        Console.WriteLine("4.0    ～ +5V")
        Console.WriteLine("Please Select Input Range(0-4):")
        InputRange = Console.ReadLine()
        If (InputRange < 0 Or InputRange > 4) Then
            GoTo Repeat ' 判断用户选择的量程是否合法，不合法，则重新选择
        End If

        SelectInputRange = InputRange
    End Function

    Sub Main()

        Dim ADStatus As PCI9757_STATUS_AD
        Dim hDevice As Int32
        Dim DeviceID As Int32
        Dim ADPara As New PCI9757_PARA_AD ' 硬件参数     
        Dim nReadSizeWords As Int32   ' 每次读取AD数据的长度(字)
        Dim nRetSizeWords As Int32
        Dim Index As Int32
        Dim ADData As UInt32
        Dim fVolt As Single
        Dim InputRange As Int32
        Dim bFirstWait As Boolean
        Dim ADBuffer(0 To 4095) As Int32  ' 分配缓冲区(存储原始数据)
        Dim nTChannel As UInt32

        DeviceID = 0
        hDevice = PCI9757_CreateDevice(DeviceID) ' 创建设备对象
        If (hDevice = INVALID_HANDLE_VALUE) Then
            Console.WriteLine("Create Error...")
            _getch()
            Exit Sub
        End If

        InputRange = SelectInputRange() ' 要求用户从键盘上选择输入量程

        ' 预置硬件参数
        ADPara.bChannelArray0 = 1 ' 允许通道0采样
        ADPara.bChannelArray1 = 1 ' 允许通道1采样
        ADPara.bChannelArray2 = 1 ' 允许通道2采样
        ADPara.bChannelArray3 = 1 ' 允许通道3采样      

        ADPara.InputRange0 = InputRange ' 允许通道0采样
        ADPara.InputRange1 = InputRange ' 允许通道1采样
        ADPara.InputRange2 = InputRange ' 允许通道2采样
        ADPara.InputRange3 = InputRange ' 允许通道3采样         

        ADPara.Frequency = 10000 ' 采样频率(Hz)
        ADPara.TriggerSource = PCI9757_TRIGMODE_SOFT ' 触发源
        ADPara.TriggerType = PCI9757_TRIGTYPE_EDGE ' 触发类型为边沿触发	
        ADPara.TriggerDir = PCI9757_TRIGDIR_NEGATIVE ' 触发方向为负向
        ADPara.TrigLevelVolt = 2000
        ADPara.TrigWindow = 40 ' 触发灵敏度
        ADPara.ClockSource = PCI9757_CLOCKSRC_IN ' 时钟源选用板内时钟源

        If (PCI9757_InitDeviceProAD(hDevice, ADPara) = False) Then ' 初始化硬件
            Console.WriteLine("PCI9757_InitDeviceProAD...")
            _getch()
            Exit Sub
        End If

        nReadSizeWords = 4096 ' 将数据长度字转换为双字	

        If (PCI9757_StartDeviceProAD(hDevice) = False) Then ' 启动设备
            Console.WriteLine("StartDeviceAD Error...")
            _getch()
        End If

        bFirstWait = True ' 为每次等待只显示一次提示
        While (_kbhit() = 0)
            bFirstWait = True
            While (True) ' 查询当前物理缓冲区数据是否已准备就绪
                If (PCI9757_GetDevStatusProAD(hDevice, ADStatus) = False) Then
                    Console.WriteLine("Get Device Status Error...")
                    GoTo ExitRead0
                End If

                If (ADStatus.bHalf) Then
                    GoTo ReadData  ' 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据
                Else
                    If (bFirstWait) Then
                        Console.WriteLine("Wait...")
                        bFirstWait = False
                    End If
                End If
            End While

ReadData:
            PCI9757_ReadDeviceProAD_Half(hDevice, ADBuffer, nReadSizeWords, nRetSizeWords)

            nTChannel = 0
            For Index = 0 To 63 Step 1
                If (_kbhit() <> 0) Then GoTo ExitRead0
                ' 将原码高2位屏蔽掉
                ADData = ADBuffer(Index) And 65535
                Select Case (InputRange)
                    Case PCI9757_INPUT_N10000_P10000mV ' -10V - +10V
                        fVolt = (20000.0 / 65536) * ADData - 10000.0
                    Case PCI9757_INPUT_N5000_P5000mV ' -5V - +5V
                        fVolt = (10000.0 / 65536) * ADData - 5000.0
                    Case PCI9757_INPUT_N2500_P2500mV ' -2.5V - +2.5V
                        fVolt = (5000.0 / 65536) * ADData - 2500.0
                    Case PCI9757_INPUT_0_P10000mV ' 0 - +10V
                        fVolt = (10000.0 / 65536) * ADData
                    Case PCI9757_INPUT_0_P5000mV ' 0 - +5V
                        fVolt = (5000.0 / 65536) * ADData
                    Case Else
                End Select
                Console.Write("CH{0}={1}   ", nTChannel, fVolt.ToString("0.00")) ' 显示电压值
                nTChannel = nTChannel + 1
                If (nTChannel >= 4) Then
                    nTChannel = 0
                    Console.WriteLine("") ' 到末通道时换行
                End If

            Next Index
        End While


ExitRead0:
        PCI9757_ReleaseDeviceProAD(hDevice) ' 释放AD
        PCI9757_ReleaseDevice(hDevice) ' 释放设备对象


    End Sub

End Module
