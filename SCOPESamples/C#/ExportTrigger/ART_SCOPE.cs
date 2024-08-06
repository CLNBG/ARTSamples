using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    public class ART_SCOPE
    {
        public const Int32 ARTSCOPE_SAMPMODE_FINITE = 0x00;                         // Finite acquisition mode
        public const Int32 ARTSCOPE_SAMPMODE_COUPLING_CONTINUOUS = 0x01;            // Continuous acquisition mode

        // ARTSCOPE coupling
        public const Int32 ARTSCOPE_VAL_DC = 0;
        public const Int32 ARTSCOPE_VAL_AC = 1;

        // ARTSCOPE inputImpedance
        public const Int32 ARTSCOPE_IMPED_1M = 0;
        public const Int32 ARTSCOPE_IMPED_50 = 1;

        // ARTSCOPE bandwidth
        public const Int32 ARTSCOPE_BANDWIDTH_ALL = -1;
        public const Int32 ARTSCOPE_BANDWIDTH_DEFAULT = 0;
        public const Int32 ARTSCOPE_BANDWIDTH_100M = 1;
        public const Int32 ARTSCOPE_BANDWIDTH_20M = 2;

        // ARTSCOPE range
        public const Int32 ARTSCOPE_RANGE_20Vpp = 0;
        public const Int32 ARTSCOPE_RANGE_10Vpp = 1;
        public const Int32 ARTSCOPE_RANGE_2Vpp = 2;
        public const Int32 ARTSCOPE_RANGE_40Vpp = 3;
        public const Int32 ARTSCOPE_RANGE_16Vpp = 4;
        public const Int32 ARTSCOPE_RANGE_8Vpp = 5;
        public const Int32 ARTSCOPE_RANGE_4Vpp = 6;
        public const Int32 ARTSCOPE_RANGE_1600mVpp = 7;
        public const Int32 ARTSCOPE_RANGE_800mVpp = 8;
        public const Int32 ARTSCOPE_RANGE_400mVpp = 9;
        public const Int32 ARTSCOPE_RANGE_160mVpp = 10;
        public const Int32 ARTSCOPE_RANGE_80mVpp = 11;
        public const Int32 ARTSCOPE_RANGE_40mVpp = 12;

        // ARTSCOPE triggerSource
        public const string ARTSCOPE_VAL_TRIGSRC_DTR = "VAL_TRIGGER_DTR";
        public const string ARTSCOPE_VAL_TRIGSRC_ATR = "VAL_TRIGGER_ATR";
        public const string ARTSCOPE_VAL_TRIGSRC_SYNC = "VAL_TRIGGER_SYNCTRIGGER";
        public const string ARTSCOPE_VAL_TRIGSRC_CH0 = "VAL_TRIGGER_CH0";
        public const string ARTSCOPE_VAL_TRIGSRC_CH1 = "VAL_TRIGGER_CH1";
        public const string ARTSCOPE_VAL_TRIGSRC_CH2 = "VAL_TRIGGER_CH2";
        public const string ARTSCOPE_VAL_TRIGSRC_CH3 = "VAL_TRIGGER_CH3";
        public const string ARTSCOPE_VAL_TRIGSRC_CH4 = "VAL_TRIGGER_CH4";
        public const string ARTSCOPE_VAL_TRIGSRC_CH5 = "VAL_TRIGGER_CH5";
        public const string ARTSCOPE_VAL_TRIGSRC_CH6 = "VAL_TRIGGER_CH6";
        public const string ARTSCOPE_VAL_TRIGSRC_CH7 = "VAL_TRIGGER_CH7";
        public const string ARTSCOPE_VAL_TRIGSRC_PFI = "VAL_TRIGGER_PFI";

        // ARTSCOPE triggerSyncPulseSource
        public const string ARTSCOPE_VAL_TRIG_SYNC_0 = "VAL_SYNC_TRIGGER0";
        public const string ARTSCOPE_VAL_TRIG_SYNC_1 = "VAL_SYNC_TRIGGER1";
        public const string ARTSCOPE_VAL_TRIG_SYNC_2 = "VAL_SYNC_TRIGGER2";
        public const string ARTSCOPE_VAL_TRIG_SYNC_3 = "VAL_SYNC_TRIGGER3";
        public const string ARTSCOPE_VAL_TRIG_SYNC_4 = "VAL_SYNC_TRIGGER4";
        public const string ARTSCOPE_VAL_TRIG_SYNC_5 = "VAL_SYNC_TRIGGER5";
        public const string ARTSCOPE_VAL_TRIG_SYNC_6 = "VAL_SYNC_TRIGGER6";
        public const string ARTSCOPE_VAL_TRIG_SYNC_7 = "VAL_SYNC_TRIGGER7";

        // ARTSCOPE triggerSlope
        public const Int32 ARTSCOPE_VAL_TRIGDIR_NEGATIVE = 0;
        public const Int32 ARTSCOPE_VAL_TRIGDIR_POSITIVE = 1;
        public const Int32 ARTSCOPE_VAL_TRIGDIR_NEGATPOSIT = 2;

        // ARTSCOPE trigger polarity
        public const Int32 ARTSCOPE_VAL_TRIGPOLAR_POSITIVE = 0;
        public const Int32 ARTSCOPE_VAL_TRIGPOLAR_NEGATIVE = 1;

        // ARTSCOPE trigger coupling
        public const Int32 ARTSCOPE_VAL_TRIGCOUP_DC = 0;
        public const Int32 ARTSCOPE_VAL_TRIGCOUP_AC = 1;
        public const Int32 ARTSCOPE_VAL_TRIGCOUP_HIG = 2;
        public const Int32 ARTSCOPE_VAL_TRIGCOUP_LOW = 3;

        // ARTSCOPE trigger window type
        public const Int32 ARTSCOPE_VAL_WINDOW_ENTERING = 0;
        public const Int32 ARTSCOPE_VAL_WINDOW_LEAVING = 1;

        // ARTSCOPE trigger pulse slope
        public const Int32 ARTSCOPE_VAL_TRIGDIR_PULSE_GREATER = 0;
        public const Int32 ARTSCOPE_VAL_TRIGDIR_PULSE_LESS = 1;
        public const Int32 ARTSCOPE_VAL_TRIGDIR_PULSE_EQUAL = 2;
        public const Int32 ARTSCOPE_VAL_TRIGDIR_PULSE_NEQUAL = 3;

        // ARTSCOPE trigOutPolarity
        public const Int32 ARTSCOPE_VAL_TRIGOUT_NEGATIVE_POLARITY = 0;
        public const Int32 ARTSCOPE_VAL_TRIGOUT_POSITIVE_POLARITY = 1;

        // ARTSCOPE inputClockSource
        public const string ARTSCOPE_VAL_CLK_IN = "VAL_CLK_IN";
        public const string ARTSCOPE_VAL_CLK_ONBOARD = "VAL_CLK_INTERNAL";
        public const string ARTSCOPE_VAL_CLK10M_EXT = "VAL_CLK10M_EXT";
        public const string ARTSCOPE_VAL_CLK10M_PXI = "VAL_CLK10M_PXI";
        public const string ARTSCOPE_VAL_CLK10M_MASTER = "VAL_CLK10M_MASTER";
        public const string ARTSCOPE_VAL_CLK100M_PXIE = "VAL_CLK100M_PXIE";

        // ARTSCOPE ExportClock Values
        public const Int32 ARTSCOPE_VAL_CLK_REF = 0;
        public const Int32 ARTSCOPE_VAL_CLK_SAMPLE = 1;

        public struct ARTSCOPE_STATUS_AD
        {
            public Int32 bADEanble;
            public Int32 bTrigger;
            public Int32 bComplete;
            public Int32 bAheadTrig;
            public UInt64 lCanReadPoint;
            public UInt64 lSavePoints;
        };

        public struct ArtScope_wfmInfo
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] fLsbs;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
            public double[] rangevalue;
            public UInt32 wMaxLSB;
            public Int32 TriggerMode;
            public UInt32 actualSamples;
            public UInt32 pAvailSampsPoints;
            public Int32 channelCount;
        };

        public struct DEVICE_EEP_INFO
        {
            public UInt32 nDLLVer;
            public UInt32 nSysVer;
            public UInt32 nFirmwareVer;
            public UInt32 nCaled;
            public UInt64 nCalDate;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
            public byte[] nReserved;
        };

        // Create Task
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_init(string resourceName,
                                                 ref IntPtr taskHandle);

        // Set Acquisition Mode
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureAcquisitionMode(IntPtr taskHandle,
                                                                     Int32 acquisitionMode);

        // Set Vertical Parameters
        // channelList-"0" means ch0,"0,1" means ch0 and ch1
        // range-the channel range,choose from the above macro definitions "ARTSCOPE range"
        // offset-the offset of channel range,note:some cards don't support this parameter
        // coupling-the coupling of the channel,ARTSCOPE_VAL_DC or ARTSCOPE_VAL_AC
        // probeAttenuation-this parameter is not used for now
        // enabled-if 1,then the channel enable,if 0,then the channel disable
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureVertical(IntPtr taskHandle,
                                                            string channelList,
                                                            Int32 range,
                                                            Int32 offset,
                                                            Int32 coupling,
                                                            Int32 probeAttenuation,
                                                            UInt16 enabled);

        // Set Channel Parameter
        // channelList-channelList-"0" means ch0,"0,1" means ch0 and ch1
        // inputImpedance-channel impedance,ARTSCOPE_IMPED_1M or ARTSCOPE_IMPED_50
        // maxInputFrequency-bandwidth card supported,choose from the above macro definitions "ARTSCOPE bandwidth"
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureChanCharacteristics(IntPtr taskHandle,
                                                                        string channelList,
                                                                        Int32 inputImpedance,
                                                                        Int32 maxInputFrequency);

        // Set Horizontal Parameters
        // minSampleRate-the sample rate set for the acquisition
        // minNumPts-the sample length set for the acquisition
        // refPosition-the trigger position set for the acquisition,0 means post trigger,0~100 means middle trigger,100 means pre-trigger
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureHorizontalTiming(IntPtr taskHandle,
                                                                    double minSampleRate,
                                                                    Int32 minNumPts,
                                                                    double refPosition);

        // Set Edge Trigger Parameters
        // triggerSource-the trigger source,choose from the above macro definitions "ARTSCOPE triggerSource"
        // level-trigger level,the unit is mV
        // slope-trigger slope,choose from the above macro definitions "ARTSCOPE triggerSlope"
        // triggerCoupling-trigger coupling,choose from the above macro definitions "ARTSCOPE trigger coupling"
        // triggerCount-trigger count
        // triggerSensitivity-the duration of this level value after the trigger signal reaches the set level,only the trigger signal that reaches this time is considered valid,the unit is ns
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerEdge(IntPtr taskHandle,
                                                            string triggerSource,
                                                            double level,
                                                            Int32 slope,
                                                            Int32 triggerCoupling,
                                                            Int32 triggerCount,
                                                            Int32 triggerSensitivity);

        // Set Window Trigger Parameters
        // triggerSource-the trigger source,choose from the above macro definitions "ARTSCOPE triggerSource"
        // lowLevel-low level of the window,the unit is mV
        // highLevel-high level of the window,the unit is mV
        // windowMode-window mode,choose from the above macro definitions "ARTSCOPE trigger window type"
        // triggerCoupling-trigger coupling,choose from the above macro definitions "ARTSCOPE trigger coupling"
        // triggerCount-trigger count
        // triggerSensitivity-the duration of this level value after the trigger signal reaches the set level,only the trigger signal that reaches this time is considered valid,the unit is ns
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerWindow(IntPtr taskHandle,
                                                                   string triggerSource,
                                                                   double lowLevel,
                                                                   double highLevel,
                                                                   Int32 windowMode,
                                                                   Int32 triggerCoupling,
                                                                   Int32 triggerCount,
                                                                   Int32 triggerSensitivity);

        // Set Pulse Trigger Parameters
        // triggerSource-the trigger source,choose from the above macro definitions "ARTSCOPE triggerSource"
        // level-trigger level,the unit is mV
        // polarity-trigger polarity,choose from the above macro definitions "ARTSCOPE trigger polarity"
        // slope-trigger slope,choose from the above macro definitions "ARTSCOPE trigger pulse slope"
        // width-pulse width,the range is [16ns,10s]
        // triggerCoupling-trigger coupling,choose from the above macro definitions "ARTSCOPE trigger coupling"
        // triggerCount-the trigger count
        // triggerSensitivity-the duration of this level value after the trigger signal reaches the set level,only the trigger signal that reaches this time is considered valid,the unit is ns
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerPulse(IntPtr taskHandle,
                                                                  string triggerSource,
                                                                  double level,
                                                                  Int32 polarity,
                                                                  Int32 slope,
                                                                  Int32 width,
                                                                  Int32 triggerCoupling,
                                                                  Int32 triggerCount,
                                                                  Int32 triggerSensitivity);

        // Set Digital Trigger Parameters
        // triggerSource-the trigger source,choose from the above macro definitions "ARTSCOPE triggerSource"
        // slope-trigger slope,choose from the above macro definitions "ARTSCOPE triggerSlope"
        // triggerCount-trigger count
        // triggerSensitivity-the duration of this level value after the trigger signal reaches the set level,only the trigger signal that reaches this time is considered valid,the unit is ns
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerDigital(IntPtr taskHandle,
                                                                    string triggerSource,
                                                                    Int32 slope,
                                                                    Int32 triggerCount,
                                                                    Int32 triggerSensitivity);

        // Set Hardware Trigger Parameters
        // delayPoints-the count of points to delay after trigger happen
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerHardwareDelay(IntPtr taskHandle,
                                                                          Int32 delayPoints);

        // Set Software Trigger
        // call this function if use software trigger
        // software trigger has preference to other trigger types
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerSoftWare(IntPtr taskHandle);

        // Set Synchronous Trigger Source
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureTriggerSyncSrc(IntPtr taskHandle,
                                                                    string triggerSyncSource);

        // Set Acquisition Clock Parameters
        // inputClockSource-acquisition clock source
        // freqDivision-frequency division coefficient,only inputClockSource is ARTSCOPE_VAL_CLK_IN,this parameter takes effect
        // masterEnabled-multi card synchronization,set the main card
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ConfigureClock(IntPtr taskHandle,
                                                        string inputClockSource,
                                                        Int32 freqDivision,
                                                        UInt32 masterEnabled);

        // Export Signal
        // signal-signal to export,choose from the above macro definitions "ARTSCOPE ExportClock Values"
        // signalIdentifier-not used for now
        // outputTerminal-not used for now
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ExportClock(IntPtr taskHandle,
                                                        Int32 signal,
                                                        string signalIdentifier,
                                                        string outputTerminal);

        // Export Trigger
        // trigOutWidth-the width of trigger out pulse,the unit is ns,range is [50,50000],step is 50ns
        // trigOutPolarity-trigger out polarity,choose from the above macro definitions "ARTSCOPE trigOutPolarity"
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ExportTrigger(IntPtr taskHandle,
                                                          Int32 trigOutWidth,
                                                          Int32 trigOutPolarity);

        // Auto Save File
        // bIsAutoSave-if 1,save file,if 0,not save
        // chFileName-path of the save file
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_AutoSaveFile(IntPtr taskHandle,
                                                        UInt32 bIsAutoSave,
                                                        string chFileName);

        // Initialize Acquisition
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_InitiateAcquisition(IntPtr taskHandle);

        // Start Acquisition
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_StartAcquisition(IntPtr taskHandle);

        // Send Software Trigger
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_SendSoftwareTrigger(IntPtr taskHandle);

        // Get Acquisition Status
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_AcquisitionStatus(IntPtr taskHandle,
                                                              ref ARTSCOPE_STATUS_AD status);

        // Read Binary Data(BYTE)
        // timeout-timeout,the unit is s
        // numSamples-read length
        // wfmBinary-data buffer
        // wfmInfo-information for analyze
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_FetchBinary8(IntPtr taskHandle,
                                                         double timeout,
                                                         UInt32 numSamples,
                                                         Byte[] wfmBinary,
                                                         ref ArtScope_wfmInfo wfmInfo);

        // Read Binary Data(unsigned short)
        // timeout-timeout,the unit is s
        // numSamples-read length
        // wfmBinary-data buffer
        // wfmInfo-information for analyze
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_FetchBinary16(IntPtr taskHandle,
                                                        double timeout,
                                                        UInt32 numSamples,
                                                        UInt16[] wfmBinary,
                                                        ref ArtScope_wfmInfo wfmInfo);

        // Read Voltage Data
        // some cards don't support
        // timeout-timeout,the unit is s
        // numSamples-read length
        // wfmBinary-data buffer
        // wfmInfo-information for analyze
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_FetchVoltage(IntPtr taskHandle,
                                                         double timeout,
                                                         UInt32 numSamples,
                                                         double[] wfmVoltage,
                                                         ref ArtScope_wfmInfo wfmInfo);

        // Stop Acquisition
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_StopAcquisition(IntPtr taskHandle);

        // Release Acquisition
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ReleaseAcquisition(IntPtr taskHandle);

        // Close Task
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_Close(IntPtr taskHandle);

        // Get Acquisition Channel Count
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ActualNumWfms(IntPtr taskHandle,
                                                          ref UInt32 numWfms);

        // Get Actual Sample Length
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_ActualRecordLength(IntPtr taskHandle,
                                                               ref UInt32 actualRecordLength);

        // Get Actual Sample Rate
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_SampleRate(IntPtr taskHandle,
                                                       ref double actualSampleRate);

        // Calibration
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_Calibration(IntPtr taskHandle);

        // Reset Device
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_Reset(string resourceName);

        // Get information from auto saved file,include the file header size,information used for conversion
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_GetInfoFromAutoSaveFile(string chFileName,
                                                                    ref Int32 fileHeaderSize,
                                                                    ref ArtScope_wfmInfo wfmInfo);

        /*************************Error Handle******************************************************************/
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_GetErrorString(Int32 errorCode,
                                                        byte[] errorString,
                                                        UInt32 bufferSize);

        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_GetExtendedErrorInfo(byte[] errorString,
                                                                 UInt32 bufferSize);

        /********************************Auxiliary Functions***************************************************/
        // Get version information
        [DllImport("ART_SCOPE.dll")]
        public static extern UInt32 ArtScope_GetDeviceEEPInfo(string resourceName,
                                                              ref DEVICE_EEP_INFO devInfo);

        // Get channel count and max sample rate the card supported
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_GetDevInformation(string resourceName,
                                                              ref Int32 channelCount,
                                                              ref float maxSampleRate);

        // Get series number the card belongs to
        // not usually used
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_GetExtenedType(IntPtr taskHandle, ref UInt32 seriesKey);

        // Open binary data file
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_OpenDataFile(string fileName,           // file name
                                                         ref IntPtr hFile,          // file handle
                                                         ref UInt64 dataCount,      // total data count
                                                         ref Int32 chanCount);      // total channel count

        // Convert binary data to voltage data,unit V
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_DataConvertFloat32(IntPtr hFile,                // file handle
                                                               UInt64 dataOffset,           // data offset unit:data count
                                                               UInt32 convertCount,         // convert data count per time(all channel count)
                                                               float[] dataBuf,             // voltage data buffer
                                                               ref UInt32 retConvertCount,  // actual convert count
                                                               Int32 dataCross);            // data ordering(cross or sequence)

        // Convert binary data to voltage data,unit V
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_DataConvertFloat64(IntPtr hFile,                // file handle
                                                               UInt64 dataOffset,           // data offset unit:data count
                                                               UInt32 convertCount,         // convert data count per time(all channel count)
                                                               double[] dataBuf,            // voltage data buffer
                                                               ref UInt32 retConvertCount,  // actual convert count
                                                               Int32 dataCross);            // data ordering(cross or sequence)

        // Close binary data file
        [DllImport("ART_SCOPE.dll")]
        public static extern Int32 ArtScope_CloseDataFile(IntPtr hFile);
    }
}
