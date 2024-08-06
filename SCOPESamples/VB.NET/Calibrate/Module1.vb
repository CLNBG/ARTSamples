'calibrate

Module Module1
    Dim taskHandle As IntPtr

    Dim resourceName As String
    Dim channelName As String

    Dim k As Int32
    Dim fVolt As Double
    Dim strError(2047) As Byte
    Dim str As String
    Dim bError As Boolean

    Sub Main()
        taskHandle = IntPtr.Zero
        bError = False

        Console.Write("Please enter the device name:")
        resourceName = Console.ReadLine()

        If (ArtScope_init(resourceName, taskHandle) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_Calibration(taskHandle) < 0) Then
            bError = True
            GoTo Error_close
        End If

Error_close:
        ' get error information
            If (bError = True) Then
                ArtScope_GetExtendedErrorInfo(strError, 2048)
                For k = 0 To 2047
                    If (strError(k) = 0) Then
                        Exit For
                    End If
                Next
                str = System.Text.Encoding.Default.GetString(strError, 0, k)
                Console.WriteLine(str)
            End If
            ArtScope_Close(taskHandle)

            Console.WriteLine("Press any key to quit...")
            _getch()
    End Sub

End Module
