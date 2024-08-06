'voltage continuous sample

Module Module1
    Dim taskHandle As IntPtr

    Dim resourceName As String
    Dim channelName As String

    ' set the acquisition mode
    Dim acquisitionMode As Int32

    ' set vertical parameters
    Dim verticalRange As Int32
    Dim verticalOffset As Int32
    Dim verticalCoupling As Int32

    ' set horizontal parameters
    Dim minRecordLength As Int32
    Dim minSampleRate As Double
    Dim refPosition As Double

    ' temporary variables
    Dim actualRecordLength As Int32
    Dim numWfms As UInteger
    Dim timeout As Double
    Dim wfmInfo As ArtScope_wfmInfo
    Dim lsb(7) As Double
    Dim rangeval(7) As Double
    Dim AD_Status As ARTSCOPE_STATUS_AD

    Dim waveformPtr() As Double
    Dim k As Int32
    Dim fVolt As Double
    Dim strError(2047) As Byte
    Dim str As String
    Dim bError As Boolean

    Sub Main()
        taskHandle = IntPtr.Zero
        acquisitionMode = ARTSCOPE_SAMPMODE_COUPLING_CONTINUOUS
        verticalRange = ARTSCOPE_RANGE_10Vpp
        verticalOffset = 0
        verticalCoupling = ARTSCOPE_VAL_DC
        numWfms = 0

        timeout = 1.0

        channelName = "0"
        minSampleRate = 10000000
        minRecordLength = 1024
        refPosition = 0.0

        actualRecordLength = 0

        wfmInfo.fLsbs = lsb
        wfmInfo.rangevalue = rangeval
        bError = False

        Console.Write("Please enter the device name:")
        resourceName = Console.ReadLine()

        If (ArtScope_init(resourceName, taskHandle) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minRecordLength, refPosition) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureTriggerSoftWare(taskHandle) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualNumWfms(taskHandle, numWfms) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_InitiateAcquisition(taskHandle) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualRecordLength(taskHandle, actualRecordLength) < 0) Then
            bError = True
            GoTo Error_close
        End If

        ReDim waveformPtr(actualRecordLength * numWfms)

        If (ArtScope_StartAcquisition(taskHandle) < 0) Then
            bError = True
            GoTo Error_label
        End If

        While (_kbhit() = 0)
            If (ArtScope_FetchVoltage(taskHandle, timeout, actualRecordLength, waveformPtr, wfmInfo) < 0) Then
                bError = True
                GoTo Error_label
            End If
            For k = 0 To numWfms - 1
                Console.Write("{0,8:F4}(mV)  ", waveformPtr(k))
            Next
            Console.WriteLine("")
        End While

        _getch()

Error_label:
        If (taskHandle <> IntPtr.Zero) Then
            ArtScope_StopAcquisition(taskHandle)
            ArtScope_ReleaseAcquisition(taskHandle)
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
            Str = System.Text.Encoding.Default.GetString(strError, 0, k)
            Console.WriteLine(Str)
        End If
        ArtScope_Close(taskHandle)

        Console.WriteLine("Press any key to quit...")
        _getch()
    End Sub

End Module
