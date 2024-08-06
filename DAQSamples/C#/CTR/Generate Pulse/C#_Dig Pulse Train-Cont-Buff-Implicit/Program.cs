//***************************************************************
// 本程序演示计数器边沿计数流程
//***************************************************************
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Sys
{
    class Program
    {
        [DllImport("msvcrt.dll")]
        public static extern int _getch();
        [DllImport("msvcrt.dll")]
        public static extern int _kbhit();

        public static void GetErrorString(Int32 errorCode)
        {
            if (errorCode < 0)
            {
                byte[] errorInfo = new byte[2048];
                ArtDAQ.ArtDAQ_GetExtendedErrorInfo(errorInfo, 2048);
                string str = Encoding.Default.GetString(errorInfo);
                Console.WriteLine("错误信息为:{0}", str);
            }
            return;
        }
        public static Double DUTY_MIN = 0.50;
        public static Double DUTY_MAX = 0.80;

        static void Main(string[] args)
        {

            Int32 error = 0;
            string errorInfo = new string('0', 2048);
            IntPtr taskHandle = (IntPtr)(0);
            Int32 i;
            Double[] dataFreq = new Double[1000];
            Double[] dataDC = new Double[1000];
            Double dutyStep = 0.0;
            Int32 numSampsPerChanWritten = 0;
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strChannelName = string.Empty;

            // Generating data for duty cycle from 0.50 to 0.80 in 1000 steps
            dutyStep = (DUTY_MAX - DUTY_MIN) / 1000;
            for (i = 0; i < 1000; ++i)
            {
                dataFreq[i] = 1000.0;
                dataDC[i] = DUTY_MIN + dutyStep * i;
            }

            /*********************************************/
            // ArtDAQ Configure Code
            /*********************************************/
            Console.WriteLine("请输入任务名:(如:task1,如果只有一个任务可以设置为空)");
            strTaskName = Console.ReadLine();
            //step1: CreateTask
            error = ArtDAQ.ArtDAQ_CreateTask(strTaskName, out taskHandle);

            Console.WriteLine("请输入板卡在DMC内识别出的名字:(如:Dev1)");
            strCardName = Console.ReadLine();
            Console.WriteLine("请输入通道名:(如:ctr0)");
            strChannelName = Console.ReadLine();
            strChannelName = strCardName + "/" + strChannelName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateCOPulseChanFreq(taskHandle, strChannelName, "", ArtDAQ.ArtDAQ_Val_Hz, ArtDAQ.ArtDAQ_Val_Low, 0.0, 1.00, 0.50);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step3: Configure Timing
            error = ArtDAQ.ArtDAQ_CfgImplicitTiming(taskHandle, ArtDAQ.ArtDAQ_Val_ContSamps, 1000);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            ////step4: Write Task
            //error = ArtDAQ.ArtDAQ_RegisterDoneEvent(taskHandle, 0, DoneCallback, (IntPtr)0);
            //GetErrorString(error);
            //if (error < 0)
            //{
            //    goto Exit;
            //}

            //step4: Write Task
            error = ArtDAQ.ArtDAQ_WriteCtrFreq(taskHandle, 1000, 0, 10.0, dataFreq, dataDC, out numSampsPerChanWritten, (IntPtr)0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step4: Start Task
            error = ArtDAQ.ArtDAQ_StartTask(taskHandle);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }
            _getch();

            //step5: Exit ReleaseTask
        Exit:
            ArtDAQ.ArtDAQ_StopTask(taskHandle);
            ArtDAQ.ArtDAQ_ClearTask(taskHandle);
            _getch();
            return;

        }

    }
}

