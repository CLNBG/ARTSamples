Module Module1

    Public Const MAX_AD_CHANNEL = 8
    Public Const SEGMENT_COUNT = 32
    Public Const HALF_SIZE_WORDS = 4096
    Dim InputRange As Int32
    Dim ADBuffer(0 To 31, 0 To 4095) As UInt32  ' 分配缓冲区(存储原始数据)
    Dim hDevice As IntPtr
    Dim DeviceID As Int32
    Dim ADPara As PCI8510_PARA_AD  ' 硬件参数
    Dim nReadSizeWords As Int32    ' 每次读取AD数据的长度(字)
    Dim nRetSizeWords As Int32
    Dim nChannel As Int32 = 0
    Dim Index As Int32 = 0
    Dim fVolt As Single
    Dim DMAStatus As PCI8510_STATUS_DMA
    Dim bFirstWait As Boolean
    Dim hDmaEvent As IntPtr
    Dim g_ulSegmentID As UInt32
    Dim DMASta(0 To SEGMENT_COUNT - 1) As Int32
    Dim i As Int32


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


        hDmaEvent = PCI8510_CreateSystemEvent
        If (PCI8510_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer, 4096, 32, 4096, ADPara) = False) Then ' 初始化硬件


            Console.WriteLine("PCI8510_InitDeviceDmaAD Error...")
            _getch()
            GoTo ExitRead1
        End If

        PCI8510_StartDeviceDmaAD(hDevice) ' 启动设备
        While (_kbhit() = 0)

            bFirstWait = True
            While (True) ' 查询当前物理缓冲区数据是否已准备就绪

                If (WaitForSingleObject(hDmaEvent, 100) = 0) Then
                    GoTo Read ' 等待DMA事件
                Else

                    If (bFirstWait) Then

                        Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...\n")
                        bFirstWait = False
                    End If
                    If (_kbhit() <> 0) Then
                        GoTo ExitRead0
                    End If

                End If

            End While
Read:
            If (PCI8510_GetDevStatusDmaAD(hDevice, DMAStatus) = False) Then

                Console.WriteLine("Get Device Status Error...\n")
                GoTo ExitRead0
            End If
            DMASta(0) = DMAStatus.bSegmentSts0
            DMASta(1) = DMAStatus.bSegmentSts1
            DMASta(2) = DMAStatus.bSegmentSts2
            DMASta(3) = DMAStatus.bSegmentSts3
            DMASta(4) = DMAStatus.bSegmentSts4
            DMASta(5) = DMAStatus.bSegmentSts5
            DMASta(6) = DMAStatus.bSegmentSts6
            DMASta(7) = DMAStatus.bSegmentSts7
            DMASta(8) = DMAStatus.bSegmentSts8
            DMASta(9) = DMAStatus.bSegmentSts9

            DMASta(10) = DMAStatus.bSegmentSts10
            DMASta(11) = DMAStatus.bSegmentSts11
            DMASta(12) = DMAStatus.bSegmentSts12
            DMASta(13) = DMAStatus.bSegmentSts13
            DMASta(14) = DMAStatus.bSegmentSts14
            DMASta(15) = DMAStatus.bSegmentSts15
            DMASta(16) = DMAStatus.bSegmentSts16
            DMASta(17) = DMAStatus.bSegmentSts17
            DMASta(18) = DMAStatus.bSegmentSts18
            DMASta(19) = DMAStatus.bSegmentSts19

            DMASta(20) = DMAStatus.bSegmentSts20
            DMASta(21) = DMAStatus.bSegmentSts21
            DMASta(22) = DMAStatus.bSegmentSts22
            DMASta(23) = DMAStatus.bSegmentSts23
            DMASta(24) = DMAStatus.bSegmentSts24
            DMASta(25) = DMAStatus.bSegmentSts25
            DMASta(26) = DMAStatus.bSegmentSts26
            DMASta(27) = DMAStatus.bSegmentSts27
            DMASta(28) = DMAStatus.bSegmentSts28
            DMASta(29) = DMAStatus.bSegmentSts29

            DMASta(30) = DMAStatus.bSegmentSts30
            DMASta(31) = DMAStatus.bSegmentSts31

           



            If (DMAStatus.bBufferOverflow) Then

                Console.WriteLine("DMA Overflow...\n")
                ' Sleep(3000) ' 当溢出时,为了让用户看得清楚,延时3秒
            End If
            For i = 0 To SEGMENT_COUNT - 1 Step 1

                If (DMASta(g_ulSegmentID)) Then
                    For Index = 0 To 63 Step 1





                        For nChannel = 0 To 7 Step 1

                            If (_kbhit() <> 0) Then
                                GoTo ExitRead0
                            End If

                            ' 将原码转换为电压值
                            Select Case (InputRange)

                                Case PCI8510_INPUT_0_P5000mV ' -5V - +5V
                                    fVolt = ((5000.0 / 65536) * (ADBuffer(g_ulSegmentID, Index) And &HFFFF))

                                Case PCI8510_INPUT_0_P10000mV ' -10V - +10V
                                    fVolt = ((10000.0 / 65536) * (ADBuffer(g_ulSegmentID, Index) And &HFFFF))

                                Case PCI8510_INPUT_N5000_P5000mV ' -5V - +5V
                                    fVolt = ((10000.0 / 65536) * (ADBuffer(g_ulSegmentID, Index) And &HFFFF) - 5000.0)

                                Case PCI8510_INPUT_N10000_P10000mV ' -10V - +10V
                                    fVolt = ((20000.0 / 65536) * (ADBuffer(g_ulSegmentID, Index) And &HFFFF) - 10000.0)
                            End Select
                            Console.Write("CH" + Format(nChannel, "d") + "=" + Format(fVolt, "0.00   ")) ' 显示电压值
                            If ((nChannel + 1) Mod 4 = 0) Then

                                Console.WriteLine("")  ' 显示电压值
                            End If
                            Index = Index + 1



                        Next nChannel ' for(Index=0 Index<64 Index++)

                        Index = Index - 1
                    Next Index
                    If (PCI8510_SetDevStatusDmaAD(hDevice, g_ulSegmentID) = False) Then

                        Console.Write("PCI8510_SetDevStatusDmaAD  Error...")
                        _getch()
                        GoTo ExitRead0
                    End If

                    g_ulSegmentID = (g_ulSegmentID + 1) Mod SEGMENT_COUNT
                End If ' for(Index=0  Index<64  Index++)
            Next i
        End While

ExitRead0:
        _getch()
        PCI8510_ReleaseDeviceDmaAD(hDevice) ' 释放AD
        PCI8510_ReleaseSystemEvent(hDmaEvent)
ExitRead1:
        PCI8510_ReleaseDevice(hDevice)  ' 释放设备对象
        _getch()
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