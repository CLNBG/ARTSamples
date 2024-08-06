' auto save file sample
Imports System.IO

Module Module1
    Dim taskHandle As IntPtr

    Dim resourceName As String
    Dim channelName As String
    Dim fileName As String

    Dim bSaveFile As Int32

    ' Set the acquisition mode
    Dim acquisitionMode As Int32

    ' Set vertical parameters
    Dim verticalRange As Int32
    Dim verticalOffset As Int32
    Dim verticalCoupling As Int32

    ' Set horizontal parameters
    Dim minSampleRate As Double
    Dim minRecordLength As Int32
    Dim refPosition As Double

    ' Set trigger parameters
    Dim triggerSource As String
    Dim triggerLevel As Int32
    Dim triggerSlope As Int32
    Dim triggerCoupling As Int32
    Dim triggerCount As Int32
    Dim triggerSensitivity As Int32

    ' Temporary variables
    Dim actualRecordLength As Int32
    Dim numWfms As UInteger
    Dim timeout As Double

    Dim wfmInfo As ArtScope_wfmInfo
    Dim lsb(7) As Double
    Dim rangeval(7) As Double
    Dim AD_Status As ARTSCOPE_STATUS_AD

    Dim waveformPtr() As UInt16
    Dim k As Int32
    Dim fVolt As Double

    Dim ret As Int32
    Dim fileHeaderSize As Int32
    Dim fileLen As Int32
    Dim filewfmInfo As ArtScope_wfmInfo
    Dim fs As FileStream
    Dim br As BinaryReader
    Dim header() As Byte
    Dim strError(2047) As Byte
    Dim fileDataBuf(1023) As UInt16
    Dim voltData(1023) As Double
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
        triggerSlope = ARTSCOPE_VAL_TRIGDIR_NEGATIVE
        triggerCoupling = ARTSCOPE_VAL_TRIGCOUP_DC
        triggerCount = 1
        triggerSensitivity = 0
        triggerSource = ARTSCOPE_VAL_TRIGSRC_CH0
        fileName = "C:\\1.pci"
        bSaveFile = 1
        actualRecordLength = 0

        wfmInfo.fLsbs = lsb
        wfmInfo.rangevalue = rangeval
        bError = False

        Console.Write("Please enter the device name:")
        resourceName = Console.ReadLine()

        Console.Write("Please enter the file path and file name(like:D:\\1.pci):")
        fileName = Console.ReadLine()

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

        If (ArtScope_ConfigureTriggerEdge(taskHandle, triggerSource, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_AutoSaveFile(taskHandle, bSaveFile, fileName) < 0) Then
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
        If (bSaveFile = 1) Then
            While (_kbhit() = 0)
                If (ArtScope_FetchBinary16(taskHandle, timeout, actualRecordLength, waveformPtr, wfmInfo) < 0) Then
                    bError = True
                    GoTo Error_label
                End If
                For k = 0 To numWfms - 1
                    Console.Write("{0,8:d}  ", waveformPtr(k))
                Next
                Console.WriteLine("")
            End While
        End If

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
            str = System.Text.Encoding.Default.GetString(strError, 0, k)
            Console.WriteLine(str)
            ArtScope_Close(taskHandle)
            _getch()
            End
        End If

        ArtScope_Close(taskHandle)

        ' Analyze the auto-saved file
        ' Use this function to get the size of file header,and some information used to convert binary data to voltage data
        ret = ArtScope_GetInfoFromAutoSaveFile(fileName, fileHeaderSize, filewfmInfo)

        ' Get the data count in the file
        fileLen = (New FileInfo(fileName).Length - fileHeaderSize) / 2
        fs = New FileStream(fileName, FileMode.Open)
        br = New BinaryReader(fs)
        ReDim header(fileHeaderSize)
        br.Read(header, 0, fileHeaderSize)

        While (fileLen > 0)
            If (fileLen >= 1024) Then
                For k = 0 To 1023
                    fileDataBuf(k) = br.ReadUInt16()
                    voltData(k) = (filewfmInfo.fLsbs(k Mod filewfmInfo.channelCount) * (fileDataBuf(k) And filewfmInfo.wMaxLSB) - filewfmInfo.rangevalue(k Mod filewfmInfo.channelCount)) / 1000.0
                    If (k > 0 And (k Mod filewfmInfo.channelCount = 0)) Then
                        Console.WriteLine()
                    End If
                    Console.Write("{0,8:F4}(mV)  ", voltData(k))
                Next
                Console.WriteLine()
            Else
                For k = 0 To fileLen - 1
                    fileDataBuf(k) = br.ReadUInt16()
                    voltData(k) = (filewfmInfo.fLsbs(k Mod filewfmInfo.channelCount) * (fileDataBuf(k) And filewfmInfo.wMaxLSB) - filewfmInfo.rangevalue(k Mod filewfmInfo.channelCount)) / 1000.0
                    If (k > 0 And (k Mod filewfmInfo.channelCount = 0)) Then
                        Console.WriteLine()
                    End If
                    Console.Write("{0,8:F4}(mV)  ", voltData(k))
                Next
                Console.WriteLine()
            End If
            Exit While
            'fileLen = fileLen - 1024
        End While

        Console.WriteLine("Press any key to quit...")
        _getch()
    End Sub

End Module
