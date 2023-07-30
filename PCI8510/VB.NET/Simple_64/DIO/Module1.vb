Module Module1




    Sub Main()

        Dim hDevice As Integer
	
        Dim DeviceID As Int32    ' 设备号
        Dim bDOSts(0 To 7), bDISts(0 To 7) As Byte
        
        Dim Key As Integer

        DeviceID = 0
        hDevice = PCI8510_CreateDevice(DeviceID)  ' 创建设备对象
        If (hDevice = INVALID_HANDLE_VALUE) Then

            Console.WriteLine("PCI8510_CreateDevice Error...")
            _getch()
            Return
        End If

      

	

        bDOSts(0) = 0       ' 设置开关量输出的状态
        bDOSts(1) = 1
        bDOSts(2) = 0
        bDOSts(3) = 1
        bDOSts(4) = 0
        bDOSts(5) = 1
        bDOSts(6) = 0
        bDOSts(7) = 1

Repeat:
        PCI8510_SetDeviceDO(hDevice, bDOSts)  ' 开关量输出

        ' 反应时间比较长，所以需要一定的延时
        If (bDOSts(0)) Then
            Console.WriteLine("DO0 = On ")
        Else : Console.WriteLine("DO0 = Off ")
        End If

        If (bDOSts(1)) Then
            Console.WriteLine("DO1 = On ")
        Else : Console.WriteLine("DO1 = Off ")
        End If

        If (bDOSts(2)) Then
            Console.WriteLine("DO2 = On ")
        Else : Console.WriteLine("DO2 = Off ")
        End If

        If (bDOSts(3)) Then
            Console.WriteLine("DO3 = On ")
        Else : Console.WriteLine("DO3 = Off ")
        End If

        If (bDOSts(4)) Then
            Console.WriteLine("DO4 = On ")
        Else : Console.WriteLine("DO4 = Off ")
        End If

        If (bDOSts(5)) Then
            Console.WriteLine("DO5 = On ")
        Else : Console.WriteLine("DO5 = Off ")
        End If

        If (bDOSts(6)) Then
            Console.WriteLine("DO6 = On ")
        Else : Console.WriteLine("DO6 = Off ")
        End If

        If (bDOSts(7)) Then
            Console.WriteLine("DO7 = On ")
        Else : Console.WriteLine("DO7 = Off ")
        End If


        Console.WriteLine("Press any key to continue get DI...")
        _getch()
        Console.WriteLine("  ")

        PCI8510_GetDeviceDI(hDevice, bDISts)   ' 开关量输入

        If (bDISts(0)) Then
            Console.WriteLine("DI0 = On ")
        Else : Console.WriteLine("DI0 = Off ")
        End If

        If (bDISts(1)) Then
            Console.WriteLine("DI1 = On ")
        Else : Console.WriteLine("DI1 = Off ")
        End If

        If (bDISts(2)) Then
            Console.WriteLine("DI2 = On ")
        Else : Console.WriteLine("DI2 = Off ")
        End If

        If (bDISts(3)) Then
            Console.WriteLine("DI3 = On ")
        Else : Console.WriteLine("DI3 = Off ")
        End If

        If (bDISts(4)) Then
            Console.WriteLine("DI4 = On ")
        Else : Console.WriteLine("DI4 = Off ")
        End If

        If (bDISts(5)) Then
            Console.WriteLine("DI5 = On ")
        Else : Console.WriteLine("DI5 = Off ")
        End If

        If (bDISts(6)) Then
            Console.WriteLine("DI6 = On ")
        Else : Console.WriteLine("DI6 = Off ")
        End If

        If (bDISts(7)) Then
            Console.WriteLine("DI7 = On ")
        Else : Console.WriteLine("DI7 = Offn")
        End If

        Console.WriteLine("Press ESC to quit ")
        Key = _getch()
        If (Key <> 27) Then
            GoTo Repeat
        End If
        PCI8510_ReleaseDevice(hDevice)

        Return
    End Sub
End Module