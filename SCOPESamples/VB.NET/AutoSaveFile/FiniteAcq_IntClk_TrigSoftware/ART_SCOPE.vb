Imports System.Runtime.InteropServices

Module ART_SCOPE

    Public Const ARTSCOPE_SAMPMODE_FINITE = 0
    Public Const ARTSCOPE_SAMPMODE_COUPLING_CONTINUOUS = 1

    ' ARTSCOPE coupling
    Public Const ARTSCOPE_VAL_DC = 0
    Public Const ARTSCOPE_VAL_AC = 1

    ' ARTSCOPE inputImpedance
    Public Const ARTSCOPE_IMPED_1M = 0      ' 1M¦¸
    Public Const ARTSCOPE_IMPED_50 = 1        ' 50¦¸

    'ARTSCOPE bandwidth
    Public Const ARTSCOPE_BANDWIDTH_ALL = -1
    Public Const ARTSCOPE_BANDWIDTH_DEFAULT = 0
    Public Const ARTSCOPE_BANDWIDTH_100M = 1
    Public Const ARTSCOPE_BANDWIDTH_20M = 2

    ' ARTSCOPE range
    Public Const ARTSCOPE_RANGE_20Vpp = 0
    Public Const ARTSCOPE_RANGE_10Vpp = 1
    Public Const ARTSCOPE_RANGE_2Vpp = 2
    Public Const ARTSCOPE_RANGE_40Vpp = 3
    Public Const ARTSCOPE_RANGE_16Vpp = 4
    Public Const ARTSCOPE_RANGE_8Vpp = 5
    Public Const ARTSCOPE_RANGE_4Vpp = 6
    Public Const ARTSCOPE_RANGE_1600mVpp = 7
    Public Const ARTSCOPE_RANGE_800mVpp = 8
    Public Const ARTSCOPE_RANGE_400mVpp = 9
    Public Const ARTSCOPE_RANGE_160mVpp = 10
    Public Const ARTSCOPE_RANGE_80mVpp = 11
    Public Const ARTSCOPE_RANGE_40mVpp = 12

    ' ArtScope_ConfigureTriggerEdge triggerSource
    Public Const ARTSCOPE_VAL_TRIGSRC_DTR = "VAL_TRIGGER_DTR"
    Public Const ARTSCOPE_VAL_TRIGSRC_ATR = "VAL_TRIGGER_ATR"
    Public Const ARTSCOPE_VAL_TRIGSRC_SYNC = "VAL_TRIGGER_SYNCTRIGGER"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH0 = "VAL_TRIGGER_CH0"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH1 = "VAL_TRIGGER_CH1"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH2 = "VAL_TRIGGER_CH2"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH3 = "VAL_TRIGGER_CH3"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH4 = "VAL_TRIGGER_CH4"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH5 = "VAL_TRIGGER_CH5"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH6 = "VAL_TRIGGER_CH6"
    Public Const ARTSCOPE_VAL_TRIGSRC_CH7 = "VAL_TRIGGER_CH7"
    Public Const ARTSCOPE_VAL_TRIGSRC_PFI = "VAL_TRIGGER_PFI"

    ' ARTSCOPE triggerSyncPulseSource
    Public Const ARTSCOPE_VAL_TRIG_SYNC_0 = "VAL_SYNC_TRIGGER0"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_1 = "VAL_SYNC_TRIGGER1"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_2 = "VAL_SYNC_TRIGGER2"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_3 = "VAL_SYNC_TRIGGER3"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_4 = "VAL_SYNC_TRIGGER4"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_5 = "VAL_SYNC_TRIGGER5"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_6 = "VAL_SYNC_TRIGGER6"
    Public Const ARTSCOPE_VAL_TRIG_SYNC_7 = "VAL_SYNC_TRIGGER7"

    ' ARTSCOPE triggerSlope
    Public Const ARTSCOPE_VAL_TRIGDIR_NEGATIVE = 0
    Public Const ARTSCOPE_VAL_TRIGDIR_POSITIVE = 1
    Public Const ARTSCOPE_VAL_TRIGDIR_NEGATPOSIT = 2

    ' ARTSCOPE trigger polarity
    Public Const ARTSCOPE_VAL_TRIGPOLAR_POSITIVE = 0
    Public Const ARTSCOPE_VAL_TRIGPOLAR_NEGATIVE = 1

    ' ARTSCOPE trigger coupling
    Public Const ARTSCOPE_VAL_TRIGCOUP_DC = 0
    Public Const ARTSCOPE_VAL_TRIGCOUP_AC = 1
    Public Const ARTSCOPE_VAL_TRIGCOUP_HIG = 2
    Public Const ARTSCOPE_VAL_TRIGCOUP_LOW = 3

    ' ARTSCOPE trigger window type
    Public Const ARTSCOPE_VAL_WINDOW_ENTERING = 0
    Public Const ARTSCOPE_VAL_WINDOW_LEAVING = 1

    ' ARTSCOPE trigger pulse direction
    Public Const ARTSCOPE_VAL_TRIGDIR_PULSE_GREATER = 0
    Public Const ARTSCOPE_VAL_TRIGDIR_PULSE_LESS = 1
    Public Const ARTSCOPE_VAL_TRIGDIR_PULSE_EQUAL = 2
    Public Const ARTSCOPE_VAL_TRIGDIR_PULSE_NEQUAL = 3

    ' ARTSCOPE trigOutPolarity
    Public Const ARTSCOPE_VAL_TRIGOUT_POLAR_NEGATIVE = 0
    Public Const ARTSCOPE_VAL_TRIGOUT_POLAR_POSITIVE = 1

    ' ARTSCOPE inputClockSource
    Public Const ARTSCOPE_VAL_CLK_IN = "VAL_CLK_IN"
    Public Const ARTSCOPE_VAL_CLK_ONBOARD = "VAL_CLK_INTERNAL"
    Public Const ARTSCOPE_VAL_CLK10M_EXT = "VAL_CLK10M_EXT"
    Public Const ARTSCOPE_VAL_CLK10M_PXI = "VAL_CLK10M_PXI"
    Public Const ARTSCOPE_VAL_CLK10M_MASTER = "VAL_CLK10M_MASTER"
    Public Const ARTSCOPE_VAL_CLK100M_PXIE = "VAL_CLK100M_PXIE"

    ' ARTSCOPE ExportClock Values
    Public Const ARTSCOPE_VAL_CLK_REF = 0
    Public Const ARTSCOPE_VAL_CLK_SAMPLE = 1

    Structure ARTSCOPE_STATUS_AD
        Dim bADEanble As Int32
        Dim bTrigger As Int32
        Dim bComplete As Int32
        Dim bAheadTrig As Int32
        Dim lCanReadPoint As ULong
        Dim lSavePoints As ULong
    End Structure

    <StructLayout(LayoutKind.Sequential)> _
    Structure ArtScope_wfmInfo
        <MarshalAs(UnmanagedType.ByValArray, sizeconst:=8)> _
        Dim fLsbs As Double()
        <MarshalAs(UnmanagedType.ByValArray, sizeconst:=8)> _
        Dim rangevalue As Double()
        Dim wMaxLSB As UInteger
        Dim TriggerMode As Int32
        Dim actualSamples As UInteger
        Dim pAvailSampsPoints As UInteger
        Dim channelCount As Int32
    End Structure

    <StructLayout(LayoutKind.Sequential)> _
    Structure DEVICE_EEP_INFO
        Dim nDLLVer As UInteger
        Dim nSysVer As UInteger
        Dim nFirmwareVer As UInteger
        Dim nCaled As UInteger
        Dim nCalDate As UInt64
        <MarshalAs(UnmanagedType.ByValArray, sizeconst:=40)> _
        Dim nReserved As Byte()
    End Structure

    ' Create Task
    Declare Function ArtScope_init Lib "ART_SCOPE.dll" (ByVal resourceName As String, ByRef taskHandle As IntPtr) As Int32

    ' Set Acquisition Mode
    Declare Function ArtScope_ConfigureAcquisitionMode Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal acquisitionMode As Int32) As Int32

    ' Set Vertical Parameters
    Declare Function ArtScope_ConfigureVertical Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal channelList As String, ByVal range As Int32, ByVal offset As Int32, ByVal coupling As Int32, ByVal probeAttenuation As Int32, ByVal enabled As UInt16) As Int32

    ' Set Channel Parameter
    Declare Function ArtScope_ConfigureChanCharacteristics Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal channelList As String, ByVal inputImpedance As Int32, ByVal maxInputFrequency As Int32) As Int32

    ' Set Horizontal Parameters
    Declare Function ArtScope_ConfigureHorizontalTiming Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal minSampleRate As Double, ByVal minNumPts As Int32, ByVal refPosition As Double) As Int32

    ' Set Edge Trigger Parameters
    Declare Function ArtScope_ConfigureTriggerEdge Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal triggerSource As String, ByVal level As Double, ByVal slope As Int32, ByVal triggerCoupling As Int32, ByVal triggerCount As Int32, ByVal triggerSensitivity As Int32) As Int32

    ' Set Window Trigger Parameters
    Declare Function ArtScope_ConfigureTriggerWindow Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal triggerSource As String, ByVal lowLevel As Double, ByVal highLevel As Double, ByVal windowMode As Int32, ByVal triggerCoupling As Int32, ByVal triggerCount As Int32, ByVal triggerSensitivity As Int32) As Int32

    ' Set Pulse Trigger Parameters
    Declare Function ArtScope_ConfigureTriggerPulse Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal triggerSource As String, ByVal level As Double, ByVal polarity As Int32, ByVal slope As Int32, ByVal width As Int32, ByVal triggerCoupling As Int32, ByVal triggerCount As Int32, ByVal triggerSensitivity As Int32) As Int32

    ' Set Digital Trigger Parameters
    Declare Function ArtScope_ConfigureTriggerDigital Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal triggerSource As String, ByVal slope As Int32, ByVal triggerCount As Int32, ByVal triggerSensitivity As Int32) As Int32

    ' Set Hardware Trigger Parameters
    Declare Function ArtScope_ConfigureTriggerHardwareDelay Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal delayPoints As Int32) As Int32

    ' Set Software Trigger
    Declare Function ArtScope_ConfigureTriggerSoftWare Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Set Synchronous Trigger Source
    Declare Function ArtScope_ConfigureTriggerSyncSrc Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal triggerSyncSource As String) As Int32

    ' Set Acquisition Clock Parameters
    Declare Function ArtScope_ConfigureClock Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal inputClockSource As String, ByVal freqDivision As Int32, ByVal masterEnabled As UInteger) As Int32

    ' Export Signal
    Declare Function ArtScope_ExportClock Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal signal As Int32, ByVal signalIdentifier As String, ByVal outputTerminal As String) As Int32

    ' Export Trigger
    Declare Function ArtScope_ExportTrigger Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal trigOutWidth As Int32, ByVal trigOutPolarity As Int32) As Int32

    ' Auto Save File
    Declare Function ArtScope_AutoSaveFile Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal bIsAutoSave As UInteger, ByVal chFileName As String) As Int32

    ' Initialize Acquisition
    Declare Function ArtScope_InitiateAcquisition Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Start Acquisition
    Declare Function ArtScope_StartAcquisition Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Send Software Trigger
    Declare Function ArtScope_SendSoftwareTrigger Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Get Acquisition Status
    Declare Function ArtScope_AcquisitionStatus Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByRef status As ARTSCOPE_STATUS_AD) As Int32

    ' Read Binary Data(BYTE)
    Declare Function ArtScope_FetchBinary8 Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal timeout As Double, ByVal numSamples As UInteger, ByVal wfm() As Byte, ByRef wfmInfo As ArtScope_wfmInfo) As Int32

    ' Read Binary Data(unsigned short)
    Declare Function ArtScope_FetchBinary16 Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal timeout As Double, ByVal numSamples As UInteger, ByVal wfm() As UInt16, ByRef wfmInfo As ArtScope_wfmInfo) As Int32

    ' Read Voltage Data
    Declare Function ArtScope_FetchVoltage Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByVal timeout As Double, ByVal numSamples As UInteger, ByVal wfm() As Double, ByRef wfmInfo As ArtScope_wfmInfo) As Int32

    ' Stop Acquisition
    Declare Function ArtScope_StopAcquisition Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Release Acquisition
    Declare Function ArtScope_ReleaseAcquisition Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Close Task
    Declare Function ArtScope_Close Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    ' Get Acquisition Channel Count
    Declare Function ArtScope_ActualNumWfms Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByRef numWfms As UInteger) As Int32

    ' Get Actual Sample Length
    Declare Function ArtScope_ActualRecordLength Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByRef actualRecordLength As UInteger) As Int32

    ' Get Actual Sample Rate
    Declare Function ArtScope_SampleRate Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByRef actualSampleRate As Double) As Int32

    ' Calibration
    Declare Function ArtScope_Calibration Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr) As Int32

    'Reset Device
    Declare Function ArtScope_Reset Lib "ART_SCOPE.dll" (ByVal resourceName As String) As Int32

    ' Get information from auto saved file,include the file header size,information used for conversion
    Declare Function ArtScope_GetInfoFromAutoSaveFile Lib "ART_SCOPE.dll" (ByVal chFileName As String, ByRef fileHeaderSize As Int32, ByRef wfmInfo As ArtScope_wfmInfo) As Int32

    ' Get error information
    Declare Function ArtScope_GetErrorString Lib "ART_SCOPE.dll" (ByVal errorCode As Int32, ByVal errorString() As Byte) As Int32

    ' Get error information
    Declare Function ArtScope_GetExtendedErrorInfo Lib "ART_SCOPE.dll" (ByVal errorString() As Byte, ByVal bufferSize As UInteger) As Int32

    ' Get version information
    Declare Function ArtScope_GetDeviceEEPInfo Lib "ART_SCOPE.dll" (ByVal resourceName As String, ByRef devInfo As DEVICE_EEP_INFO) As UInteger

    ' Get channel count and max sample rate the card supported
    Declare Function ArtScope_GetDevInformation Lib "ART_SCOPE.dll" (ByVal resourceName As String, ByRef channelCount As Int32, ByRef maxSampleRate As Single) As Int32

    ' Get series number the card belongs to
    Declare Function ArtScope_GetExtenedType Lib "ART_SCOPE.dll" (ByVal taskHandle As IntPtr, ByRef seriesKey As UInteger) As Int32

    ' Open binary data file
    Declare Function ArtScope_OpenDataFile Lib "ART_SCOPE.dll" (ByVal fileName As String, ByRef hFile As IntPtr, ByRef dataCount As UInt64, ByRef chanCount As Int32) As Int32

    ' Convert binary data to voltage data,unit V
    Declare Function ArtScope_DataConvertFloat32 Lib "ART_SCOPE.dll" (ByVal hFile As IntPtr, ByVal dataOffset As UInt64, ByVal convertCount As UInt32, ByRef dataBuf As Single, ByRef retConvertCount As UInt32, ByVal dataCross As Int32) As Int32

    ' Convert binary data to voltage data,unit V
    Declare Function ArtScope_DataConvertFloat64 Lib "ART_SCOPE.dll" (ByVal hFile As IntPtr, ByVal dataOffset As UInt64, ByVal convertCount As UInt32, ByRef dataBuf As Double, ByRef retConvertCount As UInt32, ByVal dataCross As Int32) As Int32

    ' Close binary data file
    Declare Function ArtScope_CloseDataFile Lib "ART_SCOPE.dll" (byval hFile as IntPtr) As Int32


End Module
