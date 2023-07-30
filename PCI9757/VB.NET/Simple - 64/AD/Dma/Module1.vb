Module Module1


    Public Const MAX_AD_CHANNELS = 4 ' 定义最大通道数
    Public Const SEGMENT_COUNT = 32
    Public Const HALF_SIZE_WORDS = 4096
    Dim hDmaEvent As Int32

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
        Dim hDevice As Int32
        Dim DeviceID As Int32
        Dim ADPara As New PCI9757_PARA_AD ' 硬件参数     
        Dim DMAStatus As New PCI9757_STATUS_DMA ' DMA状态参数

        Dim Index As Int32
        Dim ADData As UInt32
        Dim fVolt As Single
        Dim InputRange As Int32
        Dim ADBuffer(0 To SEGMENT_COUNT - 1, 0 To HALF_SIZE_WORDS - 1) As Int32  ' 分配缓冲区(存储原始数据)
        Dim nTChannel As UInt32
        Dim ChannelCount As Int32
        Dim nCurrentChannel As Int32
        Dim bFirstWait As Boolean
        Dim g_ulSegmentID As UInt32
        Dim i As UInt32

        DeviceID = 0
        hDevice = PCI9757_CreateDevice(DeviceID) ' 创建设备对象
        If (hDevice = INVALID_HANDLE_VALUE) Then
            Console.WriteLine("Create Error...")
            _getch()
            Exit Sub
        End If

        InputRange = SelectInputRange() ' 要求用户从键盘上选择输入量程


        ' 预置硬件参数
        ADPara.bChannelArray0 = True ' 允许通道0采样
        ADPara.bChannelArray1 = True ' 允许通道1采样
        ADPara.bChannelArray2 = True ' 允许通道2采样
        ADPara.bChannelArray3 = True ' 允许通道3采样      

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

        hDmaEvent = PCI9757_CreateSystemEvent()
        If (PCI9757_InitDeviceDmaAD(hDevice, hDmaEvent, ADBuffer, 4096, SEGMENT_COUNT, HALF_SIZE_WORDS, ADPara) = False) Then ' 初始化硬件
            Console.WriteLine("PCI9757_InitDeviceDmaAD error")
            _getch()
            GoTo ExitRead1
        End If

        ChannelCount = 4

        PCI9757_StartDeviceDmaAD(hDevice) ' 启动设备
        While (_kbhit() = 0)
            bFirstWait = True
            While (True) ' 查询当前物理缓冲区数据是否已准备就绪
                If (WaitForSingleObject(hDmaEvent, 100) = 0) Then
                    GoTo ReadData ' 等待DMA事件
                Else
                    If (bFirstWait) Then
                        Console.WriteLine("请等待，您可以按任意键退出，但请不要直接关闭窗口强制退出...")
                        bFirstWait = False
                    End If
                    If (_kbhit() <> 0) Then GoTo ExitRead0
                End If
            End While
ReadData:
            If (PCI9757_GetDevStatusDmaAD(hDevice, DMAStatus) = False) Then
                Console.WriteLine("Get Device Status Error...")
                GoTo ExitRead0
            End If

            If (DMAStatus.bBufferOverflow) Then
                Console.WriteLine("DMA Overflow...")
                ' Sleep(3000) ' 当溢出时,为了让用户看得清楚,延时3秒
            End If

            For i = 0 To SEGMENT_COUNT - 1 Step 1
                If (DMAStatus.bSegmentSts(g_ulSegmentID)) Then
                    nTChannel = 0
                    For Index = 0 To 63 Step 1

                        If (_kbhit() <> 0) Then GoTo ExitRead0
                        ' 将原码高4位屏蔽掉
                        ADData = ADBuffer(g_ulSegmentID, Index) And 65535
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
                        Console.Write("CH{0}={1}  ", nCurrentChannel, fVolt.ToString("0.00")) ' 显示电压值	
                        If ((Index + 1) Mod ChannelCount = 0) Then
                            nTChannel = 0
                            Console.WriteLine("")
                        End If
                     
                    Next Index ' for(Index=0 Index<64 Index++)

            If (PCI9757_SetDevStatusDmaAD(hDevice, g_ulSegmentID) = False) Then
                Console.WriteLine("PCI9757_SetDevStatusDmaAD error")
                _getch()
                GoTo ExitRead0
            End If

            g_ulSegmentID = (g_ulSegmentID + 1) Mod SEGMENT_COUNT
                End If
            Next i ' end of for(SegmentID=0 SegmentID<SEGMENT_COUNT SegmentID++)
        End While ' end of while(!kbhit())
ExitRead0:
        _getch()
        PCI9757_ReleaseDeviceDmaAD(hDevice) ' 释放AD
        PCI9757_ReleaseSystemEvent(hDmaEvent)
ExitRead1:
        PCI9757_ReleaseDevice(hDevice) ' 释放设备对象
        _getch()
    End Sub

End Module
