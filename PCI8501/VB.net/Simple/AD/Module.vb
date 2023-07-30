Module Module1


    Public Const AD_DATA_LEN = 1024 * 1024  ' ���建����ҳ����Ϊ32��

    
    Sub Main()
        Dim hDevice As Int32  ' �豸������
        Dim DeviceLgcID As Int32   ' �����豸ID��(�ɰ���JP1����)
        Dim ADBuffer(AD_DATA_LEN) As UInt16
        Dim bFirstWait As Boolean = True   ' Ϊÿ�εȴ�ֻ��ʾһ����ʾ
        Dim ADPara As PCI8501_PARA_AD  ' ��ʼ��AD�Ĳ����ṹ
        Dim ADStatus As PCI8501_STATUS_AD ' ��ʼ��AD�Ĳ����ṹ
        Dim nReadSizeWords As Int32
        Dim nRetSizeWords As Int32
        Dim nReadSize As Int32
        Dim nCurrentChannel As Int32 = 0
        Dim nADChannel As Int32 = 0
        Dim ADData As UInt16
        Dim fVolt As Single     ' ��ADԭʼ����ת��Ϊ��ѹֵ
        Dim g_ulSegmentID As Int32 = 0
        Dim Index As Int32
        Dim InputRange As Int32
        Dim pulDDR2Length As UInt32

        DeviceLgcID = 0  ' �豸ID��, ����ϵͳ��ֻ��һ��USB2830�豸����DeviceLgcID=0
        hDevice = PCI8501_CreateDevice(DeviceLgcID) ' �����豸����
        If (hDevice = (-1)) Then
            Console.WriteLine("Create Device Error")
            _getch()
            Exit Sub
        End If
        PCI8501_GetDDR2Length(hDevice, pulDDR2Length)
        InputRange = SelectInputRange()

        ADPara.Frequency = 100000 ' ����Ƶ��(Hz)   
        ADPara.InputRange0 = InputRange  ' ģ��ͨ��0��������
        ADPara.InputRange1 = InputRange  ' ģ��ͨ��1��������
        ADPara.InputRange2 = InputRange  ' ģ��ͨ��2��������
        ADPara.InputRange3 = InputRange  ' ģ��ͨ��3��������
        ADPara.InputRange4 = InputRange  ' ģ��ͨ��4��������
        ADPara.InputRange5 = InputRange  ' ģ��ͨ��5��������
        ADPara.InputRange6 = InputRange  ' ģ��ͨ��6��������
        ADPara.InputRange7 = InputRange  ' ģ��ͨ��7��������
        ADPara.Gains0 = 0
        ADPara.Gains1 = 0
        ADPara.Gains2 = 0
        ADPara.Gains3 = 0
        ADPara.Gains4 = 0
        ADPara.Gains5 = 0
        ADPara.Gains6 = 0
        ADPara.Gains7 = 0
        ADPara.M_Length = 0                     ' M����
        ADPara.N_Length = 1024              ' N����
        ADPara.TriggerMode = PCI8501_TRIGMODE_MIDL   'Ӳ���м䴥��(����Ԥ�������󴥷�����)
        ADPara.TriggerSource = PCI8501_TRIGMODE_SOFT ' �������
        ADPara.TriggerDir = PCI8501_TRIGDIR_NEGATIVE  ' �½��ش���
        ADPara.TrigLevelVolt = 0    ' ����������	
        ADPara.ClockSource = PCI8501_CLOCKSRC_IN ' ʱ��Դѡ�ð���ʱ��Դ
        ADPara.bClockSourceDir = False
        ADPara.OutClockSource = PCI8501_OUTCLOCKSRC_TRIGGER0

        If (PCI8501_InitDeviceAD(hDevice, ADPara) = 0) Then ' ��ʼ��Ӳ��
            Console.WriteLine("PCI8501_InitDeviceAD Error...")
            _getch()
            GoTo ExitRead  ' ��������豸����ʧ�ܣ��򷵻�
        End If

        nReadSizeWords = (ADPara.M_Length + ADPara.N_Length) * 8  'M��N����Ϊ��ȡ���ݵĳ���

        Dim ulStartAddr As Int32 = 0
        While (_kbhit() = 0)

            If (PCI8501_StartDeviceAD(hDevice) = 0) Then ' �����豸

                Console.WriteLine("StartDeviceAD Error...")
                _getch()
            End If
            If (PCI8501_SetDeviceTrigAD(hDevice) = 0) Then     '����AD

                Console.WriteLine("SetDeviceTrigAD Error...")
                _getch()
            End If
            ulStartAddr = 0
            nReadSize = nReadSizeWords
            bFirstWait = True
            While (True)  ' ��ѯ��ǰ�������������Ƿ���׼������
                If (PCI8501_GetDevStatusAD(hDevice, ADStatus) = 0) Then
                    Console.WriteLine("GetDevStatusAD error...")
                    _getch()
                    GoTo ExitRead
                End If

                If (ADStatus.bComplete = 1) Then
                    GoTo Read  ' ������FIFO�洢���������ﵽ�������ϣ����˳�״̬��ѯ,��ʼ��ȡ��������
                Else

                    If (bFirstWait = True) Then
                        Console.WriteLine("��ȴ��������԰�������˳������벻Ҫֱ�ӹرմ���ǿ���˳�...")
                        bFirstWait = False
                    End If
                    If (_kbhit() <> 0) Then
                        GoTo ExitRead  ' ����û����������˳�
                    End If
                End If
            End While

Read:
            If (PCI8501_StopDeviceAD(hDevice) = 0) Then    'ֹͣ�豸

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
                        ' ��ԭ��ת��Ϊ��ѹֵ
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
                        Console.Write("CH{0}={1}    ", nADChannel, fVolt.ToString("0.00"))   ' ��ʾ��ѹֵ
                        Index = Index + 1
                        If ((nADChannel + 1) Mod 4 = 0) Then
                            Console.WriteLine("")
                        End If
                    Next nADChannel

                Next Index


            End While
        End While



ExitRead:
        PCI8501_StopDeviceAD(hDevice)    'ֹͣAD
        PCI8501_ReleaseDeviceAD(hDevice)    ' �ͷ�AD
        PCI8501_ReleaseDevice(hDevice)     ' �ͷ��豸����

    End Sub


    '''''''''''''''''''''''''''
    ' ��ȡ�û�ѡ�����������
    Public Function SelectInputRange() As Int32
        Dim InputRange As Int32
Repeat:
        Console.WriteLine("")
        Console.WriteLine("0. -10V �� +10V")
        Console.WriteLine("1. -5V �� +5V")
        Console.WriteLine("2. -2500mV �� +2500mV")
        Console.WriteLine("3. 0mV �� +10000mV")
        Console.WriteLine("4. 0mV �� +5000mV")
        Console.WriteLine("Please Select Input Range[0-4]:")
        InputRange = Convert.ToInt32(Console.ReadLine())
        If (InputRange < 0 Or InputRange > 4) Then

            GoTo Repeat ' �ж��û�ѡ��������Ƿ�Ϸ������Ϸ���������ѡ��

        End If
        Return InputRange

    End Function

End Module

  