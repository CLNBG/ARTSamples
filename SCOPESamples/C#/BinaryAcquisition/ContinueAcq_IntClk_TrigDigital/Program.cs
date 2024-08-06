// binary continuous sample

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

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
            string triggerSource;
            Int32 triggerSlope = ART_SCOPE.ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
            Int32 triggerCount = 1;
            Int32 triggerSensitivity = 0;

            // Temporary variables
            UInt32 numWfms = 0;
            UInt32 actualRecordLength = 0;
            double timeout = 1.0;

            Int32 k = 0;
            double fVolt = 0;
            bool bError = false;

            ART_SCOPE.ArtScope_wfmInfo wfmInfo = new ART_SCOPE.ArtScope_wfmInfo();

            Console.Write("Please enter the device name:");
            resourceName = Console.ReadLine();

            if (ART_SCOPE.ArtScope_init(resourceName, ref taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

            channelName = "0";
            minSampleRate = 10000000;
            minRecordLength = 1024;
            refPosition = 0.0;
            triggerSource = ART_SCOPE.ARTSCOPE_VAL_TRIGSRC_DTR;

            if (ART_SCOPE.ArtScope_ConfigureAcquisitionMode(taskHandle, acquisitionMode) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureVertical(taskHandle, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureHorizontalTiming(taskHandle, minSampleRate, minRecordLength, refPosition) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureTriggerDigital(taskHandle, triggerSource, triggerSlope, triggerCount, triggerSensitivity) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualNumWfms(taskHandle, ref numWfms) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_InitiateAcquisition(taskHandle) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualRecordLength(taskHandle, ref actualRecordLength) < 0)
            {
                bError = true;
                goto Error_close;
            }

            UInt16[] waveformPtr = new UInt16[actualRecordLength * numWfms];

            if (ART_SCOPE.ArtScope_StartAcquisition(taskHandle) < 0)
            {
                bError = true;
                goto Error_label;
            }
            while (_kbhit() == 0)
            {
                if (ART_SCOPE.ArtScope_FetchBinary16(taskHandle, timeout, actualRecordLength, waveformPtr, ref wfmInfo) < 0)
                {
                    Console.WriteLine("Fetch Binary16 error!");
                    goto Error_label;
                }
                for (k = 0; k < numWfms; k++)
                {
                    Console.Write("{0,8:d}\t", waveformPtr[k]);
                }
                Console.WriteLine("");
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
            }
            ART_SCOPE.ArtScope_Close(taskHandle);

            Console.WriteLine("Press any key to quit...");
            _getch();
        }
    }
}
