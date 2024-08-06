// Auto save sample

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace Sys
{
    class Program
    {
        [DllImport("msvcrt.dll")]
        public static extern Int32 _getch();
        [DllImport("msvcrt.dll")]
        public static extern Int32 _kbhit();

        static void Main(string[] args)
        {
            IntPtr taskHandle = IntPtr.Zero;

            string resourceName;
            string channelName;
            string fileName = "D:\\1.pci";
            UInt32 bSaveFile = 0;

            // Set the acquisition mode
            Int32 acquisitionMode = ART_SCOPE.ARTSCOPE_SAMPMODE_COUPLING_CONTINUOUS;

            // Set vertical parameters
            Int32 verticalRange = ART_SCOPE.ARTSCOPE_RANGE_10Vpp;
            Int32 verticalOffset = 0;
            Int32 verticalCoupling = ART_SCOPE.ARTSCOPE_VAL_DC;

            // Set horizontal parameters
            double minSampleRate = 0;
            Int32 minRecordLength = 1000;
            double refPosition = 0.0;

            // Set trigger parameters
            double triggerLevel = 0;
            string triggerSource;
            Int32 triggerSlope = ART_SCOPE.ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
            Int32 triggerCoupling = ART_SCOPE.ARTSCOPE_VAL_TRIGCOUP_DC;
            Int32 triggerCount = 1;
            Int32 triggerSensitivity = 0;

            // Temporary variables
            UInt32 numWfms = 0;
            UInt32 actualRecordLength = 0;

            Int32 k = 0;
            double fVolt = 0;
            double timeout = 1.0;
            bool bError = false;

            Int32 fileHeaderSize = 0;
            ART_SCOPE.ArtScope_wfmInfo filewfmInfo = new ART_SCOPE.ArtScope_wfmInfo();
            ART_SCOPE.ArtScope_wfmInfo wfmInfo = new ART_SCOPE.ArtScope_wfmInfo();

            ART_SCOPE.ARTSCOPE_STATUS_AD AD_Status = new ART_SCOPE.ARTSCOPE_STATUS_AD();

            Console.Write("Please enter the device name:");
            resourceName = Console.ReadLine();

            // Create task
            if (ART_SCOPE.ArtScope_init(resourceName, ref taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

            Console.Write("Please enter the file path and file name(like:D:\\1.pci):");
            fileName = Console.ReadLine();

            channelName = "0";
            minSampleRate = 10000000;
            minRecordLength = 1024;
            refPosition = 0.0;
            triggerSource = ART_SCOPE.ARTSCOPE_VAL_TRIGSRC_CH0;
            bSaveFile = 1;

            // Set acquisition mode
            if (ART_SCOPE.ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Set vertical parameters
            if (ART_SCOPE.ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Set horizontal parameters
            if (ART_SCOPE.ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minRecordLength, refPosition) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Set edge trigger parameters
            // Note:different cards support different trigger modes
            if (ART_SCOPE.ArtScope_ConfigureTriggerEdge(taskHandle, triggerSource, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Set auto save is true
            if (ART_SCOPE.ArtScope_AutoSaveFile(taskHandle, bSaveFile, fileName) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Get actual sample channel count
            if (ART_SCOPE.ArtScope_ActualNumWfms(taskHandle, ref numWfms) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Initialize the acquisition
            if (ART_SCOPE.ArtScope_InitiateAcquisition(taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Get the actual record length
            // There is some limits of the record length,so here you can use this function to get the actual record length after initialize the acquisition.
            if (ART_SCOPE.ArtScope_ActualRecordLength(taskHandle, ref actualRecordLength) < 0)
            {
                bError = true;
                goto Error_close;
            }

            UInt16[] waveformPtr = new UInt16[actualRecordLength * numWfms];

            // Start acquisition
            if (ART_SCOPE.ArtScope_StartAcquisition(taskHandle) < 0)
            {
                bError = true;
                goto Error_label;
            }
            if (bSaveFile == 1)
            {
                while (_kbhit() == 0)
                {
                    // Call the fetch function to save data,data has been saved inside the function.
                    if (ART_SCOPE.ArtScope_FetchBinary16(taskHandle, timeout, actualRecordLength, waveformPtr, ref wfmInfo) < 0)
                    {
                        bError = true;
                        goto Error_label;
                    }
                    // You can see the data in the waveformPtr
                    for (k = 0; k < numWfms; k++)
                    {
                        // Here is the conversion from binary data to voltage data,the unit of voltage data is mV.
                        fVolt = wfmInfo.fLsbs[k % numWfms] * (waveformPtr[k] & wfmInfo.wMaxLSB) - wfmInfo.rangevalue[k % numWfms];
                        Console.Write("{0,8:f2}(mV)\t", fVolt);
                    }
                    Console.WriteLine("");
                }
            }

            _getch();

        Error_label:
            if (taskHandle != IntPtr.Zero)
            {
                ART_SCOPE.ArtScope_StopAcquisition(taskHandle);
                ART_SCOPE.ArtScope_ReleaseAcquisition(taskHandle);
            }

        Error_close:
            // Get wrong information
            if (bError == true)
            {
                byte[] strError = new byte[2048];
                ART_SCOPE.ArtScope_GetExtendedErrorInfo(strError, 2048);
                string str = System.Text.Encoding.Default.GetString(strError);
                Console.WriteLine(str.Replace("\n\n", "\n").Replace("\0", ""));
                if (taskHandle != IntPtr.Zero)
                {
                    ART_SCOPE.ArtScope_Close(taskHandle);
                }
                Console.WriteLine("Press any key to quit...");
                _getch();
                return;
            }
            // Close the task
            ART_SCOPE.ArtScope_Close(taskHandle);

            // Analyze the auto-saved file
            // Use this function to get the size of file header,and some information used to convert binary data to voltage data
            Int32 ret = ART_SCOPE.ArtScope_GetInfoFromAutoSaveFile(fileName, ref fileHeaderSize, ref filewfmInfo);

            // Get the data count in the file
            // Here dividing 2,since one binary data has two bytes
            long fileLen = (new FileInfo(fileName).Length - fileHeaderSize) / 2;

            FileStream fs = new FileStream(fileName, FileMode.Open);
            BinaryReader br = new BinaryReader(fs);
            byte[] header = new byte[fileHeaderSize];
            br.Read(header, 0, fileHeaderSize);

            while (fileLen > 0 && _kbhit() == 0)
            {
                UInt16[] dataBuf = new UInt16[1024];
                double[] voltData = new double[1024];
                if (fileLen >= 1024)
                {
                    for (k = 0; k < 1024; k++)
                    {
                        dataBuf[k] = br.ReadUInt16();
                        voltData[k] = (filewfmInfo.fLsbs[k % filewfmInfo.channelCount] * (dataBuf[k] & filewfmInfo.wMaxLSB) - filewfmInfo.rangevalue[k % filewfmInfo.channelCount]) / 1000f;
                        if (k > 0 && (k % filewfmInfo.channelCount == 0))
                            Console.WriteLine();
                        Console.Write("{0,8:f4}(mV)\t", voltData[k]);
                    }
                    Console.WriteLine();
                }
                else
                {
                    for (k = 0; k < fileLen; k++)
                    {
                        dataBuf[k] = br.ReadUInt16();
                        voltData[k] = (filewfmInfo.fLsbs[k % filewfmInfo.channelCount] * (dataBuf[k] & filewfmInfo.wMaxLSB) - filewfmInfo.rangevalue[k % filewfmInfo.channelCount]) / 1000f;
                        if (k > 0 && (k % filewfmInfo.channelCount == 0))
                            Console.WriteLine();
                        Console.Write("{0,8:f4}(mV)\t", voltData[k]);
                    }
                    Console.WriteLine();
                }
                break;
                //fileLen = fileLen - 1024;
            }

            Console.WriteLine("Press any key to quit...");
            _getch();
        }
    }
}
