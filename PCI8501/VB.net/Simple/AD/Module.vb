Module Module1


    Public Const AD_DATA_LEN = 1024 * 1024  ' 定义缓冲区页段数为32个

    
    Sub Main()
        Dim hDevice As Int32  ' 设备对象句柄
        Dim DeviceLgcID As Int32   ' 物理设备ID号(由板上JP1决定)
        Dim ADBuffer(AD_DATA_LEN) As UInt16
        Dim bFirstWait As Boolean = True   ' 为每次等待只显示一次提示
        Dim ADPara As PCI8501_PARA_AD  ' 初始化AD的参数结构
        Dim ADStatus As PCI8501_STATUS_AD ' 初始化AD的参数结构
        Dim nReadSizeWords As Int32
        Dim nRetSizeWords As Int32
        Dim nReadSize As Int32
        Dim nCurrentChannel As Int32 = 0
        Dim nADChannel As Int32 = 0
        Dim ADData As UInt16
        Dim fVolt As Single     ' 将AD原始数据转换为电压值
        Dim g_ulSegmentID As Int32 = 0
        Dim Index As Int32
        Dim InputRange As Int32
        Dim pulDDR2Length As UInt32

        DeviceLgcID = 0  ' 设备ID号, 假设系统中只有一个USB2830设备，即DeviceLgcID=0
        hDevice = PCI8501_CreateDevice(DeviceLgcID) ' 创建设备对象
        If (hDevice = (-1)) Then
            Console.WriteLine("Create Device Error")
            _getch()
            Exit Sub
        End If
        PCI8501_GetDDR2Length(hDevice, pulDDR2Length)
        InputRange = SelectInputRange()

        ADPara.Frequency = 100000 ' 采样频率(Hz)   
        ADPara.InputRange0 = InputRange  ' 模拟通道0输入量程
        ADPara.InputRange1 = InputRange  ' 模拟通道1输入量程
        ADPara.InputRange2 = InputRange  ' 模拟通道2输入量程
        ADPara.InputRange3 = InputRange  ' 模拟通道3输入量程
        ADPara.InputRange4 = InputRange  ' 模拟通道4输入量程
        ADPara.InputRange5 = InputRange  ' 模拟通道5输入量程
        ADPara.InputRange6 = InputRange  ' 模拟通道6输入量程
        ADPara.InputRange7 = InputRange  ' 模拟通道7输入量程
        ADPara.Gains0 = 0
        ADPara.Gains1 = 0
        ADPara.Gains2 = 0
        ADPara.Gains3 = 0
        ADPara.Gains4 = 0
        ADPara.Gains5 = 0
        ADPara.Gains6 = 0
        ADPara.Gains7 = 0
        ADPara.M_Length = 0                     ' M长度
        ADPara.N_Length = 1024              ' N长度
        ADPara.TriggerMode = PCI8501_TRIGMODE_MIDL   '硬件中间触发(包括预触发、后触发功能)
        ADPara.TriggerSource = PCI8501_TRIGMODE_SOFT ' 软件触发
        ADPara.TriggerDir = PCI8501_TRIGDIR_NEGATIVE  ' 下降沿触发
        ADPara.TrigLevelVolt = 0    ' 触发灵敏度	
        ADPara.ClockSource = PCI8501_CLOCKSRC_IN ' 时钟源选用板内时钟源
        ADPara.bClockSourceDir = False
        ADPara.OutClockSource = PCI8501_OUTCLOCKSRC_TRIGGER0

        If (PCI8501_InitDeviceAD(hDevice, ADPara) = 0) Then ' 初始化硬件
            Console.WriteLine("PCI8501_InitDeviceAD Error...")
            _getch()
            GoTo ExitRead  ' 如果创建设备对象失败，则返回
        End If

        nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8  'M加N长度为读取数据的长度

        Dim ulStartAddr As Int32 = 0
        While (_kbhit() = 0)

            If (PCI8501_StartDeviceAD(hDevice) = 0) Then ' 启动设备

                Console.WriteLine("StartDeviceAD Error...")
                _getch()
            End If
            If (PCI8501_SetDeviceTrigAD(hDevice) = 0) Then     '触发AD

                Console.WriteLine("SetDeviceTrigAD Error...")
                _getch()
            End If
            ulStartAddr = 0
            nReadSize = nReadSizeWords
            bFirstWait = True
            While (True)  ' 查询当前物理缓冲区数据是否已准备就绪
                If (PCI8501_GetDevStatusAD(hDevice, ADStatus) = 0) Then
                    Console.WriteLine("GetDevStatusAD error...")
                    _getch()
                    GoTo ExitRead
                End If

                If (ADStatus.bComplete = 1) Then
                    GoTo Read  ' 若板载FIFO存储器数据量达到半满以上，则退出状态查询,开始读取半满数据
                Else

                    If (bFirstWait = True) Then
                        Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...")
                        bFirstWait = False
                    End If
                    If (_kbhit() <> 0) Then
                        GoTo ExitRead  ' 如果用户按键，则退出
                    End If
                End If
            End While

Read:
            If (PCI8501_StopDeviceAD(hDevice) = 0) Then    '停止设备

                Console.WriteLine("StopDeviceAD Error...")
                _getch()
            End If
            Dim offsetAddr As Int32
            offsetAddr = ADStatus.lEndAddr - nReadSizeWords * 2 + 4
            If (offsetAddr < 0) Then

                ulStartAddr = pulDDR2Length * 1024 * 1024 + (ADStatus.lEndAddr - nReadSizeWords * 2 + 4)

            Else
                ulStartAddr = offsetAddr
            End If

            While (nReadSize > 0)

                If (PCI8501_ReadDeviceAD(hDevice, ADBuffer, AD_DATA_LEN, ulStartAddr, nRetSizeWords) = 0) Then

                    Console.WriteLine("ReadDeviceDmaAD error...")
                    _getch()
                    GoTo ExitRead
                End If
                nReadSize = nReadSize - AD_DATA_LEN
                ulStartAddr = ulStartAddr + AD_DATA_LEN * 2

                For Index = 0 To 63 Step 0

                    For nADChannel = 0 To 7 Step 1

                        If (_kbhit() <> 0) Then
                            GoTo ExitRead
                        End If


                        ADData = (ADBuffer(Index)) And 65535
                        ' 将原码转换为电压值
                        Select Case InputRange

                            Case PCI8501_INPUT_N10000_P10000mV   '-10000mV - +10000mV
                                fVolt = ((20000.0 / 65536) * ADData - 10000.0)

                            Case PCI8501_INPUT_N5000_P5000mV   ' -5000mV - +5000mV
                                fVolt = ((10000.0 / 65536) * ADData - 5000.0)

                            Case PCI8501_INPUT_N2500_P2500mV   ' -2500mV - +2500mV
                                fVolt = ((5000.0 / 65536) * ADData - 2500.0)

                            Case PCI8501_INPUT_0_P10000mV      '0mV - +10000mV
                                fVolt = ((10000.0 / 65536) * ADData)

                            Case PCI8501_INPUT_0_P5000mV         '0mV - +5000mV
                                fVolt = ((5000.0 / 65536) * ADData)
                        End Select
                        Console.Write("CH{0}={1}    ", nADChannel, fVolt.ToString("0.00"))   ' 显示电压值
                        Index = Index + 1
                        If ((nADChannel + 1) Mod 4 = 0) Then
                            Console.WriteLine("")
                        End If
                    Next nADChannel

                Next Index


            End While
        End While



ExitRead:
        PCI8501_StopDeviceAD(hDevice)    '停止AD
        PCI8501_ReleaseDeviceAD(hDevice)    ' 释放AD
        PCI8501_ReleaseDevice(hDevice)     ' 释放设备对象

    End Sub


    '''''''''''''''''''''''''''
    ' 获取用户选择的输入量程
    Public Function SelectInputRange() As Int32
        Dim InputRange As Int32
Repeat:
        Console.WriteLine("")
        Console.WriteLine("0. -10V ～ +10V")
        Console.WriteLine("1. -5V ～ +5V")
        Console.WriteLine("2. -2500mV ～ +2500mV")
        Console.WriteLine("3. 0mV ～ +10000mV")
        Console.WriteLine("4. 0mV ～ +5000mV")
        Console.WriteLine("Please Select Input Range[0-4]:")
        InputRange = Convert.ToInt32(Console.ReadLine())
        If (InputRange < 0 Or InputRange > 4) Then

            GoTo Repeat ' 判断用户选择的量程是否合法，不合法，则重新选择

        End If
        Return InputRange

    End Function

End Module

  