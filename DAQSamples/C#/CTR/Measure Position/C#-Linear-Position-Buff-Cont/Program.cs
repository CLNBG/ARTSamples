//***********************************************************
// 本例程演示计数器例程 外部时钟源 连续模式下计数流程
//***********************************************************
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

        static void Main(string[] args)
        {

            Int32 error = 0;
            IntPtr taskHandle = (IntPtr)(0);
            Int32 read = 0;
            Double[] data = new Double[1000];
            string errBuff = new string('0', 2048);

            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strChannelName = string.Empty;
            string strSource = string.Empty;

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
            error = ArtDAQ.ArtDAQ_CreateCILinEncoderChan(taskHandle, strChannelName, "", ArtDAQ.ArtDAQ_Val_X1, 1, 0.0, ArtDAQ.ArtDAQ_Val_AHighBHigh, ArtDAQ.ArtDAQ_Val_Meters, 0.001, 0.0, "");
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            strSource = "/" + strCardName + "/PFI4";
            //step3: Configure Clock
            error = ArtDAQ.ArtDAQ_CfgSampClkTiming(taskHandle, strSource, 1000.0, ArtDAQ.ArtDAQ_Val_Rising, ArtDAQ.ArtDAQ_Val_ContSamps, 1000);
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

            Console.WriteLine("Continuously reading. Press Ctrl+C to interrupt\n");
            //step5: ReadData
            while (_kbhit() == 0)
            {
                error = ArtDAQ.ArtDAQ_ReadCounterF64(taskHandle, 1000, 10.0, data, 1000, out read, (IntPtr)0);
                GetErrorString(error);
                if (error < 0)
                {
                    goto Exit;
                }
                Console.WriteLine("Acquired {0:d} samples", read);
                for (int i = 0; i < (int)read; i++)
                {
                    Console.WriteLine("采集的数据为 {0:f}", data[i]);
                }	
            }

            //step6: Exit ReleaseTask
        Exit:
            ArtDAQ.ArtDAQ_StopTask(taskHandle);
            ArtDAQ.ArtDAQ_ClearTask(taskHandle);
            _getch();
            return;

        }
    }
}

