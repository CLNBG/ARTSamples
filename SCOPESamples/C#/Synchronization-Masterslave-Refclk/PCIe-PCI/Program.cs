///////////////////////////////////////////////////
// This example program demonstrates multi cards synchronization in PCIE PCI bus using master-slave way.

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
            IntPtr taskHandle_master = IntPtr.Zero;
            IntPtr taskHandle_slave = IntPtr.Zero;
            List<EventWaitHandle> manualEvents = new List<EventWaitHandle>();

            string resourceName_master;
            string resourceName_slave;
            string channelName;

            // Set the acquisition mode
            Int32 acquisitionMode = ART_SCOPE.ARTSCOPE_SAMPMODE_FINITE;

            // Set vertical parameters
            Int32 verticalRange = ART_SCOPE.ARTSCOPE_RANGE_10Vpp;
            Int32 verticalOffset = 0;
            Int32 verticalCoupling = ART_SCOPE.ARTSCOPE_VAL_DC;

            UInt32 numWfms_master = 0, numWfms_slave = 0;
            // Set horizontal parameters
            double minSampleRate = 0;
            Int32 minRecordLength = 1000;
            double refPosition = 0.0;

            // Set trigger parameters
            float triggerLevel = 0;
            string triggerSource_master;
            string triggerSource_slave;
            string triggerSyncPulseSouce;
            Int32 triggerSlope = ART_SCOPE.ARTSCOPE_VAL_TRIGDIR_NEGATIVE;
            Int32 triggerCoupling = ART_SCOPE.ARTSCOPE_VAL_TRIGCOUP_DC;
            Int32 triggerCount = 1;
            Int32 triggerSensitivity = 0;

            // Temporary variables
            UInt32 actualRecordLength_master = 0, actualRecordLength_slave = 0;
            double timeout = 5.0;
            bool bError = false;

            Console.Write("Please enter the master device name:");
            resourceName_master = Console.ReadLine();
            Console.Write("Please enter the slave device name:");
            resourceName_slave = Console.ReadLine();

            triggerSource_master = ART_SCOPE.ARTSCOPE_VAL_TRIGSRC_CH0;
            triggerSource_slave = ART_SCOPE.ARTSCOPE_VAL_TRIGSRC_SYNC;
            triggerSyncPulseSouce = ART_SCOPE.ARTSCOPE_VAL_TRIG_SYNC_0;

            channelName = "0";
            minSampleRate = 10000000;
            minRecordLength = 1024;
            refPosition = 0.0;
            acquisitionMode = ART_SCOPE.ARTSCOPE_SAMPMODE_FINITE;

            // Master card parameters
            if (ART_SCOPE.ArtScope_init(resourceName_master, ref taskHandle_master) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureAcquisitionMode(taskHandle_master, acquisitionMode) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureVertical(taskHandle_master, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureHorizontalTiming(taskHandle_master, minSampleRate, minRecordLength, refPosition) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureTriggerSyncSrc(taskHandle_master, triggerSyncPulseSouce) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureTriggerEdge(taskHandle_master, triggerSource_master, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureClock(taskHandle_master, ART_SCOPE.ARTSCOPE_VAL_CLK_ONBOARD, 1, 1) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ExportClock(taskHandle_master, ART_SCOPE.ARTSCOPE_VAL_CLK_REF, "", "") < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualNumWfms(taskHandle_master, ref numWfms_master) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_InitiateAcquisition(taskHandle_master) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualRecordLength(taskHandle_master, ref actualRecordLength_master) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // Slave card parameters
            if (ART_SCOPE.ArtScope_init(resourceName_slave, ref taskHandle_slave) < 0)
            {
                bError = true;
                return;
            }

            if (ART_SCOPE.ArtScope_ConfigureAcquisitionMode(taskHandle_slave, acquisitionMode) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureVertical(taskHandle_slave, channelName, verticalRange, verticalOffset, verticalCoupling, 1, 1) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureHorizontalTiming(taskHandle_slave, minSampleRate, minRecordLength, refPosition) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureTriggerSyncSrc(taskHandle_slave, triggerSyncPulseSouce) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureTriggerEdge(taskHandle_slave, triggerSource_slave, triggerLevel, triggerSlope, triggerCoupling, triggerCount, triggerSensitivity) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ConfigureClock(taskHandle_slave, ART_SCOPE.ARTSCOPE_VAL_CLK10M_MASTER, 1, 0) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualNumWfms(taskHandle_slave, ref numWfms_slave) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_InitiateAcquisition(taskHandle_slave) < 0)
            {
                bError = true;
                goto Error_close;
            }

            if (ART_SCOPE.ArtScope_ActualRecordLength(taskHandle_slave, ref actualRecordLength_slave) < 0)
            {
                bError = true;
                goto Error_close;
            }

            // First start slave card
            if (ART_SCOPE.ArtScope_StartAcquisition(taskHandle_slave) < 0)
            {
                bError = true;
                goto Error_label;
            }
            // Start thread to read data of slave card
            ManualResetEvent handle_slave = new ManualResetEvent(false);
            manualEvents.Add(handle_slave);
            ReadThread_Slave slavereadclass = new ReadThread_Slave(taskHandle_slave, timeout, actualRecordLength_slave, numWfms_slave, handle_slave);
            Thread readthreadslave = new Thread(slavereadclass.ReadData);
            readthreadslave.Start();

            // Start master card
            if (ART_SCOPE.ArtScope_StartAcquisition(taskHandle_master) < 0)
            {
                bError = true;
                goto Error_label;
            }
            // Start thread to read data of master card
            ManualResetEvent handle_master = new ManualResetEvent(false);
            manualEvents.Add(handle_master);
            ReadThread_Master masterreadclass = new ReadThread_Master(taskHandle_master, timeout, actualRecordLength_master, numWfms_master, handle_master);
            Thread readthreadmaster = new Thread(masterreadclass.ReadData);
            readthreadmaster.Start();

            // wait for thread to exit
            WaitHandle.WaitAll(manualEvents.ToArray());

            Console.WriteLine("Press any key to quit...");
            _getch();
            return;

        Error_label:
            if (taskHandle_master != IntPtr.Zero)
            {
                ART_SCOPE.ArtScope_StopAcquisition(taskHandle_master);
                ART_SCOPE.ArtScope_ReleaseAcquisition(taskHandle_master);
            }
            if (taskHandle_slave != IntPtr.Zero)
            {
                ART_SCOPE.ArtScope_StopAcquisition(taskHandle_slave);
                ART_SCOPE.ArtScope_ReleaseAcquisition(taskHandle_slave);
            }

        Error_close:
            // get error information
            if (bError == true)
            {
                byte[] strError = new byte[2048];
                ART_SCOPE.ArtScope_GetExtendedErrorInfo(strError, 2048);
                string str = System.Text.Encoding.Default.GetString(strError);
                Console.WriteLine(str.Replace("\n\n", "\n").Replace("\0", ""));
            }
            if (taskHandle_master != IntPtr.Zero)
                ART_SCOPE.ArtScope_Close(taskHandle_master);
            if (taskHandle_slave != IntPtr.Zero)
                ART_SCOPE.ArtScope_Close(taskHandle_slave);

            Console.WriteLine("Press any key to quit...");
            _getch();
        }
    }
}
