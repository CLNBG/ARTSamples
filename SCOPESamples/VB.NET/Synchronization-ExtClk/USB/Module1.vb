'This example program demonstrates multi cards synchronization in USB bus using common external clock.

Imports System.Threading

Module Module1
    Public Structure threadParas
        Dim hDevice As IntPtr
        Dim samplelength As UInteger
        Dim numWave As UInteger
        Dim timeout As Double
        Dim flag As Int32
    End Structure
    Dim taskHandle_master As IntPtr
    Dim taskHandle_slave As IntPtr

    Dim resourceName_master As String
    Dim resourceName_slave As String
    Dim channelName As String

    ' set the acquisition mode
    Dim acquisitionMode As Int32

    ' set vertical parameters
    Dim verticalRange As Int32
    Dim verticalOffset As Int32
    Dim verticalCoupling As Int32

    ' set horizontal parameters
    Dim minSampleRate As Double
    Dim refPosition As Double
    Dim minRecordLength As Int32

    ' set trigger parameters
    Dim triggerLevel As Int32
    Dim triggerSlope As Int32
    Dim triggerCoupling As Int32
    Dim triggerCount As Int32
    Dim triggerSensitivity As Int32
    Dim triggerSource_master As String
    Dim triggerSource_slave As String
    Dim triggerSynPulseSource As String

    ' temporary variables
    Dim actualRecordLength_master As Int32
    Dim actualRecordLength_slave As Int32

    Dim numWfms_master As UInteger
    Dim numWfms_slave As UInteger
    Dim timeout As Double

    Dim lsb(7) As Double
    Dim rangeval(7) As Double
    Dim k As Int32
    Dim fVolt As Double
    Dim strError(2047) As Byte
    Dim str As String
    Dim bError As Boolean

    Dim parasMaster As threadParas
    Dim parasSlave As threadParas
    Dim readThreadMaster As Thread
    Dim readThreadSlave As Thread

    Sub Main()
        taskHandle_master = IntPtr.Zero
        taskHandle_slave = IntPtr.Zero
        acquisitionMode = ARTSCOPE_SAMPMODE_FINITE
        verticalRange = ARTSCOPE_RANGE_10Vpp
        verticalCoupling = ARTSCOPE_VAL_DC
        numWfms_master = 0
        numWfms_slave = 0

        timeout = 5.0

        channelName = "0"
        minSampleRate = 10000000
        minRecordLength = 1024
        refPosition = 0.0
        triggerSlope = ARTSCOPE_VAL_TRIGDIR_NEGATIVE
        triggerCount = 1
        triggerSensitivity = 0

        triggerSource_master = ARTSCOPE_VAL_TRIGSRC_CH0
        triggerSource_slave = ARTSCOPE_VAL_TRIGSRC_SYNC
        triggerSynPulseSource = ARTSCOPE_VAL_TRIG_SYNC_0
        actualRecordLength_master = 0
        actualRecordLength_slave = 0
        bError = False

        Console.Write("Please enter the master device name:")
        resourceName_master = Console.ReadLine()
        Console.Write("Please enter the slave device name:")
        resourceName_slave = Console.ReadLine()

        'initialize master card
        If (ArtScope_init(resourceName_master, taskHandle_master) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureAcquisitionMode(taskHandle_master, acquisitionMode) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureVertical(taskHandle_master, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureHorizontalTiming(taskHandle_master, minSampleRate, minRecordLength, refPosition) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureTriggerEdge(taskHandle_master, triggerSource_master, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureClock(taskHandle_master, ARTSCOPE_VAL_CLK_IN, 1, 1) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualNumWfms(taskHandle_master, numWfms_master) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_InitiateAcquisition(taskHandle_master) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualRecordLength(taskHandle_master, actualRecordLength_master) < 0) Then
            bError = True
            GoTo Error_close
        End If

        'initialize slave card
        If (ArtScope_init(resourceName_slave, taskHandle_slave) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureAcquisitionMode(taskHandle_slave, acquisitionMode) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureVertical(taskHandle_slave, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureHorizontalTiming(taskHandle_slave, minSampleRate, minRecordLength, refPosition) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureTriggerEdge(taskHandle_slave, triggerSource_slave, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ConfigureClock(taskHandle_slave, ARTSCOPE_VAL_CLK_IN, 1, 0) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualNumWfms(taskHandle_slave, numWfms_slave) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_InitiateAcquisition(taskHandle_slave) < 0) Then
            bError = True
            GoTo Error_close
        End If

        If (ArtScope_ActualRecordLength(taskHandle_slave, actualRecordLength_slave) < 0) Then
            bError = True
            GoTo Error_close
        End If

        'start slave card
        If (ArtScope_StartAcquisition(taskHandle_slave) < 0) Then
            bError = True
            GoTo Error_label
        End If
        'create thread to read data of slave card
        parasSlave = New threadParas
        parasSlave.hDevice = taskHandle_slave
        parasSlave.samplelength = actualRecordLength_slave
        parasSlave.numWave = numWfms_slave
        parasSlave.timeout = timeout
        parasSlave.flag = 2
        readThreadSlave = New Thread(AddressOf ReadDataThread)
        readThreadSlave.Start(parasSlave)

        'start master card
        If (ArtScope_StartAcquisition(taskHandle_master) < 0) Then
            bError = True
            GoTo Error_label
        End If
        'create thread to read data of master card
        parasMaster = New threadParas
        parasMaster.hDevice = taskHandle_master
        parasMaster.samplelength = actualRecordLength_master
        parasMaster.numWave = numWfms_master
        parasMaster.timeout = timeout
        parasMaster.flag = 1
        readThreadMaster = New Thread(AddressOf ReadDataThread)
        readThreadMaster.Start(parasMaster)

        Console.WriteLine("Press any key to quit...")
        _getch()
        Exit Sub

Error_label:
        If (taskHandle_master <> IntPtr.Zero) Then
            ArtScope_StopAcquisition(taskHandle_master)
            ArtScope_ReleaseAcquisition(taskHandle_master)
        End If
        If (taskHandle_slave <> IntPtr.Zero) Then
            ArtScope_StopAcquisition(taskHandle_slave)
            ArtScope_ReleaseAcquisition(taskHandle_slave)
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
        If (taskHandle_master <> IntPtr.Zero) Then
            ArtScope_Close(taskHandle_master)
        End If
        If (taskHandle_slave <> IntPtr.Zero) Then
            ArtScope_Close(taskHandle_slave)
        End If
        Console.WriteLine("Press any key to quit...")
        _getch()
    End Sub

    Private Sub ReadDataThread(ByVal paras As Object)
        Dim samplepara As threadParas = CType(paras, threadParas)
        Dim waveformPtr() As UInt16
        Dim wfmInfo As ArtScope_wfmInfo = New ArtScope_wfmInfo()
        wfmInfo.fLsbs = lsb
        wfmInfo.rangevalue = rangeval
        ReDim waveformPtr(samplepara.samplelength * samplepara.numWave)

        While True
            If (ArtScope_FetchBinary16(samplepara.hDevice, samplepara.timeout, samplepara.samplelength, waveformPtr, wfmInfo) < 0) Then
                Console.WriteLine("{0} Fetch Binary16 error!", samplepara.flag)
                Exit While
            End If
            For k = 0 To samplepara.numWave - 1
                fVolt = wfmInfo.fLsbs(k Mod samplepara.numWave) * (waveformPtr(k) And wfmInfo.wMaxLSB) - wfmInfo.rangevalue(k Mod samplepara.numWave)
                Console.Write("{0,8:F2}(mV) ", fVolt)
            Next
            Console.WriteLine("")
            Exit While
        End While

        ArtScope_StopAcquisition(samplepara.hDevice)
        ArtScope_ReleaseAcquisition(samplepara.hDevice)
        ArtScope_Close(samplepara.hDevice)

    End Sub

End Module
