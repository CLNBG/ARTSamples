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

        static void Main(string[] args)
        {

            Int32 error = 0;
            IntPtr taskHandle = (IntPtr)(0);
            Int32 read = 0;
            UInt32[] data = new UInt32[1000];
            UInt32[] position = new UInt32[1000];
            UInt32[] pulseWidth = new UInt32[1000];
            Int32 written=0;
            string errBuff = new string('0', 2048);
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strChannelName = string.Empty;
            string strSource = string.Empty;

            for (int i = 0; i < 1000; i++)
            {
                position[i] = (UInt32)(10*(i+1));
                pulseWidth[i] = 500 * (UInt32)(i + 1);    // 单位嘀嗒，时基100M  
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
            Console.WriteLine("请输入输出引脚:(如:PFI11)");
            strSource = Console.ReadLine();
            strSource = "/"+strCardName + "/" + strSource;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateCICountEdgesChan(taskHandle, strChannelName, "", ArtDAQ.ArtDAQ_Val_Rising, 0, ArtDAQ.ArtDAQ_Val_CountUp);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }
            error = ArtDAQ.ArtDAQ_ExportPositionComparsionEvent(taskHandle, strSource, ArtDAQ.ArtDAQ_Val_Toggle, ArtDAQ.ArtDAQ_Val_ActiveLow, ArtDAQ.ArtDAQ_Val_High, ArtDAQ.ArtDAQ_Val_PulseWidth_Implicit);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            error = ArtDAQ.ArtDAQ_WritePositionComparsionData(taskHandle, 10, position, pulseWidth, ref written,(IntPtr)0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }


            //step3: Start Task
            error = ArtDAQ.ArtDAQ_StartTask(taskHandle);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step4: ReadData
            while (_kbhit() == 0)
            {
                error = ArtDAQ.ArtDAQ_ReadCounterU32(taskHandle,1000,10.0,data,1000,out read,(IntPtr)0);
                GetErrorString(error);
                if (error < 0)
                {
                    goto Exit;
                }
                Console.WriteLine("Acquired count={0}", read);
            }

            //step5: Exit ReleaseTask
        Exit:
            ArtDAQ.ArtDAQ_StopTask(taskHandle);
            ArtDAQ.ArtDAQ_ClearTask(taskHandle);
            _getch();
            return;

        }
    }
}

