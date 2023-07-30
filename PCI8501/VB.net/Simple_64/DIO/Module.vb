Module Module1

    Declare Function _getch Lib "msvcrt" () As Int32
    Declare Function _kbhit Lib "msvcrt" () As Int32

    Sub Main()
        Dim hDevice As Int32  ' 设备对象句柄
        Dim DeviceLgcID As Int32   ' 物理设备ID号(由板上JP1决定)
        Dim bDISts(0 To 7) As Byte
        Dim bDOSts(0 To 7) As Byte
        Dim Key As Int32
        Dim i As Int32

        DeviceLgcID = 0  ' 设备ID号, 假设系统中只有一个USB2830设备，即DeviceLgcID=0
        hDevice = PCI8501_CreateDevice(DeviceLgcID) ' 创建设备对象
        If (hDevice = (-1)) Then

            Console.WriteLine("Create Device Error")
            _getch()
            Exit Sub

        End If
        bDOSts(0) = 0
        bDOSts(1) = 1
        bDOSts(2) = 0
        bDOSts(3) = 1
        bDOSts(4) = 0
        bDOSts(5) = 1
        bDOSts(6) = 0
        bDOSts(7) = 1

Repeat:

        Console.WriteLine("Press any key to set DO")
        _getch()
        If PCI8501_SetDeviceDO(hDevice, bDOSts) = False Then

            Console.WriteLine("SetDeviceDO Error...")
            _getch()
            Exit Sub

        End If

        For i = 0 To 7
            If (bDOSts(i)) Then

                Console.WriteLine("DO{0} = On", i)
            Else : Console.WriteLine("DO{0} = Off", i)

            End If

        Next i

        Console.WriteLine()
        Console.WriteLine("Press any key to get DI")
        _getch()
        If PCI8501_GetDeviceDI(hDevice, bDISts) = False Then ' 开关量输入

            Console.WriteLine("PCI8501_GetDeviceDI error...")
            _getch()
            Exit Sub

        End If

        For i = 0 To 7
            If (bDISts(i)) Then

                Console.WriteLine("DI{0} = On", i)
            Else : Console.WriteLine("DI{0} = Off", i)

            End If

        Next i

        Console.WriteLine()

        Console.WriteLine("Press any to continue, ESC to quit")
        Key = _getch()
        If (Key <> 27) Then
            GoTo Repeat
        End If

        PCI8501_ReleaseDevice(hDevice)
    End Sub

End Module
