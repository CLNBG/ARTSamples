//***************************************************************
// 本例程演示DI操作流程 数字触发
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
            string errorInfo = new string('0', 2048);
            IntPtr taskHandle = (IntPtr)(0);
            Int32 read = 0;
            Int32 bytesPerSamp;
            byte[] data = new byte[8000];
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strPortName = string.Empty;
            string strLineName = string.Empty;
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
            Console.WriteLine("请输入端口名:(如:port0)");
            strPortName = Console.ReadLine();
            Console.WriteLine("请输入Line名:(如:line0:7)");
            strLineName = Console.ReadLine();
            strLineName = strCardName + "/" + strPortName + "/" + strLineName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateDIChan(taskHandle, strLineName, "", ArtDAQ.ArtDAQ_Val_ChanPerLine);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step3:Configure Clock
            error = ArtDAQ.ArtDAQ_CfgSampClkTiming(taskHandle, "", 10000.0, ArtDAQ.ArtDAQ_Val_Rising, ArtDAQ.ArtDAQ_Val_FiniteSamps, 1000);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            strSource = "/" + strCardName + "/PFI0";
            //step4:Configure Trigger
            error = ArtDAQ.ArtDAQ_CfgDigEdgeStartTrig(taskHandle, "/Dev1/PFI0", ArtDAQ.ArtDAQ_Val_Rising);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step5: Start Task
            error = ArtDAQ.ArtDAQ_StartTask(taskHandle);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }

            //step6: Read
            error = ArtDAQ.ArtDAQ_ReadDigitalLines(taskHandle, 1000, 10.0, ArtDAQ.ArtDAQ_Val_GroupByChannel, data, 8000, out read, out bytesPerSamp, (IntPtr)0);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;
            }
            for (int k = 0; k < read; k++)
            {
                Console.WriteLine("DI data={0}", data[k]);
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

