//******************************************************
// ��������ʾ���������ڲ�������
//******************************************************
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
                Console.WriteLine("������ϢΪ:{0}", str);
            }
            return;     
        }
       
        static void Main(string[] args)
        {
           
            Int32 error = 0;
            string errorInfo = new string('0', 2048);
            byte[] errBuff = new byte[2048];
            IntPtr taskHandle = (IntPtr)(0);
            string strTaskName = string.Empty;
            string strCardName = string.Empty;
            string strChannelName = string.Empty;

            /*********************************************/
            // ArtDAQ Configure Code
            /*********************************************/
            Console.WriteLine("������������:(��:task1,���ֻ��һ�������������Ϊ��)");
            strTaskName = Console.ReadLine();
            //step1: CreateTask
            error = ArtDAQ.ArtDAQ_CreateTask(strTaskName, out taskHandle);

            Console.WriteLine("������忨��DMC��ʶ���������:(��:Dev1)");
            strCardName = Console.ReadLine();
            Console.WriteLine("������ͨ����:(��:ctr0)");
            strChannelName = Console.ReadLine();
            strChannelName = strCardName + "/" + strChannelName;
            //step2: CreateChannel
            error = ArtDAQ.ArtDAQ_CreateCOPulseChanFreq(taskHandle, strChannelName, "", ArtDAQ.ArtDAQ_Val_Hz, ArtDAQ.ArtDAQ_Val_Low, 0.0, 1000.00, 0.50);
            GetErrorString(error);
            if (error < 0)
            {
                goto Exit;

            }
            //step3: Configure Clock
            error = ArtDAQ.ArtDAQ_CfgImplicitTiming(taskHandle, ArtDAQ.ArtDAQ_Val_ContSamps, 1000);
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
               Console.WriteLine("Generating pulse train. Press Enter to interrupt\n"); 
                goto Exit;
            }
            Console.WriteLine("End of program, press Enter key to quit\n");
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

